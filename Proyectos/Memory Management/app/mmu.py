# app/mmu.py
from .models import Page

class MemoryManagementUnit:
    def __init__(self, ram_size, page_size) -> None:
        self.ram_size = ram_size
        self.page_size = page_size
        self.real_memory = [Page() for _ in range(ram_size // page_size)]
        self.virtual_memory = []

    def execute_instruction(self, instruction) -> None:
        #TODO: implement logic to execute instruction
        pass
