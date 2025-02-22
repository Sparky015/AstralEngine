//
// Created by Andrew Fagan on 11/1/24.
//

#pragma once

#include "Core/Memory/Tracking/MemoryMetrics.h"

#define TRACK_ALLOCATION(size) Core::MemoryMetricsManager::Get().TrackAllocation(size)
#define TRACK_DEALLOCATION(size) Core::MemoryMetricsManager::Get().TrackDeallocation(size)

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


// For malloc/free override
extern "C"
{
    void* malloc(size_t size);
    void free(void* ptr);
    void* calloc(size_t num, size_t size);
    void* realloc(void* ptr, size_t new_size);
}


