# Operating-Systems-6233
----

# Disk Scheduling
This lab project addresses the implementation of IO-scheduling algorithms in an operating system.

Each IO request in an operating system is managed by using a data structure called the Request Control Block (RCB). An RCB contains the request ID, arrival timestamp, cylinder, address, and the ID of the process that posted the request. The RCB struct is defined as follows and is accessible from the rest of the code in this lab project by including the header file oslabs.h in your source code files:

    struct RCB {
        int request_id;
        int arrival_timestamp;
        int cylinder;
        int address;
        int process_id;
    }

The set of IO requests in the operating system that are to be processed are tracked in an IO Request Queue. This data structure is an array of RCBs of the requests.


The NULLRCB is defined as [RID:0, AT:0, CYL:0, ADDR:0, PID:0]

To determine the schedule of servicing the IO requests in an operating system, we consider three policies:


First-Come-First-Served Scheduling (FCFS)
Shortest-Seek-Time-First Scheduling (SSTF)
LOOK Scheduling (LOOK)

In order to implement the above policies, we need to develop methods that handle the arrival of IO requests and the completion of IO requests. That is, when a new IO request arrives, we need to figure out whether to service it immediately or to store it in the IO Request Queue. Whenever an IO request is completed, we need to figure out the next request from the IO Request Queue that needs to be serviced.
----

# CPU Scheduling
This lab project addresses the implementation of CPU-scheduling algorithms in an operating system.

Each process in an operating system is managed by using a data structure called the Process Control Block (PCB). A PCB contains the process ID, arrival timestamp, total burst time, execution start time, execution end time, remaining burst time and the priority of the process in the system. The PCB class is defined as follows and is accessible from the rest of the code in this lab project:

    struct PCB {
        int process_id;
        int arrival_timestamp;
        int total_bursttime;
        int execution_starttime;
        int execution_endtime;
        int remaining_bursttime;
        int process_priority;
    }

The set of processes in the operating system that are ready to execute are maintained in a data structure called the Ready Queue. This data structure is an array of PCBs of the processes that are waiting for the CPU to become available so that they can execute.


The NULLPCB is defined as [PID:0, AT:0, TBT:0, EST:0, EET:0, RBT:0, Priority:0]



To determine the schedule of execution for the processes in an operating system, we consider three policies:


Priority-based Preemptive Scheduling (PP)
Shortest-Remaining-Time-Next Preemptive Scheduling (SRTP)
Round-Robin Scheduling (RR)


In order to implement the above policies, we need to develop methods that handle arrival of processes for execution and the completion of process execution. Whenever a process arrives for execution, it can either join the ready queue and wait for its chance to execute or execute immediately (if there is no other process currently executing or if the currently-running process can be preempted). Whenever a process completes execution, another process from the ready queue is chosen to execute next, based on the scheduling policy.
----

# Memory Allocation
This lab project addresses the various memory-management schemes in an operating system.

We model the memory managed in an operating system as a list of memory blocks. Each block of memory is modeled by a data structure of MEMORY_BLOCK defined below:

    struct MEMORY_BLOCK {
        int start_address;
        int end_address;
        int segment_size;
        int process_id; //0 indicates a free block
    }

Thus, the operating system code can set up memory blocks for the available memory and keep track of which blocks are allocated to which processes. In particular, note that for each memory block, the system tracks the starting and ending addresses, along with the size of the block and the process to which the block is currently allocated. For free blocks, the process is indicated as 0. 

The system manages the memory as an array of memory blocks, stored in an array. As memory is allocated and released, the list of blocks in the memory map changes.

In this lab project, we address the various ways in which memory can be allocated (based on different schemes - Best Fit, First Fit, Next Fit, Worst (Largest) Fit). In addition, we also develop a method to manage the release of memory blocks by processes.


For the purposes of these labs we define the NULLBLOCK as [start_address: 0, end_address: 0, segment_size: 0, process_id: 0]
----

# Virtual Memory Allocation
This lab project addresses the implementation of page-replacement algorithms in a demand-paging system.

Each process in a demand-paging system has a page table that contains a list of entries. For each logical page of the process, there is an entry in the table that indicates if the page is in memory. If the page is in memory, the memory frame number that page is resident in is indicated. Also, for each page, the time at which the page has arrived in memory, the time at which it has been last referenced, and the number of times the page has been referenced since the page arrived in memory are maintained.

The page table data structure is a simple array of page-table entries (PTEs). Each PTE contains five fields as defined below:
   struct PTE {
        int is_valid;
        int frame_number;
        int arrival_timestamp;
        int last_access_timestamp;
        int reference_count;
    }


Each process in the system has a page table that is simply an array of PTEs. Each process also has a pool of frames that is allocated. The frame pool for a process is represented as an array of integers, where each Integer represents the frame number of a frame that is free and is available for use by the process. Note that in order to get the frame number in the pool, you first need to access the integer in the array. 

This lab project aims to exercise the various policies for page replacement. In particular, we study the following three page-replacement policies:
First-In-First-Out (FIFO)
Least-Recently-Used (LRU)
Least-Frequently-Used (LFU)

In order to implement the above policies, we need to develop corresponding functions that process page accesses for a process. That is, for each process, given its page table, a logical page number being referenced and the free frame pool of the process, the functions should determine the memory frame number for the logical page. Also, the functions should modify the page table and the free frame pool appropriately.
