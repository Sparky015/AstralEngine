/**
* @file SceneMetricsImporter.cpp
* @author Andrew Fagan
* @date 2/8/2025
*/

#include "SceneMetricsImporter.h"

#include "Debug/Macros/Loggers.h"
#include <iostream>
#include <msgpack.hpp>

bool Core::SceneMetricsImporter::ImportMemoryProfile(const std::filesystem::path& filePath)
{
    if (!std::filesystem::exists(filePath))
    {
        WARN("SceneMetricsImporter: Invalid filepath given!")
        return false;
    }

    m_File.open(filePath, std::ios::binary);
    if (!m_File.is_open())
    {
        WARN("SceneMetricsImporter: Imported memory profile failed to open!")
        return false;
    }

    constexpr uint8 offset = 30;
    m_File.seekg(-1 * offset, std::ios::end);
    std::array<char, offset> snapshotCountBuffer{};
    m_File.read(snapshotCountBuffer.data(), sizeof(snapshotCountBuffer));

    try
    {
        // Read the number of snapshots (the last thing in the file buffer)
        msgpack::unpacker snapshotUnpacker;

        snapshotUnpacker.reserve_buffer(snapshotCountBuffer.size());
        memcpy(snapshotUnpacker.buffer(), snapshotCountBuffer.data(), snapshotCountBuffer.size());
        snapshotUnpacker.buffer_consumed(snapshotCountBuffer.size());


        // Keep getting the object handles until the last object has been read
        msgpack::object_handle objectHandle;
        msgpack::object_handle finalObjectHandle;
        while(snapshotUnpacker.next(objectHandle))
        {
            // Error will occur if we don't use two object handles like this because when next() returns false,
            // the objectHandle populated will be invalid, so we can't use it after the while loop to init the storage
            finalObjectHandle = std::move(objectHandle);
        }

        size_t snapshotCount = finalObjectHandle.get().as<size_t>();
        m_SceneMetricsStorage.InitializeStorage(snapshotCount);
    }
    catch (std::exception& e)
    {
        WARN("Unexpected error while importing memory profile: " << e.what());
        return false;
    }


    // Get size of file
    // Option 1
    // Read in and unpack the snapshots into the storage
    m_File.seekg(0, std::ios::end); // Get file size
    size_t file_size = m_File.tellg();
    m_File.seekg(0, std::ios::beg);


    // Set up buffer for msgpack to read from
    std::vector<char> buffer(file_size);

    // Place file contents into the buffer
    m_File.read(buffer.data(), file_size);


    // Start feeding msgpack the buffer
    try
    {
        msgpack::unpacker unpacker;

        // Feeds the file buffer to the msgpack buffer
        unpacker.reserve_buffer(buffer.size());
        memcpy(unpacker.buffer(), buffer.data(), buffer.size());
        unpacker.buffer_consumed(buffer.size());

        msgpack::object_handle objectHandle;
        MemoryMetrics snapshot;
        while(unpacker.next(objectHandle))
        {
            try
            {
                objectHandle.get().convert(snapshot );
                m_SceneMetricsStorage.AppendSnapshot(snapshot);
            }
            catch (std::bad_cast&)
            {
                // Ignore the bad cast and move on
                // (At least one bad cast will happen because the last object in the stream will
                //  be the snapshot count)
            }
        }

    }
    catch (std::exception& e)
    {
        WARN("Unexpected error while reading memory profile: " << e.what());
        return false;
    }

    return true;
}
