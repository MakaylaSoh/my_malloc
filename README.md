Project : My Malloc

Purpose:  Developed a my_malloc() and my_free() function to emulate the malloc and free functions in the stdlib.h header in C. The system calls sbrk() and brk() are used to allocate and free memory. 

MY MALLOC FUNCTION

Function definition: void* my_malloc(int size)

The my_malloc() function accepts one argument representing the size the user wants. Often operating systems allocate memory in chunks called pagesizes as a way to manage virtual memory. Thus, my_malloc() function uses the size argument to calculate the appropriate pagesize needed per user. This funtion uses 4096 bytes as one pagesize, which is a typical pagesize amount. 

Each chunk of allocated memory for a program is tracked as a node in a linked-list. These chunks have a "chunk header", or information held by the linked list that indicate whether the node is used, the size used, and the next/previous nodes. A used node will be refered to as occupied, and an unused node will be referred to as unoccupied. To allocate more memory, my_malloc() first traverse through the linked list searching for any unused nodes. When a node is freed in the middle of the linked list, it is considered an unused, or free, node that exists within the linked list. See the "free function" description below.

If there is an unused node in the linked-list that has enough space for the memory needed, that node is taken. This means that node is now marked as occupied. If the node is significantly larger than the amount of memory needed, the node is split into two nodes - the additional node holds the remaining unocuppied, free memory. This is done in the typical way of adding a new node into a linked list. Only if there are no unoccupied nodes that fit the needed size, then sbrk() will be called with the correct calculated size to allocate more memory. The address where the user can access the allocated memory is returned as a void pointer. If there is no more memory, null is returned.

FREE FUNCTION

Function definition: void* my_free(BYTE* p)

The my_free() function accepts a pointer to an address where memory was previously allocated to. For this implementation, this would be a place in the linked list described in the "my malloc" section above. This address may be in the middle of two occupied nodes. If this is the case, then that node is simply marked as unoccupied and can be used by future my_malloc() calls. If the address provided is a node that is in the middle of two nodes, but one or both nodes are unoccupied, then all the unoccupied nodes should be merged into one big unoccupied node. This is done by updating one of the node's size to reflect the sum of the size of all the nodes, and then removing the other nodes. Removing a node is done in the typical way of removing a element from a linked list.

If the provided pointer indicates the last node in the linked list, then brk() is used to move the program break to exclude the unoccupied node. The program break indicates the total memory in the heap accessable to a specific process. If the last node is unused, then the program break can be moved up. This cannot be done with nodes freed in the middle of the list.

Note: In the implementation, each node is the structure chunkhead. The node/chunkhead is marked "unoccupied" or "occupied" based off the info field of the chunkhead structure.
