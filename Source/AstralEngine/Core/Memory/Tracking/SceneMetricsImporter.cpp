/**
* @file SceneMetricsImporter.cpp
* @author Andrew Fagan
* @date 2/8/2025
*/

#include "SceneMetricsImporter.h"

#include <msgpack.hpp>
#include <iostream>

#include "Debug/Macros/Loggers.h"

bool Core::SceneMetricsImporter::ImportMemoryProfile(const std::filesystem::path& filePath)
{
    if (!std::filesystem::exists(filePath))
    {
        LOG("Invalid filepath given")
        return false;
    }
    m_File.open(filePath, std::ios::binary);
    if (!m_File.is_open())
    {
        LOG("Imported memory profile failed to open")
        return false;
    }

    // Read in and unpack the snapshots into the storage
    m_File.seekg(0, std::ios::end); // Get file size
    size_t file_size = m_File.tellg();
    m_File.seekg(0, std::ios::beg);

    std::vector<char> buffer(file_size);
    if (!m_File.read(buffer.data(), file_size))
    {
//        std::cerr << "Error: Could not read file '" << filename << "'." << std::endl;
        return false;
    }

    try
    {
        // deserializes these objects using msgpack::unpacker.
        msgpack::unpacker pac;

        // feeds the buffer.
        pac.reserve_buffer(buffer.size());
        memcpy(pac.buffer(), buffer.data(), buffer.size());
        pac.buffer_consumed(buffer.size());

        // now starts streaming deserialization.
        msgpack::object_handle oh;
        while(pac.next(oh))
        {
            std::cout << oh.get() << "\n\n";
            // m_SceneMetricsStorage.AppendSnapshot();
        }

    }
    catch (const msgpack::unpack_error& e)
    {
        std::cerr << "Error: Msgpack unpack error: " << e.what() << std::endl;
        return false;
    }
    catch (const std::exception& e)
    {
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
        return false;
    }

    return false;
}
