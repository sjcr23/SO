# app/utils.py

import random


def generate_instructions_sequence(P, N):
    instructions = []
    for pid in range(1, P+1):
        process_instructions = []
        for _ in range(random.randint(1, N // P)):
            # Generate random instructions for each process
            instruction = random.choice(["new", "use", "delete", "kill"])
            if instruction == "new":
                size = random.randint(1, 8192)
                process_instructions.append(f"new({pid}, {size})")
            elif instruction == "use":
                ptr = random.randint(1, 100) # suppose there are 100 pointers
                process_instructions.append(f"use({ptr})")
            elif instruction == "delete":
                ptr = random.randint(1, 100)
                process_instructions.append(f"delete({ptr})")
            elif instruction == "kill":
                process_instructions.append(f"kill({pid})")
        instructions.extend(process_instructions)
    return instructions



def get_algorithms(data):
    OPT = data[0]['NAME']
    ALG = OPT
    if len(data) > 1:    
        ALG = data[1]['NAME']
    return OPT, ALG


def get_ptr(mem):
    return [x.ptr for x in mem if x!=None]

def get_pid(mem):
    return [x.page_id for x in mem if x!=None]

def get_pages(mem_map):
    r = {}
    for key in mem_map.keys():
        r[key] = get_pid(mem_map[key])
    
    return r

def excecute_file(mmu):
    global current_color
    mmu_state = {'real_memory':[], 'memory_map':[], 'pointer_map':[]}
    
    # Simulate PC file read
    for i in range(len(mmu.real_memory)):
        mmu.execute_instruction(f"new({i+1}, {4096*5})")
        mmu_state['memory_map'].append(get_pages(mmu.memory_map))
        mmu_state['real_memory'].append(get_ptr(mmu.real_memory))
        mmu_state['pointer_map'].append(mmu.pointer_map)
    
    return mmu_state

