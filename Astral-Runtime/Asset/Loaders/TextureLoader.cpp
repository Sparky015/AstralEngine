/**
* @file TextureLoader.cpp
* @author Andrew Fagan
* @date 4/13/25
*/

#include "TextureLoader.h"

#include "Core/Containers/Bitmap.h"
#include "Core/SmartPointers.h"
#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Resources/Texture.h"

#include "stb_image/stb_image.h"
#include "stb_image_write.h"

#include <numbers>
#include <cmath>


namespace Astral {

    Ref<Asset> TextureLoader::LoadAsset(const std::filesystem::path& filePath)
    {
        PROFILE_SCOPE("TextureLoader::LoadAsset")

        Ref<Texture> texture;

        if (filePath.extension() == ".hdr") // Load cubemap
        {
            int width;
            int height;
            int bpp;

            stbi_set_flip_vertically_on_load(true);
            float* equirectangularData = stbi_loadf(filePath.string().c_str(), &width, &height, &bpp, 4);
            ImageFormat fileImageFormat = ImageFormat::R32G32B32A32_SFLOAT;


            // Convert the equirectangular image to a cubemap
            Bitmap equirectangularImage = Bitmap(equirectangularData, fileImageFormat, width, height);
            std::vector<Bitmap> cubemap;

            ConvertEquirectangularToCubemap(equirectangularImage, cubemap);

            uint32 totalCubemapSize = cubemap[0].GetTotalBitmapSize() * 6;
            uint8* cubemapData = new uint8[totalCubemapSize];

            uint32 totalBytesWritten = 0;
            for (const Bitmap& face : cubemap)
            {
                memcpy(cubemapData + totalBytesWritten, face.GetData(), face.GetTotalBitmapSize());
                totalBytesWritten += face.GetTotalBitmapSize();
            }

            uint32 faceSideLength = cubemap[0].GetPixelWidth();

            // For debugging or viewing face images
            // std::string stem = filePath.parent_path().string() + "/" + filePath.stem().string();
            //
            // for (size_t i = 0; i < cubemap.size(); i++)
            // {
            //     std::string outFilePath = stem + "_" + std::to_string(i) + ".hdr";
            //     stbi_write_hdr(outFilePath.c_str(), faceSideLength, faceSideLength, 4, (float*)cubemap[i].GetData());
            // }

            texture = Texture::CreateCubemap(cubemapData, faceSideLength, faceSideLength, ImageFormat::R16G16B16A16_SFLOAT, 0);


            stbi_image_free(equirectangularData);
            delete[] cubemapData;
        }
        else if (filePath.extension() == ".cube") // Load LUT (Look Up Table)
        {
            int size = 0;
            std::vector<Vec4> lut;

            std::ifstream file(filePath);
            if (!file.is_open()) { return nullptr; }

            std::string line;
            while (std::getline(file, line))
            {
                if (line.empty() || line[0] == '#') continue;

                std::istringstream iss(line);
                std::string key;
                iss >> key;

                if (key == "LUT_3D_SIZE")
                {
                    iss >> size;
                    lut.reserve(size * size * size);
                }
                else if (isdigit(key[0]) || key[0] == '-' || key[0] == '.') // If the line starts with a number (like -0.4, or 1.2 or .2)
                {
                    iss.clear(); // Clears any error state
                    iss.str(line);

                    float r, g, b;
                    iss >> r >> g >> b;
                    lut.push_back({r, g, b, 1.0f});
                }
            }

            if (lut.size() != size * size * size) { return nullptr; }

            texture = Texture::CreateLUT(lut.data(), size, size, ImageFormat::R16G16B16A16_SFLOAT);
            return texture;
        }
        else // Load regular 2D texture
        {
            texture = Texture::CreateTexture(filePath.string());
        }

        if (!texture) { return nullptr; } // Return nullptr if an error occurred

        RendererAPI::NameObject(texture, filePath.filename().string().data());
        return texture;
    }




}
