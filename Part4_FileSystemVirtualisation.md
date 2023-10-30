## Why is there a need for hierarchical IO structure? 

- Costs and performance
- Faster bus needs to be short (no room to plug devices)
- Components that demand high performance are nearer to the CPU
- Lower performance components are put in peripheral bus.

![](Figures/Figure36.1.png)

## What are the components of a canonical device 

- Hardware interface which allows the software to control its operations. (Status, Command, Data)
- Internal structure - implementation of the hardware interface. 

![](Figures/Figure36.3.png)

## Describe the four steps canonical protocol 

- Polling: the OS checks whether the device status is ready 
- Programmed IO: the OS sends data to data register
- The OS writes command to command register to be executed
- Poll for exit status 

## What is Programmed IO 

When the OS is directly involved with data movement 

## What is the problem with four step canonical protocol

A lot of polling which is wasteful. No work is done during the polling period 

## How does interrupt improve the canonical protocol?

- Instead of repeatedly poll the device, the OS issue the request, put the calling process to sleep and context switch to another task. 
- When the device operation is complete, it will raise a hardware interrupt.
- OS then executes an interrupt handler, finishing the request and waking up the sleeping process 
- Thus both the CPU and the disk are utilised during this process

## What could be the problem with interrupt in canonical protocol?

- If the poll period is quick, interrupting can be expensive due to context switch 
- Potential livelock in networks, when there are a huge stream of incoming packets, each generate an interrupt. The OS thus spend its time processing interrupt and never run any user-level process.

## How to address livelock network interrupt?

- Use polling 
- Coalescing - wait a bit and gather all the interrupts to be processed at once. This however, increases latency 

## What is the issue with Programmed IO when transferring a large chunk of data to a device?

The CPU is burden with trivial task and waste clock cycles

## What is DAMA and how is DAMA superior to Programmed IO 

- A DAMA is a special device that allows data to transfer between devices and main memory without much CPU intervention

- CPU does not need to waste CPU cycles, instead only needs to instruct DAMA which data to transfer and where to send to 

## What are the two different methods of Device Interraction 

- Explicit IO instructions - instructions that allow OS to send data to specific device registers (privillege instruction)

- Memory mapped IO. The hardware make device registers available as if they are memory locations. 

## What is memory mapped IO?

The hardware makes device registers appear as if they are memory locations. To access register, OS writes data to the memory locations.

## Can IO instructions be executed in user mode? 

No in privillege mode. Other processes could use a loophole to control the machine 

## What is a device driver?

A driver provides a software interface to hardware devices, enabling the OS to access hardware functions without needing to know about the implementation details. 

## What are the downsides of device driver encapsulations?

Some features that do not fit into the abstraction interface will not get used. 

## Descibe the address space of a drive

A drive consists of N blocks (512 bytes) array. Address space is from 0 to N - 1

## Is multi-sector/multi-block operations supported for a drive?

Yes. Read can be done in parallel. Write is usually on one block with atomic operation. 

## Describe the geometry of a disk

![](Figures/DiskGeometry.png)

## Can there be multiple platters on a disk?

Yes

## What is a disk cylinder 

Aggregate of the same track number on every platter used for recording 

## What is rotational delay

If the head is on the right track, the head will need to wait for the platter to rotate to the right sector. The waiting time is rotational delay. 

## What is seek time

If the head does not reside on the right track, it needs to move to the right track. This moving time is seek time. 

## What is transfer time

Time required to perform IO once the head resides on the right track at the right sector 

## What is track skew 

The rearrangement of sectors on a disk so that by the time the computer has read and processed one sector, the next will be in the right position for the disk to read (cross track read). 

## Why is track skew useful

The arrangement is useful because without the rearrangement, once the disk head has read from one track and moved to the other track with the desired sectors, it will need to wait for rotation, incurring rotational delay. 

## What is a multi-zoned disk 

The surface is divided into a number of concentric zones. Within a zone, the number of bits per track is constant. Each zone has the same number of sectors per track. Outter zones have more sectors than inner zone. 

## What is a track buffer 

Cache for storing data read from or to disk. 

## Formula for finding IO time 

`Tio = Tseek + Trotation + Ttransfer`

## Formula for finding rate of IO 

`Rio = FileSize/Tio`

## Assume the following Disk Drive Specs, find rate of IO for a random read of 4KB and sequential read of 100MB

![](Figures/Figure37.5.png)

- Tseek: 4ms
- Trotation: 1/(15000/60)/2 = 2 ms (average is half of rotation time)

For random read: transfer time = 4KB/125MB = 31 micro seconds 

Hence Tio for random read: 6ms. 
Rio for random read: 4KB/6ms = 0.65 MB/s

For sequential read, time take is seek time + a rotation + transfer:

4ms + 2ms + 100MB/125MB/s = 0.806s

Hence Rate of IO: 100MB/0.806 = 124.1 MB/s (close to max transfer rate)

## Why is there a need for disk scheduling? 

Because IO cost is high, the OS plays a role in deciding the order of IO issed to disk. i.e given a set of IO requests, the OS decides which one to schedule next. 

## What is the difference between process scheduling and disk scheduling 

IO scheduling is more predictable by estimating the seek and rotational delay -> Generally shortest job first 

## Describe the principle of shortest seek time first 

Order the IO requests based on nearest track to complete. 

## What are the issues with Shortest Seek Time First

- Disk geometry information not available to the OS - the OS only sees an array of blocks
- Lead to starvation - if there is a steady stream of requests for the current track, IO requests for other tracks will be ignored. 

## Describe the mechanism of the Elevator scheduling

The arm moves in and out to the edges of the disk. Requests that are currently on the direction of movements are handled. 

## Describe the elevator variant FSCAN 
The queue to be service is freezed during movement. Requests that come during a sweep are placed in a queue. This avoids starvation of far-away requests

## Describe the elevator varient CSCAN

Sweep is only in one direction, from outter to inner, and then reset at outter.

## Why is CSCAN fairer than FSCAN 

CSCAN is fairer to the sectors at outter tracks. In SCAN, middle tracks can be served twice.

## What is the issue with SCAN-family algorithms?

Doesn't take into account rotation. 

## Describe Shortest Positioning Time First

Schedule IO request that takes the shortest time to move the head to, taking into account rotation. 

## Why is it difficult to implement Shortest Positioning Time First

Need to know both the track geometry and the rotational speed. Thus SPTF is usually implemented inside a drive 

## Where is disk scheduling performed in modern OS system

In both the OS and the disk. The OS picks the best few requests and sends them to be handled by the disk 

## What is IO merging in the context of disk scheduling 

Wait for IO request to aggregate before handling them. This reduces the number of requests sent to the disk and hence lower overheads. 

## What is the difference between work-conserving and non-work-conserving scheduling approach

Work-conserving - immediate handle request as it arrives
Non-work-conserving - wait for some time before handling requests. 