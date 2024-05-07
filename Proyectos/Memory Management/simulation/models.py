# app/models.py
from mmu import MemoryManagementUnit as MMU

class Computer:
    def __init__(self, num_cores=1, ips=1, disk_access_time=5, ram_size=400*1024, page_size=4096):
        self.num_cores = num_cores
        self.ips = ips # instructions per second
        self.disk_access_time = disk_access_time
        self.ram_size = ram_size
        self.page_size = page_size
        self.mmu = MMU(ram_size, page_size)
        self.processes = {}
        self.next_pid = 1

    def create_process(self):
        pid = self.next_pid
        self.next_pid += 1
        self.processes[pid] = Process(pid)


class Process:
    def __init__(self, pid):
        self.pid = pid
        self.instructions = []
    
    def add_instruction(self, instruction):
        self.instructions.append(instruction)

    
