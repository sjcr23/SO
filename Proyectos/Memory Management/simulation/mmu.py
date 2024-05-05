# app/mmu.py
class Page:
    def __init__(self, page_id, in_real_memory, physical_address=None):
        self.page_id = page_id # Unique identifier for the page
        self.in_real_memory = in_real_memory # Flag indicating whether the page is in real memory
        self.physical_address = physical_address
        # add more attributes as needed

class MemoryManagementUnit:
    def __init__(self, ram_size, page_size):
        self.ram_size = ram_size
        self.page_size = page_size
        self.real_memory = [None] * (ram_size // page_size)
        self.virtual_memory = []
        self.memory_map = {} # Memory map: ptr -> pages
        self.pointer_map = {} # PID -> ptr
        self.pointer_counter = 1

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
        
    def allocate_memory(self, pid, size):
        # Generate and assign a pointer to the process
        ptr = self.generate_pointer()
        self.pointer_map[ptr] = pid

        # Calculate the number of pages to assign
        num_pages = size // self.page_size
        if size % self.page_size != 0:
            num_pages += 1

        pages = []
        for _ in range(num_pages):
            page = self.allocate_page()
            print("Page allocated: ", page.page_id, f" in real memory: {page.in_real_memory}")
            pages.append(page)

        self.memory_map[ptr] = pages
        return ptr
    
    def generate_pointer(self):
        ptr = self.pointer_counter
        self.pointer_counter += 1
        return ptr

    def allocate_page(self):
        for i, page in enumerate(self.real_memory):
            if page is None:
                self.real_memory[i] = Page(page_id=i, in_real_memory=True, physical_address=i)
                return self.real_memory[i]
            
        # If no free page is available in real memory, allocate in virtual memory
        virtual_page = Page(page_id=len(self.virtual_memory), in_real_memory=False)
        self.virtual_memory.append(virtual_page)
        return virtual_page
    
    def delete_memory(self, pid):
        if pid in self.memory_map:
            pages = self.memory_map[pid]
            for page in pages:
                if page.in_real_memory:
                    self.real_memory[page.physical_address] = None
                else:
                    self.virtual_memory.remove(page)
            del self.memory_map[pid]

# test
mmu = MemoryManagementUnit(400*1024, 4096)
print("real memory ", len(mmu.real_memory))
mmu.execute_instruction('new(1, 24576)')
print("Memory map for pid 1: ", mmu.memory_map[mmu.pointer_map[1]])
mmu.delete_memory(1)
if 1 in mmu.memory_map:
    print("Memory map for pid 1: ", mmu.memory_map[mmu.pointer_map[1]])
else:
    print("There is not memory associated to the PID 1")

mmu.execute_instruction('new(2, 4096)')
mmu.execute_instruction('new(3, 4096)')
mmu.execute_instruction('new(4, 4096)')
mmu.execute_instruction('new(1, 24576)')