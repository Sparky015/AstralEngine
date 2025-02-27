//
// Created by Andrew Fagan on 11/1/24.
//

#pragma once

#include "MemoryMetrics.h"

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