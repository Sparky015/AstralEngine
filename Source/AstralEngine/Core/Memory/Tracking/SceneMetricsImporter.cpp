/**
* @file SceneMetricsImporter.cpp
* @author Andrew Fagan
* @date 2/8/2025
*/

#include "SceneMetricsImporter.h"

#include "Debug/Macros/Loggers.h"
#include <iostream>
#include <msgpack.hpp>

#include "Debug/Instrumentation/ScopeProfiler.h"

Core::SceneMetricsImporter::SceneMetricsImporter() : m_HasFileLoaded(false)
{

}

bool Core::SceneMetricsImporter::ImportMemoryProfile(const std::filesystem::path& filePath)
{
    PROFILE_SCOPE("MemorySceneMetricsImporter")
    if (!std::filesystem::exists(filePath))
    {
        WARN("SceneMetricsImporter: Invalid filepath given!")
        m_HasFileLoaded = false;
        return false;
    }

    m_File.open(filePath, std::ios::in | std::ios::binary);
    if (!m_File.is_open())
    {
        WARN("SceneMetricsImporter: Imported memory profile failed to open!")
        m_HasFileLoaded = false;
        return false;
    }

    constexpr uint8 offset = 30; // TODO: Take the file size into account here
    m_File.seekg(-1 * offset, std::ios::end);
    std::array<char, offset> snapshotCountBuffer{};
    m_File.read(snapshotCountBuffer.data(), sizeof(snapshotCountBuffer));

    try
    {
        PROFILE_SCOPE("Read Snapshots Count")
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
        m_HasFileLoaded = false;
        return false;
    }

    m_File.seekg(0, std::ios::beg);


    // Get size of file
    // Option 1
    // Read in and unpack the snapshots into the storage
    size_t file_size = std::filesystem::file_size(filePath);
    // m_File.seekg(0, std::ios::end); // Get file size
    // size_t file_size = m_File.tellg();
    // m_File.seekg(0, std::ios::beg);


    // Set up buffer for msgpack to read from
    std::vector<char> buffer(file_size);

    // Place file contents into the buffer
    {
        PROFILE_SCOPE("Reading file contents into buffer")
        m_File.read(buffer.data(), file_size);
    }


    // Start feeding msgpack the buffer
    try
    {
        PROFILE_SCOPE("Parsing file buffer and appending snapshot data")
        msgpack::unpacker unpacker;

        // Feeds the file buffer to the msgpack buffer
        unpacker.reserve_buffer(buffer.size());
        memcpy(unpacker.buffer(), buffer.data(), buffer.size());
        unpacker.buffer_consumed(buffer.size());

        msgpack::object_handle objectHandle;
        MemoryMetrics snapshot;
        for (size_t i = 0; i < m_SceneMetricsStorage.GetExpectedSnapshotCount(); i++)
        {
            unpacker.next(objectHandle);

            try
            {
                {
                    PROFILE_SCOPE("Object Handle Conversion to Snapshot")
                    // LOG(objectHandle.get());
                    snapshot = objectHandle.get().as<MemoryMetrics>();
                }
                {
                    PROFILE_SCOPE("Appending snapshot to Storage")
                    m_SceneMetricsStorage.AppendSnapshot(snapshot);
                }
            }
            catch (std::bad_cast&)
            {
                WARN("SceneMetricsImporter: Object Handle failed cast to MemoryMetrics snapshot!")
            }
        }

    }
    catch (std::exception& e)
    {
        WARN("Unexpected error while reading memory profile: " << e.what());
        m_HasFileLoaded = false;
        return false;
    }

    m_HasFileLoaded = true;
    return true;
}
