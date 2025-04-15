//
// Created by Andrew Fagan on 11/30/24.
//

#include "Debug/MemoryTracking/MallocFreeOverrides.h"

#define STBI_MALLOC(size) Astral::TrackingMalloc(size, Core::ASSETS)
#define STBI_REALLOC(ptr, size) Astral::TrackingRealloc(ptr, size, Core::ASSETS)
#define STBI_FREE(ptr) Astral::TrackingFree(ptr)
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"