# Astral Allocators

----

A collection of allocators to help performance in the engine.


### Brief Descriptions of Allocators

---

#### Linear Allocator
This allocator efficiently allocates memory out, but does not allow individual deallocations. You can call
the Reset method to deallocate all previous allocations at once.

#### Stack-Based Linear Allocator
Behaves the same as the Linear Allocator except memory is allocated from the stack instead of the heap.
This allows for more flexible memory management for stack memory than what would otherwise be available.
This allocates stack memory by creating a fixed-sized memory buffer using template arguments. I avoided
alloca because this does not work with all compilers and platforms.

#### Stack Allocator
This allocator efficiently allocates memory out and does allow individual deallocations. However, you can 
only deallocate the immediate previous allocation. You can also set a marker that "marks" the spot. Then once
you make more allocations after that point. You can roll back the memory allocations until that marker, freeing 
all the allocations that came after the marker.

#### Frame Allocator
This is like the linear allocation, but additionally supports markers from the Stack Allocator. It was made 
with allocations that would only last for a frame in mind.

#### Double Buffered Allocator
This allocator is simply two frame allocators put together, allowing for you to write to one frame allocator
while having a second frame allocator in a back buffer. This allows you to save the previous frame's data
for one frame.

#### Pool Allocator
This allocator efficiently allocates fixed-size blocks of memory at a time. The size of the blocks and
the number of blocks that can be stored in the allocator's arena can be specified through the template 
arguments. You can also check out the object pool class I built too it overlaps a little bit with this if
you need something more than this.

#### Aligned Allocator
This allocator allows the user to allocate a memory block with any alignment requirement. All the other
Astral Allocators use this allocator to allocate their memory arenas with the max alignment. I was having 
issues with allocators like std::aligned_alloc on some platforms, so I decided to make my own.

#### Ring Allocator
This allocator allocates memory that with the idea that they are short-lived, temporary
allocations. This allows efficient allocations that don't need to be deallocated as it will be
overwritten as more allocations are made. Some good applications of this are streaming data with 
the ring allocator and processing that data. The ring allocator is basically a ring buffer with 
an allocator interface. The allocator will track the top of the memory arena (the most recent allocation).
Once, the allocator reaches the end of the memory arena, it will move back to the bottom of the arena and start
overwriting previous allocations. This is why the allocations must be temporary and short-lived if you use this
allocator. Providing the allocator with a bigger arena size will allow the allocations to live a little long depending
on how many allocations and the size of the allocations you make. There is no way to check if the data has
been overridden, so I suggest that you treat this as a temporary buffer instead of a location to store data in.

### Usage

---

#### Basic API Usage
```c++
LinearAllocator allocator = LinearAllocator(512); // Create an instance with a 512 byte arena
int* array = allocator.allocate(sizeof(int) * 5, alignof(int)); // Allocate memory for an int array with 5 elements
size_t usedSize = allocator.GetUsedBlockSize(); // Returns the amount of memory that has been allocated so far
allocator.Reset() // Deallocates everything from the allocator instance
``` 

#### Resize Allocator 
- Note that the allocator has to be empty or use the Reset method to make it empty for it to be
resized
```c++
LinearAllocator allocator = LinearAllocator(512); // Create an instance with a 512 byte arena
allocator.Reset() // Deallocates everything from the allocator instance
allocator.ResizeBuffer() // Double the size of the arena
```

#### STL Allocator Version Basic Usage

```c++
STLLinearAllocator<int> allocator = STLLinearAllocator(512);
int* array = allocator.allocate(5); // Allocates 5 elements of type int
size_t usedSize = allocator.getUsedBlockSize(); // Returns the amount of memory that has been allocated so far
allocator.reset() // Deallocates everything from the allocator instance
allocator.resizeBuffer() // Double the size of the arena
```

#### STL Allocator Passing to STL Container
```c++
template <typename T>
using CustomVector = std::vector<T, STLLinearAllocator<T>>; 

STLLinearAllocator<int> allocator = STLLinearAllocator<int>(200);
CustomVector<int> intVector{allocator};
intVector.reserve(3); // Use as normal
```

###
### Design Process and Considerations

---

#### Why I Made a Collection of Custom Allocators

The Problem: When I added a basic memory tracker, I discovered that everytime I logged a message using my logging system, it would
make ~5 allocations. This was completely unnecessary as allocations can be slow, especially if I need to log tons of 
messages every frame. I began to look into these allocations and I found that these allocations were coming from my 
usage of std::stringstream. My logging system allows the user to pass in anything that can be inserted in a stringstream. This is very helpful for debugging or logging information
as any numbers or anything that is not a string can still be logged and recorded. At the same time, std::stringstream
can cause a lot of allocations as it processes and allocates data in chunks. However, I still wanted to keep the ability
to log streams of data as it can be very helpful. So this led me to finding a solution that could remove these allocations,
while keeping the std::stringstreams for logging data that isn't a string. At the same time, I also happened to be reading
the book Game Engine Architecture which mentioned a bunch of different memory allocators and what they are for and how to 
make them, so that influenced me to make a linear/arena allocator that I can pass to std::stringstream which will prevent it from
causing so many memory allocations from the heap. Then, I just kept adding different allocators just because I wanted to.
I was reading about it in the book and then I wanted to implement it, so I did. Plus, it gives me a bunch of allocators
I can use in the future to help keep my engine performant. 


#### Allocator Versions: STL vs Non-STL

Every allocator that I wrote has a C-style API as I think it is more flexible for the user, but in order to work with 
standard library containers, I wrote an STL version of each allocator that implements the required format by the STL.
The STL versions are simply a wrapper around the regular allocator version. Another thing to note is that the STL versions
have a copy constructor and assignment operator, but the STL version allocator does not actually support creating 
copies of instances. None of the allocators I wrote support copying instances. The STL versions have copy constructors 
and operators in order to support passing allocator instances without the STL container creating its own copy and 
making it harder for the user to access and make function calls to the allocator instance.


#### ASAN Integration

Since I found ASAN very useful for helping me detect and fix my memory bugs, I decided to integrate ASAN into the 
Astral allocators. This way, there is still detection for any memory bugs that could come up and prevents a blind
spot for bugs that might be very hard to notice without ASAN. If ASAN is available, the memory arena for each allocator
will be poisoned and will become poisoned as you allocate memory from the allocator.

#### Error Handling Strategy

Whenever the allocators run into an error or have ran out of memory to allocate, they can do two
different things depending on their version. If the allocator is a Non-STL, regular version allocator,
it will return nullptr whenever any errors happen. However, if the allocator is a STL version allocator,
it will throw std::bad_alloc. This is to maintain compatibility with the STL containers.

#### Thread Safety

I decided to not add any thread safety mechanisms into the allocators to prevent performance impacts as some people
could only be on one thread or otherwise don't need the thread safety, but would still pay for it in additional performance 
hits (whether it be small or big). Therefore, if the user needs the thread safety, they should implement a mechanism for
thread safety when using the allocators.

#### Debugging

On debugging builds, I have asserts enabled that provide checks to ensure that any errors are caught. Additionally, ASAN
checks are made to help catch any bugs. Beyond this, I have memory tracking system integrated with these allocators to 
provide usage metrics and other information that can be used to debug and improve performance. You can learn more
about the memory tracking system in the Astral Engine [here](link here).

#### Fragmentation

Many of the allocators allocate in a linear fashion as commonly you can only deallocate the previous allocation,
use markers to rollback, or completely reset the arena. As a result, there is little to no fragmentation in most
of the allocators. Some exceptions are the aligned allocator (uses malloc underneath)
and the pool allocator (can deallocate without restrictions).

###
#### References Used:

---


- Game Engine Architecture (3rd Edition)
- CppCon 2015: Scott Wardle “Memory and C++ debugging at Electronic Arts”
- CppCon 2017: John Lakos “Local ('Arena') Memory Allocators
- CppCon 2017: Bob Steagall “How to Write a Custom Allocator” 