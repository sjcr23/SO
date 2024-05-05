# app/models.py

class Computer:
    def __init__(self, num_cores, ips, disk_access_time, ram_size, page_size) -> None:
        self.num_cores = num_cores
        self.ips = ips
        self.disk_access_time = disk_access_time
        self.ram_size = ram_size
        self.page_size = page_size


class Process:
    def __init__(self, pid) -> None:
        self.pid = pid
        self.instructions = []
    
    def add_instruction(self, instruction) -> None:
        self.instructions.append(instruction)

    
class Page:
    def __init__(self) -> None:
        self.id = None
        self.address = None
        self.in_real_memory = False
        # add more attributes as needed