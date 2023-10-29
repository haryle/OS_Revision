## What are the goals of virtualising memory

- Transparency 
- Efficiency 
- Protection

## What is transparency in memory virtualisation

The implementation details of virtual memory should be invisible to the running program.

## What is efficiency in memory virtualisation 

- Time: run quickly 
- Spacec - don't use too much memory 

## What is protection in memory virtualisation

- Each process can only access addresses in its address space
- Run in isolation 

## What is address translation

The hardware transforms each memory access - i.e fetch/load/store instruction, changing virtual address to a physical address where the desired data is located. 

## What is the difference between uni programming and multi-programming and why is multi-programming prefered w.r.t memory virtualisation.

For uni programming, only one process is loaded to memory, and operation is performed on the entire memory space. 

For multi-programs, multiple processes are loaded to memory and share the memory space by having different isolated address spaces. 

Benefits: higher resource utilisation, more efficient. 

## What is the motivation for dynamic memory allocation 

- Sometime we do not know the required memory size at compile time.
- If we can only allocate statically, we must allocate as much as possibly needed -> storage is not used efficiently. 

## What is stack allocation and its benefits 

Simple to allocate and deallocate -> just increment/decrement the stack pointer.
No fragmentation 

## What are the advantages and disadvantages of heap allocation 

- Heap works for all data structure 
- Slow, and usually lead to fragmentation (small chunks of free space between allocated memory blocks). 
- Need to search for free space

## How can timesharing be implemented for memory virtualisation and why is it not preferred.

- Save CPU registers to memory when a process is not running. 
- Disadvantage is poor performance. 

## What is static relocation

The OS rewrites each program before loading it as process in memory. Rewrite addresses and pointers. 

## What are the disadvantages os static relocaton

- No protection as processes can access the address of other processes.
- Cannot move address space after it has been placed

## What is dynamic relocation:

- Protect processes from one another 
- USes hardware support with memory management unit (MMU).
- MMU dynamically changes process address at every memory reference:
    - Process generate logical or virtual address in their address space
    - Memory hardware uses physical or real address 

## What are the hardware support for dynamic relocation
- Kernel mode - handle traps and access physical memory. Handle memory violation exceptions 
- User mode - translate address 
= Base - fixed offset to add to address 
- Bound - either maximum size or maximum address. If access is beyond bound -> Segmentation Fault

## Can users directly change base and bound registers?

No 

## What are the advantages of Base and Bound 
- Provides protection to address space 
- Support dynamic relocation
- Simple, fast 

## Disadvantages of Base and Bound:
- Process must be allocated contiguously in physical memory 
- No partial sharing: cannot share limited parts of address space 
- Must allocate memory that may not be used by other processes 
- Hard to run when address space does not fit into the free space of physical memory 

## What are the basic principles of segmentation

The address space for each process is divided into logical segments. Each segment corresponds to logical entities like code, stack, heap. The segments can be placed independently in physical memory, and grow and shrink by themselves. Each segment can also be protected with separate R/W/E protection bits. 