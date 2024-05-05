from collections import deque

class FIFO:
    def __init__(self, ram_size, page_size, real_memory, virtual_memory):
        self.ram_size = ram_size
        self.page_size = page_size
        self.real_memory = real_memory
        self.virtual_memory = virtual_memory
        self.fifo_queue = deque() # queue to track the order of page allocation

    def update_queue(self, page):
        self.fifo_queue.append(page)
        if len(self.fifo_queue) > self.ram_size // self.page_size:
            self.evict_page(page)

    def evict_page(self, page):
        if self.fifo_queue and self.real_memory:
            evicted_page = self.fifo_queue.popleft()
            print(f"Page to evict: ID: {evicted_page.page_id}")

            index = self.real_memory.index(evicted_page)
            page.page_id = index
            page.in_real_memory = True
            page.physical_address = index
            self.real_memory[index] = page
            print(f"Page: {page.page_id} moved to real memory")
            
            # Remove the page from virtual memory
            self.virtual_memory.remove(page)
            print(f"Page {page.page_id} was removed from virtual memory")

            evicted_page.page_id = None
            evicted_page.in_real_memory = False
            evicted_page.physical_address = None
            self.virtual_memory.append(evicted_page)
            print(f"Evicted page {evicted_page.page_id} moved to virtual memory")
    

class SecondChance:
    def __init__(self, ram_size, page_size):
        self.ram_size = ram_size
        self.page_size = page_size
        self.sc_queue = deque() # Queue to track the order of page allocation
        self.referenced = set() # Set the track referenced pages

    def update_queue(self, ptr):
        self.sc_queue.append(ptr)
        if len(self.sc_queue) > self.ram_size // self.page_size:
            self.evict_page() # Check and evict a page if necessary
        
    def evict_page(self):
        while True:
            ptr = self.sc_queue.popleft()
            if ptr not in self.referenced:
                return ptr
            self.referenced.remove(ptr)
            self.sc_queue.append(ptr) # Put the page back in the queue

    def reference_page(self, ptr):
        self.referenced.add(ptr)