/**
* @file TextureLoader.cpp
* @author Andrew Fagan
* @date 4/13/25
*/

#include "TextureLoader.h"

#include "Core/SmartPointers.h"
#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Resources/Texture.h"

#include "stb_image/stb_image.h"
#include "stb_image_write.h"

#include <numbers>
#include <cmath>

#include "Core/Containers/Bitmap.h"

namespace Astral {

    Ref<Asset> TextureLoader::LoadAsset(const std::filesystem::path& filePath)
    {
        PROFILE_SCOPE("TextureLoader::LoadAsset")

        Ref<Texture> texture;

        if (filePath.extension() == ".hdr")
        {
            int width;
            int height;
            int bpp;

            stbi_set_flip_vertically_on_load(true);
            float* equirectangularData = stbi_loadf(filePath.string().c_str(), &width, &height, &bpp, 4);
            ImageFormat imageFormat = ImageFormat::R32G32B32A32_SFLOAT;


            // Convert the equirectangular image to a cubemap
            Bitmap equirectangularImage = Bitmap(equirectangularData, imageFormat, width, height);
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

            texture = Texture::CreateCubemap(cubemapData, faceSideLength, faceSideLength, ImageFormat::R16G16B16A16_SFLOAT);


            stbi_image_free(equirectangularData);
            delete cubemapData;
        }
        else
        {
            texture = Texture::CreateTexture(filePath.string());
        }

        if (!texture) { return nullptr; } // Return nullptr if an error occurred

        RendererAPI::NameObject(texture, filePath.filename().string().data());
        return texture;
    }




}
