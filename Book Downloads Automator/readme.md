
# Gutenberg Downloads Automator

A script to download a certain list of books from [Project Gutenberg](https://www.gutenberg.org/). Guide made for Windows 10, it also works in others OS if you get correctly the requirements and the webdriver.

**Author:** [sjcr23](https://sjcr23.github.io)

## Requirements

- [Python & Pip](https://www.python.org/)
- [Selenium library](https://pypi.org/project/selenium/)

    ```bash
    pip install -U selenium
    ```

- [Google Chrome](https://www.google.com/chrome/)
- [Google Chrome web driver](https://chromedriver.chromium.org/downloads)

> Note: In this repository is already included the [ChromeDriver 114.0.5735.90](https://chromedriver.storage.googleapis.com/index.html?path=114.0.5735.90/). For the future, you may update the driver or your Google Chromes's version.

## Usage

1. Create a folder named `output` in the same directory level with `downloader.py`, or just make sure that the variable `resources_path` has the same name as the output folder created.

    ```python
    # Directory to file storage "output"
    resources_path = "output"
    ```

3. Run  `downloader.py`

    ```bash
    python downloader.py
    ```
