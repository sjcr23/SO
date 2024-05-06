# app/mmu.py
from algorithms import FIFO, SecondChance
class Page:
    def __init__(self, page_id, in_real_memory, physical_address=None, timestamp=None, referenced=False):
        self.page_id = page_id # Unique identifier for the page
        self.in_real_memory = in_real_memory # Flag indicating whether the page is in real memory
        self.physical_address = physical_address
        self.timestamp = timestamp # Timestamp indicating when the page was allocated
        self.referenced = referenced # Flag indicating whether the page has been referenced recently
        # add more attributes as needed

class MemoryManagementUnit:
    def __init__(self, ram_size, page_size, algorithm):
        self.ram_size = ram_size
        self.page_size = page_size
        self.real_memory = [None] * (ram_size // page_size)
        self.virtual_memory = []
        self.memory_map = {} # Memory map: ptr -> pages
        self.pointer_map = {} # PID -> ptr
        self.pointer_counter = 1
        self.page_counter = 1
        self.algorithm = None
        if algorithm == 'FIFO':
            self.algorithm = FIFO(ram_size, page_size, self.real_memory, self.virtual_memory)
        elif algorithm == 'SC':
            self.algorithm = SecondChance(ram_size, page_size, self.real_memory, self.virtual_memory)
        else:
            raise ValueError("Invalid algorithm specified")

    def execute_instruction(self, instruction):
        tokens = instruction.split('(')
        command = tokens[0]
        args = tokens[1].rstrip(')').split(',')

        if command == 'new':
            # Extract pid and size from the instruction string
            pid = int(args[0])
            size = int(args[1])
            print(f"Instruction new -> Pid [{pid}], size [{size}]")
            ptr = self.allocate_memory(pid, size)
            print(f"Memory allocated for PID[{pid}] -> ptr:{ptr}")
        
        elif command == 'use':
            ptr = int(args[0])
            print(f"Instruction use -> ptr [{ptr}]")
            self.access_memory(ptr)

    def allocate_memory(self, pid, size):
        # Calculate the number of pages to assign
        num_pages = size // self.page_size
        if size % self.page_size != 0:
            num_pages += 1

        pages = []
        for _ in range(num_pages):
            page = self.allocate_page()
            print("Page allocated: ", page.page_id, f" in real memory: {page.in_real_memory}")
            pages.append(page)

        # Generate and assign a pointer to the process
        ptr = self.generate_pointer()
        self.pointer_map[ptr] = pid
        self.memory_map[ptr] = pages        
        return ptr
    
    def generate_pointer(self):
        ptr = self.pointer_counter
        self.pointer_counter += 1
        return ptr

    def allocate_page(self):
        for i, page in enumerate(self.real_memory):
            if page is None:
                return self.add_page_to_memory(i)
            
        # If no free page is available in real memory, allocate in virtual memory
        return self.add_page_to_virtual_memory()
    
    def add_page_to_memory(self, index):
        page = Page(page_id=self.page_counter, in_real_memory=True, physical_address=index, referenced=True)
        self.page_counter += 1
        self.real_memory[index] = page
        self.algorithm.update_queue(page)
        return page

    def add_page_to_virtual_memory(self):
        page = Page(page_id=self.page_counter, in_real_memory=False)
        self.page_counter += 1
        self.virtual_memory.append(page)
        return page
    
    def access_memory(self, ptr):
        if ptr in self.memory_map:
            # Check if the pages are in real memory
            pages = self.memory_map[ptr]
            for page in pages:
                if not page.in_real_memory:
                    # Bring pages into real memory
                    self.bring_into_real_memory(page)
                else:
                    page.referenced = True
                    index = self.algorithm.page_queue.index(page)
                    self.algorithm.page_queue[index].referenced = True
            print(f"Memory accessed at ptr [{ptr}]")
        else:
            print("Error: Pointer not found in memory map")

    def bring_into_real_memory(self, page):
        if None in self.memory_map:
            # Find an empty slot in real memory
            index = self.real_memory.index(None)
            page.in_real_memory = True
            page.physical_address = index
            self.real_memory[index] = page
            self.algorithm.update_queue(page)
            print(f"Page {page.page_id} was brought back to real memory")

            # Remove the page from virtual memory
            self.virtual_memory.remove(page)
            print(f"Page {page.page_id} was removed from virtual memory")
        else:
            # Evict a page using the corresponding algorithm
            self.algorithm.evict_page(page)

    def delete_memory(self, pid):
        if pid in self.pointer_map:
            ptr = self.pointer_map.pop(pid)
            if ptr in self.memory_map:
                pages = self.memory_map[ptr]
                for page in pages:
                    if page.in_real_memory:
                        self.real_memory[page.physical_address] = None
                    else:
                        self.virtual_memory.remove(page)
                del self.memory_map[ptr]

# test
mmu = MemoryManagementUnit(400*1024, 4096, 'SC')
print("real memory ", len(mmu.real_memory))
for i in range(len(mmu.real_memory)):
    mmu.execute_instruction(f"new({i+1}, {4096*5})")

mmu.execute_instruction('new(101, 4096)')

print("Real memory before SC", [page.page_id for page in mmu.real_memory])
print("Virtual memory before SC", [page.page_id for page in mmu.virtual_memory])
for key in mmu.pointer_map:
    mmu.execute_instruction(f'use({mmu.pointer_map[key]})')
print("Real memory after SC", [page.page_id for page in mmu.real_memory])
print("Virtual memory after SC", [page.page_id for page in mmu.virtual_memory])
