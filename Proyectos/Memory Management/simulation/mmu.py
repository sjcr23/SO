# app/mmu.py

class MemoryManagementUnit:
    def __init__(self, ram_size, page_size) -> None:
        self.ram_size = ram_size
        self.page_size = page_size
        self.real_memory = [None] * (ram_size // page_size)
        self.virtual_memory = []

    def execute_instruction(self, instruction) -> None:
        #TODO: implement logic to execute instruction
        pass


# test
mmu = MemoryManagementUnit(400*1024, 4096)
print("real memory ", len(mmu.real_memory))