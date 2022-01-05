# Simple-Cache-Simluation

My implementation of a simplified cache that simulates the steps of data storage of memory addresses that occurs in a regular cache. 

The program reads a trace file of memory accesses and simulates the behavior of a regular cache according to that trace with and without prefetching. It take 5
variable arguments: cache size (a power of 2), type of associativity ("direct", "assoc:n" [n is a power of 2], or "assoc"), FIFO replacement policy, block size
(a power of 2), and the trace file.

The program uses these arguments to construct two simluated caches with one performing prefetching and the other nonprefetching. It reads memory access from the
trace file given and performs the behavior of the specific cache given. Afterwards, it will print the number of cache hits, cache misses, memory reads, and memory
writes for both the prefetched and nonprefetched cache.
