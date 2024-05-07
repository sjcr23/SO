from collections import deque
import random

class PageReplacementAlgorithm:
    def __init__(self, ram_size, page_size, real_memory, virtual_memory):
        self.ram_size = ram_size
        self.page_size = page_size
        self.real_memory = real_memory
        self.virtual_memory = virtual_memory
        self.page_queue = deque()

    def update_queue(self, new_page):
        if len(self.page_queue) >= self.ram_size // self.page_size:
            self.evict_page(new_page)
        else:
            self.page_queue.append(new_page)

    def evict_page(self, new_page):
        raise NotImplementedError("Subclasses must implement evict_page method")
    
    def move_to_real_memory(self, new_page, evicted_page):
        index = self.real_memory.index(evicted_page)
        new_page.in_real_memory = True
        new_page.physical_address = index
        new_page.referenced = False
        self.real_memory[index] = new_page
        self.page_queue.append(new_page)
        print(f"Page: {new_page.page_id} moved to real memory")

    def move_to_virtual_memory(self, page):
        page.in_real_memory = False
        page.physical_address = None
        self.virtual_memory.append(page)
        print(f"Evicted page moved to virtual memory")

    def remove_from_virtual_memory(self, page):
        # Remove the page from virtual memory
        self.virtual_memory.remove(page)
        print(f"Page {page.page_id} was removed from virtual memory")

    def access_page(self, page):
        raise NotImplementedError("Subclasses must implement access_page method")

    def delete_page(self, page):
        self.page_queue.remove(page)


class FIFO(PageReplacementAlgorithm):
    def update_queue(self, new_page):
        return super().update_queue(new_page)

    def evict_page(self, new_page):
        if self.page_queue and self.real_memory:
            evicted_page = self.page_queue.popleft()
            print(f"Page to evict: ID: {evicted_page.page_id}")
            # Move the new page from virtual memory to real memory
            self.move_to_real_memory(new_page, evicted_page)
            # Remove the page from virtual memory
            self.remove_from_virtual_memory(new_page)
            # Move evicted page to virtual memory
            self.move_to_virtual_memory(evicted_page)

    def access_page(self, page):
        return 
    
    def delete_page(self, page):
        return super().delete_page(page)
    

class SecondChance(PageReplacementAlgorithm):
    def update_queue(self, new_page):
        return super().update_queue(new_page)
        
    def evict_page(self, new_page):
        while True:
            pageInRealMemory = self.page_queue.popleft()
            if not pageInRealMemory.referenced:
                print(f"Page {pageInRealMemory.page_id} is going to be evicted from real memory")
                # if the page is not referenced, it can be evicted
                # Move the new page from virtual memory to real memory
                self.move_to_real_memory(new_page, pageInRealMemory)
                # Remove the page from virtual memory
                self.remove_from_virtual_memory(new_page)
                # Move evicted page to virtual memory
                self.move_to_virtual_memory(pageInRealMemory)
                return
            else:
                # If the page is referenced, give it a second chance and change its bit to False and move it to the end of the queue
                print(f"Giving a second chance to page {pageInRealMemory.page_id}")
                pageInRealMemory.referenced = False
                self.page_queue.append(pageInRealMemory)

    def access_page(self, page):
        index = self.page_queue.index(page)
        self.page_queue[index].referenced = True

    def delete_page(self, page):
        return super().delete_page(page)


class MRU(PageReplacementAlgorithm):
    def update_queue(self, new_page):
        return super().update_queue(new_page)

    def evict_page(self, new_page):
        if self.page_queue and self.real_memory:
            # Find the most recently used page in the queue (last element)
            evicted_page = self.page_queue.pop()
            print(f"Page to evict: ID: {evicted_page.page_id}")
            # Move the new page from virtual memory to real memory
            self.move_to_real_memory(new_page, evicted_page)
            # Remove the page from virtual memory
            self.remove_from_virtual_memory(new_page)
            # Move evicted page to virtual memory
            self.move_to_virtual_memory(evicted_page)

    def access_page(self, page):
        # move the element to the front of the stack
        # it means it's the last element to be used
        self.page_queue.remove(page)
        self.page_queue.append(page)

    def delete_page(self, page):
        return super().delete_page(page)
    

class RND(PageReplacementAlgorithm):
    def update_queue(self, new_page):
        return super().update_queue(new_page)

    def evict_page(self, new_page):
        if self.page_queue and self.real_memory:
            # Select a random page from the queue
            evicted_page = random.choice(self.page_queue)
            self.page_queue.remove(evicted_page)
            print(f"Random page to evict: ID: {evicted_page.page_id}")
            # Move the new page from virtual memory to real memory
            self.move_to_real_memory(new_page, evicted_page)
            # Remove the page from virtual memory
            self.remove_from_virtual_memory(new_page)
            # Move evicted page to virtual memory
            self.move_to_virtual_memory(evicted_page)

    def access_page(self, page):
        return 
    
    def delete_page(self, page):
        return super().delete_page(page)