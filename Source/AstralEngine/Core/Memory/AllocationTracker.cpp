//
// Created by Andrew Fagan on 1/7/25.
//

#include "AllocationTracker.h"

#include <cstdlib> // for std::malloc and std::free
#include <iostream>

#include "MemoryMetricsManager.h"


// void operator delete[](void* pointer, std::size_t size) noexcept
// {
//     if (pointer) { // Check for null
//         Core::MemoryMetricsManager::Get().Free(size);
//         std::free(pointer);
//     }
// }
//
// void operator delete(void* ptr, std::size_t size, void* placement_ptr) noexcept
// {
//     std::cout << "Custom placement delete called, size: " << size << ", ptr: " << ptr << ", placement_ptr: " << placement_ptr << std::endl;
// }





// void operator delete(void* pointer) noexcept
// {
//     if (pointer) { // Check for null
//         std::cout << "Freeing memory!\n";
//         // Core::MemoryMetricsManager::Get().Free(size);
//         std::free(pointer);
//     }
// }