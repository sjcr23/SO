from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.common.by import By
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.wait import WebDriverWait
import codecs
import books
import logging

def load_logger(name):
    logger = logging.getLogger(name)
    logger.setLevel(logging.DEBUG)
    ch = logging.StreamHandler()
    ch.setLevel(logging.DEBUG)
    formatter = logging.Formatter('[%(asctime)s][%(levelname)s] %(message)s')
    ch.setFormatter(formatter)
    logger.addHandler(ch)
    return logger

def filter_special_chars(name):
    specials = [":", "/", "*", "?", '"', "<", ">", "|"]
    for char in specials:
        name = name.replace(char, " ")
    return name
    
def log(name, founded =True):
    if founded:
        logger.info(f' Downloaded: {name[:15]}...')
    else:
        logger.error(f'Not founded: {name[:15]}...')


# Directory to file storage
resources_path = "output"

# Logger
logger = load_logger("top30 downloads")

# Selenium configs
service = Service(executable_path="chromedriver.exe")
driver = webdriver.Chrome(service=service)

# Open website
driver.get(books.url)

# Book file type for download
format_type = "Plain Text UTF-8"

for name in books.top30:
    # Open book in new tab
    book_link = driver.find_element(By.LINK_TEXT, name)
    book_link.click()
    not_founded = False

    # Looks for UTF-8 file type
    try:
        WebDriverWait(driver, 0.1).until(EC.presence_of_element_located((By.LINK_TEXT, format_type)))
        not_founded = False
    except:
        not_founded = True

    # Goes back if not available
    if not_founded:
        driver.back()
        log(name, founded=False)
        continue
    
    # Open plain text in new tab
    text = driver.find_element(By.LINK_TEXT, format_type)
    text.click()
    
    file = codecs.open(f'{resources_path}/{filter_special_chars(name)}.txt', "w", "utf-8")
    file.write(driver.page_source)
    file.close()

    # Go to main page
    driver.back()
    driver.back()
    log(name)


