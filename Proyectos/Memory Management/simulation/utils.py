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
