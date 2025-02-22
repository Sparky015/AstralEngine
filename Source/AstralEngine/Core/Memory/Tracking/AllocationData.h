/**
* @file AllocationData.h
* @author Andrew Fagan
* @date 2/9/25
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Debug/Macros/Error.h"
#include <msgpack.hpp>
#include <thread>

namespace Core {

    enum MemoryRegion : uint8
    {
        RENDERER,
        WINDOW,
        ASSETS,
        ECS,
        DEBUG,
        CORE,
        UNKNOWN,

        MEMORY_REGION_END
    };


    inline const char* MemoryRegionToString(MemoryRegion memoryRegion)
    {
        switch (memoryRegion)
        {
            case RENDERER: return "Renderer";
            case WINDOW: return "Window";
            case ASSETS: return "Assets";
            case ECS: return "ECS";
            case DEBUG: return "Debug";
            case CORE: return "Core";
            case UNKNOWN: return "Unknown Region";
            case MEMORY_REGION_END: return "Enum Memory Region End Marker";
            default: ERROR("Memory region value not valid!");
        }
    }


    enum AllocatorType : uint8
    {
        STACK,
        RING,
        FRAME,
        LINEAR,
        POOL,
        DOUBLE_BUFFERED,

        ALIGNED_ALLOCATOR,
        NEW_OPERATOR,
        MALLOC,

        ALLOCATOR_TYPE_END
    };


    inline const char* AllocatorTypeToString(AllocatorType allocatorType)
    {
        switch (allocatorType)
        {
            case STACK: return "Stack Allocator";
            case RING: return "Ring Allocator";
            case FRAME: return "Frame Allocator";
            case LINEAR: return "Linear Allocator";
            case POOL: return "Pool Allocator";
            case DOUBLE_BUFFERED: return "Double Buffered Allocator";
            case ALIGNED_ALLOCATOR: return "Aligned Allocator";
            case NEW_OPERATOR: return "New Operator";
            case MALLOC: return "Malloc";
            case ALLOCATOR_TYPE_END: return "Enum Memory Region End Marker";
            default: ERROR("Memory region value not valid!");
        }
    }


    struct AllocationData
    {
        void* pointer;
        size_t size;
        MemoryRegion region;
        AllocatorType allocatorType;
        std::thread::id threadID;
    };




}




// Add msgpack adapters for the enums
namespace msgpack {

    MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {

        namespace adaptor {

            // Serialization support for Core::MemoryRegion
            template<>
            struct convert<Core::MemoryRegion>
            {
                msgpack::object const& operator()(msgpack::object const& o, Core::MemoryRegion& v) const
                {
                    v = static_cast<Core::MemoryRegion>(o.as<uint8_t>());
                    return o;
                }
            };

            template<>
            struct pack<Core::MemoryRegion>
            {
                template <typename Stream>
                packer<Stream>& operator()(msgpack::packer<Stream>& o, Core::MemoryRegion const& v) const
                {
                    o.pack(static_cast<uint8_t>(v));
                    return o;
                }
            };

            // Serialization support for Core::AllocatorType
            template<>
            struct convert<Core::AllocatorType>
            {
                msgpack::object const& operator()(msgpack::object const& o, Core::AllocatorType& v) const
                {
                    v = static_cast<Core::AllocatorType>(o.as<uint8_t>());
                    return o;
                }
            };

            template<>
            struct pack<Core::AllocatorType>
            {
                template <typename Stream>
                packer<Stream>& operator()(msgpack::packer<Stream>& o, Core::AllocatorType const& v) const
                        {
                    o.pack(static_cast<uint8_t>(v));
                    return o;
                }
            };

        }

    }

}
