# app/mmu.py
from algorithms import FIFO, SecondChance, MRU, RND
class Page:
    def __init__(self, page_id, pid, ptr, in_real_memory,  physical_address=None, timestamp=None, referenced=False):
        self.pid = pid
        self.ptr = ptr
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
        elif algorithm == 'MRU':
            self.algorithm = MRU(ram_size, page_size, self.real_memory, self.virtual_memory)
        elif algorithm == 'RND':
            self.algorithm = RND(ram_size, page_size, self.real_memory, self.virtual_memory)
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
            print(f"\nInstruction new -> Pid [{pid}], size [{size}]")
            ptr = self.allocate_memory(pid, size)
            print(f"\nMemory allocated for PID[{pid}] -> ptr:{ptr}")
        
        elif command == 'use':
            ptr = int(args[0])
            print(f"\nInstruction use -> ptr [{ptr}]")
            self.access_memory(ptr)

        elif command == 'delete':
            ptr = int(args[0])
            print(f"\nInstruction delete -> ptr [{ptr}]")
            self.delete_ptr(ptr)
        
        elif command == 'kill':
            pid = int(args[0])
            print(f"\nInstruction kill -> PID [{pid}]")
            self.delete_memory(pid)

        else:
            print(f"\nCommand [{command}] not recognized. Only available commands are:\n\t- new(pid, size) -> ptr\n\t- use(ptr) -> void\n\t- delete(ptr) -> void\n\t- kill(pid) -> void")

    def allocate_memory(self, pid, size):
        # Calculate the number of pages to assign
        num_pages = size // self.page_size
        if size % self.page_size != 0:
            num_pages += 1
        
        # Generate and assign a pointer to the process
        ptr = self.generate_pointer()

        pages = []
        for _ in range(num_pages):
            page = self.allocate_page(pid, ptr)
            print("Page allocated: ", page.page_id, f" in real memory: {page.in_real_memory}")
            pages.append(page)

        self.pointer_map[ptr] = pid
        self.memory_map[ptr] = pages        
        return ptr
    
    def generate_pointer(self):
        ptr = self.pointer_counter
        self.pointer_counter += 1
        return ptr

    def allocate_page(self, pid, ptr):
        for i, page in enumerate(self.real_memory):
            if page is None:
                return self.add_page_to_memory(pid, ptr, i)
            
        # If no free page is available in real memory, allocate in virtual memory
        return self.add_page_to_virtual_memory(pid, ptr)
    
    def add_page_to_memory(self, pid, ptr, physical_address):
        page = Page(self.page_counter, pid, ptr, in_real_memory=True, physical_address=physical_address, referenced=True)
        self.page_counter += 1
        self.real_memory[physical_address] = page
        self.algorithm.update_queue(page)
        return page

    def add_page_to_virtual_memory(self, pid, ptr):
        page = Page(self.page_counter, pid, ptr, in_real_memory=False)
        self.page_counter += 1
        self.virtual_memory.append(page)
        return page
    
    def access_memory(self, ptr):
        print(f"\nAccessing memory associated to ptr [{ptr}]")
        if ptr in self.memory_map:
            # Check if the pages are in real memory
            pages = self.memory_map[ptr]
            print(f"Pages associated to ptr [{ptr}]: ", [page.page_id for page in pages])
            for page in pages:
                if not page.in_real_memory:
                    # Bring pages into real memory
                    self.bring_into_real_memory(page)
                else:
                    page.referenced = True
                    self.algorithm.access_page(page)
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

    def delete_ptr(self, ptr):
        # We first delete the pages related to the ptr
        self.delete_pages(ptr)
        
        # Finally, we delete the ptr associated with a PID
        keys_to_delete = []
        if self.pointer_map:
            for key, value in self.pointer_map.items():
                if value == ptr:
                    keys_to_delete.append(key)
        for key in keys_to_delete:
            print(f"Deleting ptr[{ptr}] related to PID [{key}] from pointer map")
            del self.pointer_map[key]

    def delete_memory(self, pid):
        if pid in self.pointer_map:
            print(f"Deleting memory for PID [{pid}]")
            ptr = self.pointer_map.pop(pid)
            print(f"Deleting memory associated to PID [{pid}] with ptr [{ptr}]")
            self.delete_pages(ptr)
        else:
            print(f"PID [{pid}] does not exist")

    def delete_pages(self, ptr):
        if ptr in self.memory_map:
            print(f"Freeing memory allocated for pages associated with ptr[{ptr}]")
            pages = self.memory_map[ptr]
            print(f"Pages associated to ptr [{ptr}]: ", [page.page_id for page in pages])
            for page in pages:
                if page.in_real_memory:
                    self.real_memory[page.physical_address] = None
                    # update queue for the current algorithm
                    self.algorithm.delete_page(page)
                else:
                    self.virtual_memory.remove(page)
            print(f"All pages associated to ptr [{ptr}] were removed from memory")
            # then proceed to delete the ptr in the memory map
            print(f"Deleting ptr[{ptr}] from memory map")
            del self.memory_map[ptr]
        else:
            print(f"Pointer [{ptr}] does not exist in memory")
            return

# test
alg = 'RND'
mmu = MemoryManagementUnit(400*1024, 4096, alg)
print("real memory ", len(mmu.real_memory))
for i in range(len(mmu.real_memory)):
    mmu.execute_instruction(f"new({i+1}, {4096*5})")

mmu.execute_instruction('new(101, 4096)')

print(f"Real memory before {alg}", [page.page_id if page is not None else None for page in mmu.real_memory])
print(f"Virtual memory before {alg}", [page.page_id for page in mmu.virtual_memory])
for key in mmu.pointer_map:
    mmu.execute_instruction(f'use({mmu.pointer_map[key]})')
print(f"Real memory after {alg}", [page.page_id if page is not None else None for page in mmu.real_memory])
print(f"Virtual memory after {alg}", [page.page_id for page in mmu.virtual_memory])
print(f"Queue for {alg} algorithm before delete instruction: ", [page.page_id for page in mmu.algorithm.page_queue])
print("\n")
mmu.execute_instruction('delete(100)')
print(f"Queue for {alg} algorithm after delete instruction: ", [page.page_id for page in mmu.algorithm.page_queue])
mmu.execute_instruction('use(100)')
mmu.execute_instruction('kill(1)')
print("Real memory after kill instruction: ", [page.page_id if page is not None else None for page in mmu.real_memory])
print("Algorithm queue state: ", [page.page_id for page in mmu.algorithm.page_queue])
mmu.execute_instruction('use(1)')
mmu.execute_instruction('use(55)')