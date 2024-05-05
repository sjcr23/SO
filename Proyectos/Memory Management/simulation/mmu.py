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

    def execute_instruction(self, instruction):
        tokens = instruction.split('(')
        command = tokens[0]
        args = tokens[1].rstrip(')').split(',')
        if command == 'new':
            # Extract pid and size from the instruction string
            pid = int(args[0])
            size = int(args[1])
            print(f"Instruction new -> Pid [{pid}], size [{size}]")
            self.allocate_memory(pid, size)
        
    def allocate_memory(self, pid, size):
        num_pages = size // self.page_size
        if size % self.page_size != 0:
            num_pages += 1

        pages = []
        for _ in range(num_pages):
            page = self.allocate_page()
            print("Page allocated: ", page.page_id, f" in real memory: {page.in_real_memory}")
            pages.append(page)

        self.memory_map[pid] = pages

    def allocate_page(self):
        for i, page in enumerate(self.real_memory):
            if page is None:
                self.real_memory[i] = Page(page_id=i, in_real_memory=True, physical_address=i)
                return self.real_memory[i]
            
        # If no free page is available in real memory, allocate in virtual memory
        virtual_page = Page(page_id=len(self.virtual_memory), in_real_memory=False)
        self.virtual_memory.append(virtual_page)
        return virtual_page
# test
mmu = MemoryManagementUnit(400*1024, 4096)
print("real memory ", len(mmu.real_memory))
mmu.execute_instruction('new(1, 24576)')
print("Memory map for pid 1: ", mmu.memory_map[1])