Memory Pool

Fixed sized memory block pool.

Motivation:
Fixed-size blocks allocation, also called memory pool allocation, uses a free
list of fixed-size blocks of memory. Used in embedded systems and high performance
systems (audio and video processing) where fixed sized memory blocks needed
to be allocated to significantly reduced overhead and substantially improve
performance for objects that need frequent allocation / de-allocation.

Uses:
Audio Video processing (double buffering)
High performance networking (network window size)

Performance:
O(1) : using free list (stack) O(1) performance can be obtained

Considerations:
Primary objective is to improve memory allocation/management for applications
with a fixed sized memory requirement. Random and dynamic memory allocation
needs of the majority of applications are not suitable for this memory strategy.
