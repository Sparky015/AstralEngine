//
// Created by Andrew Fagan on 11/1/24.
//

#pragma once

/**@brief Redefines the new operator to track memory usages and allocations
 * @throws bad_alloc if std::malloc returns nullptr */
void* operator new(std::size_t size);

/**@brief Redefines the new operator to track memory usages and allocations
* @throws bad_alloc if std::malloc returns nullptr */
void* operator new[](std::size_t size);

/**@brief Redefines the delete operator to track memory usages and frees */
void operator delete(void* pointer) noexcept;

/**@brief Redefines the delete operator to track memory usages and frees */
void operator delete[](void* pointer) noexcept;



// TODO: Write MemoryScopeProfiler that operators like the timer scope profiler
//      on construction, save the current total memory allocated and total freed memory and total number of allocations.
//      Then on destruction, mark the same things and measure the difference in data points.
//      This will show how much memory was allocated and freed and how many allocations occurred.

