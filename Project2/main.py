#Shimaa Dar Taha 
#Abeer Hussein 

class Memory:
    def __init__(self, start, end, process="Unused"):
        self.start = start        # Starting address of the block
        self.end = end            # Ending address of the block
        self.process = process    # Name of the process or "Unused"
        self.next = None          # Pointer to the next memory block

head = None             # Head of the linked list representing memory
total_memory = 0        # Total memory size

def create_memory(size):
    """
    Initializes the memory with a single large unused block.

    Args:
        size (int): Total size of memory in bytes.
    """
    global head
    head = Memory(0, size - 1, "Unused")#Make an instance of Memory

def print_stat():
    """
    Prints the current memory status, showing each block's address range and its owner (process or Unused).
    """
    curr = head
    while curr:
        print(f"Addresses [{curr.start}:{curr.end}] {curr.process}")#Print statement 
        curr = curr.next#Keep tracing the list 

def split_block(block, size, pid):
    """
    splits a memory block into two:
    First part is allocated to the process
    Second part remains unused
    Args:
        block (Memory): The block to split.
        size: The size needed by the process.
        pid: Process ID requesting the memory.
    """
    old_end = block.end
    block.end = block.start + size - 1# Resize the current block to the requested size
    block.process = pid # Assign the process ID to the first part (allocated part)

    new_block = Memory(block.end + 1, old_end, "Unused")#Make new instance of Memory
    new_block.next = block.next # Link the new block to  the original list
    block.next = new_block # Link the allocated block to the new unused block

def request_memory_con(pid, size, strategy):
    """
    Allocates memory to a process using the selected strategy
    Args:
        pid:process ID requesting memory
        size:memory requested in bytes
        strategy:Allocation strategy:F(First Fit),B (Best Fit),W(Worst Fit).
    """
    global head
    best = None #best block found based on strategy
    best_size = -1#size of the best block (used for comparison)
    curr = head
 # Search for a suitable block
    while curr:
        if curr.process == "Unused" and (curr.end - curr.start + 1) >= size:
            block_size = curr.end - curr.start + 1
            # Check which block to choose based on the strategy
            if (strategy == "F" and best is None) or \
               (strategy == "B" and (best_size == -1 or block_size < best_size)) or \
               (strategy == "W" and block_size > best_size):
                best = curr
                best_size = block_size
        curr = curr.next  # Move to the next memory block
#if suitable block is found
    if best:
        if (best.end - best.start + 1) == size:
            best.process = pid
        else:
            # if block is bigger then split it
            split_block(best, size, pid)
        print(f"Memory allocated to {pid}")
    else:
         # no suitable block found!
        print(f"Not enough memory for {pid}")

def release_memory_con(pid):
    """
    Releases all memory blocks owned by the specified process
    Args:
        pid:process ID to release memory from
    """
    curr = head
    found = False#Initial found flag (False)
    while curr:
        if curr.process == pid:
            curr.process = "Unused"#Release the block 
            found = True#Make the flage (True)
        curr = curr.next# Move to the next memory block
    if found:
        print(f"Memory released for {pid}")#Release the Block 
    else:
        #if the process not found 
        print(f"Process {pid} not found")

def compact_memory():
    """
    Compacts memory by moving all used blocks to the beginning and merging all free space into one block at the end
    to reduce the fragmentation issue
    """
    global head
    curr = head
    next_free = 0
    new_head = None#head of the new compacted memory list
    last_ptr = None #pointer to the last block in the new list

    while curr:
        if curr.process != "Unused":#move used blocks
            size = curr.end - curr.start + 1#the size of the current block
            new_block = Memory(next_free, next_free + size - 1, curr.process)#create a new compacted block
            if new_head is None:
                new_head = new_block # Set new head if it is the first block
                last_ptr = new_block
            else:
                last_ptr.next = new_block  #put the new block to the list
                last_ptr = new_block  #Update the last pointer
            next_free += size  # Move the next_free pointer
        curr = curr.next#Move to the next Block

# Add one "Unused" at the end for all remaining free space
    if next_free < total_memory:
        unused = Memory(next_free, total_memory - 1, "Unused")
        if new_head is None:
            new_head = unused# if there no used blocks
        else:
            last_ptr.next = unused# link the unused block at the end

    head = new_head# Update the head to point to the new list
    print("Memory compacted!")

    
# shaimaa work 
# frame number and frames size
num_frames = 0
frame_size = 0

# Frame representation: A list containing information about each frame.
frames = []

# Page table: Each process has a table that links the page to the frame.
page_tables = {}



# Initializes the paging system with given number of frames and frame size
def init_paging_system(frames_count, size_per_frame):
    global num_frames, frame_size, frames, page_tables  # Declare global variables to use them outside the function
    num_frames = frames_count  # Store the total number of frames
    frame_size = size_per_frame  # Store the size of each frame
    frames = [None] * num_frames  # Initialize all frames as empty (None)
    page_tables = {}  # Dictionary to store page tables for each process
    print(f"\n Paging system initialized with {num_frames} frames, each of size {frame_size} bytes.\n")

# ===================== Commands =====================


# Handles memory request for a process
def request_memory(process_id, num_pages):
    # Find all free frame indices
    free_indexes = [i for i, frame in enumerate(frames) if frame is None]

    # If not enough free frames are available, show an error and exit
    if len(free_indexes) < num_pages:
        print(f" Error: Not enough free frames to allocate {num_pages} pages to {process_id}.\n")
        return

    # If the process doesn't have a page table yet, create one
    if process_id not in page_tables:
        page_tables[process_id] = {}

    # Allocate a frame for each requested page
    for page_num in range(num_pages):
        frame_index = free_indexes.pop(0)  # Take the first available frame
        frames[frame_index] = (process_id, page_num)  # Mark frame as used by this process and page
        page_tables[process_id][page_num] = frame_index  # Map the page number to the frame index in the page table

    print(f" Allocated {num_pages} pages to process {process_id}.\n")

# Releases all memory pages allocated to a process
def release_memory(process_id):
    # If process is not found, show an error
    if process_id not in page_tables:
        print(f" Error: Process {process_id} not found.\n")
        return

    # Free all frames that belong to this process
    for i, frame in enumerate(frames):
        if frame is not None and frame[0] == process_id:
            frames[i] = None  # Mark frame as free

    # Remove the process's page table
    del page_tables[process_id]
    print(f" Released all pages of process {process_id}.\n")

# Displays the current status of memory (which frames are used or free)
def print_memory_status():
    print("\n Memory Status:")
    for i, frame in enumerate(frames):
        if frame is None:
            print(f"Frame {i}: Free")  # Frame is empty
        else:
            process_id, page_num = frame
            print(f"Frame {i}: {process_id}, Page {page_num}")  # Frame is occupied
    print()

# Translates a virtual address (process ID, page number, offset) to a physical address
def translate_address(process_id, page_num, offset):
    global frame_size  # Access the global frame size variable

    # If process is not found, show an error
    if process_id not in page_tables:
        print(f" Error: Process {process_id} not found.\n")
        return

    # If the page is not found for this process, show an error
    if page_num not in page_tables[process_id]:
        print(f" Error: Page {page_num} not found for process {process_id}.\n")
        return

    # If offset exceeds frame size, it's invalid
    if offset >= frame_size:
        print(f" Error: Offset {offset} exceeds frame size ({frame_size}).\n")
        return

    # Get the frame index and calculate physical address
    frame_index = page_tables[process_id][page_num]
    physical_address = frame_index * frame_size + offset

    # Display the translated physical address
    print(f" Physical address for {process_id}, page {page_num}, offset {offset} = {physical_address}\n")

# ===================== Main Loop =====================

def main():
    while True:
        print("\n Memory Allocation Simulator")
        print("Choose allocation strategy:")
        print("1. Contiguous Allocation")
        print("2. Paging System")
        print("X. Exit")

        choice = input("Enter your choice: ").strip()

        if choice == "1":
            main_contiguous()
        elif choice == "2":
            main_paging()
        elif choice.upper() == "X":
            print("Exiting the program.")
            break
        else:
            print(" Invalid choice. Please enter 1, 2, or X.")

# ------------------------ CONTIGUOUS ------------------------

def main_contiguous():
    global total_memory
    total_memory = int(input("Enter total memory size (bytes): "))
    create_memory(total_memory)

    print("\n Contiguous Allocation Mode")
    print("Available commands:")
    print("  RQ <ProcessID> <Size> <Strategy(F/B/W)>")
    print("  RL <ProcessID>")
    print("  C  -> Compact memory")
    print("  STAT  -> Show memory status")
    print("  X  -> Back to main menu")
    print("------------------------------------------------------------")

    while True:
        command = input("allocate: ").strip()
        if command == "":
            continue

        parts = command.split()

        if parts[0] == "RQ" and len(parts) == 4:
            pid = parts[1]
            size = int(parts[2])
            strategy = parts[3].upper()
            request_memory_con(pid, size, strategy)

        elif parts[0] == "RL" and len(parts) == 2:
            release_memory_con(parts[1])

        elif parts[0] == "C":
            compact_memory()

        elif parts[0] == "STAT":
            print_stat()

        elif parts[0].upper() == "X":
            print("Returning to main menu...\n")
            break

        else:
            print(" Invalid command. Please try again.")

# ------------------------ PAGING ------------------------

def main_paging():
    print("\n Paging System Mode")
    frames_count = int(input("Enter number of frames: "))
    frame_size_input = int(input("Enter size of each frame (bytes): "))
    init_paging_system(frames_count, frame_size_input)

    print("\nAvailable commands:")
    print("  RQ <ProcessID> <NumberOfPages>")
    print("  RL <ProcessID>")
    print("  TR <ProcessID> <PageNumber> <Offset>")
    print("  STAT  -> Show memory status")
    print("  X     -> Back to main menu")
    print("------------------------------------------------------------")

    while True:
        command = input(">> ").strip()
        if command == "":
            continue

        parts = command.split()

        if parts[0] == "RQ" and len(parts) == 3:
            request_memory(parts[1], int(parts[2]))

        elif parts[0] == "RL" and len(parts) == 2:
            release_memory(parts[1])

        elif parts[0] == "TR" and len(parts) == 4:
            translate_address(parts[1], int(parts[2]), int(parts[3]))

        elif parts[0] == "STAT":
            print_memory_status()

        elif parts[0].upper() == "X":
            print(" Returning to main menu...\n")
            break

        else:
            print(" Invalid command. Please try again.")

# --------------------------------------------------------

if __name__ == "__main__":
    main()





