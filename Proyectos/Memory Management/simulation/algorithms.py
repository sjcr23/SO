from collections import deque, defaultdict
import random

class PageReplacementAlgorithm:
    def __init__(self, ram_size, page_size, real_memory, virtual_memory, memory_map):
        self.ram_size = ram_size
        self.page_size = page_size
        self.real_memory = real_memory
        self.virtual_memory = virtual_memory
        self.memory_map = memory_map
        self.page_queue = deque()

    def update_queue(self, new_page, ptr):
        if len(self.page_queue) >= self.ram_size // self.page_size:
            self.evict_page(new_page, ptr)
        else:
            self.page_queue.append(new_page)

    def evict_page(self, new_page, ptr):
        raise NotImplementedError("Subclasses must implement evict_page method")
    
    def move_to_real_memory(self, new_page, evicted_page):
        index = self.real_memory.index(evicted_page)
        new_page.in_real_memory = True
        new_page.physical_address = index
        new_page.virtual_address = None
        new_page.referenced = False
        self.real_memory[index] = new_page
        self.page_queue.append(new_page)
        print(f"Page: {new_page.page_id} moved to real memory")

    def move_to_virtual_memory(self, page):
        virtual_address = self.virtual_memory.index(None)
        page.in_real_memory = False
        page.physical_address = None
        page.virtual_address = virtual_address
        self.virtual_memory[virtual_address] = page
        print(f"Evicted page moved to virtual memory at position {page.virtual_address}")

    def remove_from_virtual_memory(self, page):
        # Remove the page from virtual memory
        self.virtual_memory[page.virtual_address] = None
        print(f"Page {page.page_id} was removed from virtual memory")

    def access_page(self, page):
        raise NotImplementedError("Subclasses must implement access_page method")

    def delete_page(self, page):
        self.page_queue.remove(page)


class FIFO(PageReplacementAlgorithm):
    def evict_page(self, new_page, ptr):
        if self.page_queue and self.real_memory:
            evicted_page = self.page_queue.popleft()
            print(f"Page to evict: ID: {evicted_page.page_id}")
            # Remove the page from virtual memory
            self.remove_from_virtual_memory(new_page)
            # Move the new page from virtual memory to real memory
            self.move_to_real_memory(new_page, evicted_page)
            # Move evicted page to virtual memory
            self.move_to_virtual_memory(evicted_page)

    def access_page(self, page):
        return 
    
    def delete_page(self, page):
        return super().delete_page(page)
    

class SecondChance(PageReplacementAlgorithm):
    def evict_page(self, new_page, ptr):
        while True:
            pageInRealMemory = self.page_queue.popleft()
            if not pageInRealMemory.referenced:
                print(f"Page {pageInRealMemory.page_id} is going to be evicted from real memory")
                # if the page is not referenced, it can be evicted
                # Remove the page from virtual memory
                self.remove_from_virtual_memory(new_page)
                # Move the new page from virtual memory to real memory
                self.move_to_real_memory(new_page, pageInRealMemory)
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
    def evict_page(self, new_page, ptr):
        ptr_related_pages = []
        if ptr in self.memory_map:
            ptr_related_pages = self.memory_map[ptr]

        if self.page_queue and self.real_memory:
            # Find the most recently used page in the queue (last element)
            # that is not one of the pages related to the current ptr request
            evicted_page = None
            pages_to_append = []
            while True:
                evicted_page = self.page_queue.pop()
                if evicted_page in ptr_related_pages:
                    pages_to_append.append(evicted_page)
                else:
                    break

            if evicted_page is None:
                evicted_page = self.page_queue.pop()

            for pages_tmp in pages_to_append:
                self.page_queue.append(pages_tmp)

            print(f"Page to evict: ID: {evicted_page.page_id}")
            # Remove the page from virtual memory
            self.remove_from_virtual_memory(new_page)
            # Move the new page from virtual memory to real memory
            self.move_to_real_memory(new_page, evicted_page)
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
    def evict_page(self, new_page, ptr):
        if self.page_queue and self.real_memory:
            # Select a random page from the queue
            evicted_page = random.choice(self.page_queue)
            self.page_queue.remove(evicted_page)
            print(f"Random page to evict: ID: {evicted_page.page_id}")
            # Remove the page from virtual memory
            self.remove_from_virtual_memory(new_page)
            # Move the new page from virtual memory to real memory
            self.move_to_real_memory(new_page, evicted_page)
            # Move evicted page to virtual memory
            self.move_to_virtual_memory(evicted_page)

    def access_page(self, page):
        return 
    
    def delete_page(self, page):
        return super().delete_page(page)
    
class OPT(PageReplacementAlgorithm):
    def evict_page(self, new_page, ptr):
        if self.page_queue and self.real_memory:
            page_to_evict = self._select_page_to_evict(ptr)
            self.page_queue.remove(page_to_evict)
            print(f"Page to evict: ID: {page_to_evict.page_id}")
            self.remove_from_virtual_memory(new_page)
            self.move_to_real_memory(new_page, page_to_evict)
            self.move_to_virtual_memory(page_to_evict)

    def access_page(self, page):
        return 
    
    def delete_page(self, page):
        return super().delete_page(page)


    def _select_page_to_evict(self, ptr):
        pages_related_to_ptr = []
        if ptr in self.memory_map:
            pages_related_to_ptr = self.memory_map[ptr]

        # Evict page with furthest predicted future access index
        page_to_evict = None
        for page in self.page_queue:
            if page.future_accesses:
                for ptr in reversed(page.future_accesses):
                    if ptr in self.memory_map:
                        ptr_pages = self.memory_map[ptr]
                        for page in reversed(ptr_pages):
                            if page.in_real_memory and page not in pages_related_to_ptr:
                                page_to_evict = page
                                break
                    if page_to_evict is not None:
                        break
            if page_to_evict is not None:
                break
        print(f"Page selected to evict: Page [{page_to_evict.page_id}], in real memory: {page_to_evict.in_real_memory}")

        return page_to_evict