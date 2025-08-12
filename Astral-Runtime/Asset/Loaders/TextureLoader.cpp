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
            unsigned char* equirectangularData = stbi_load(filePath.string().c_str(), &width, &height, &bpp, 4);
            ImageFormat imageFormat = ImageFormat::R8G8B8A8_UNORM;


            // Convert the equirectangular image to a cubemap
            details::Bitmap equirectangularImage = details::Bitmap(equirectangularData, imageFormat, width, height);
            std::vector<details::Bitmap> cubemap;

            details::ConvertEquirectangularToCubemap(equirectangularImage, cubemap);

            uint32 totalCubemapSize = cubemap[0].GetTotalBitmapSize() * 6;
            uint8* cubemapData = new uint8[totalCubemapSize];

            uint32 totalBytesWritten = 0;
            for (const details::Bitmap& face : cubemap)
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
            //     std::string outFilePath = stem + "_" + std::to_string(i) + ".jpg";
            //     stbi_write_jpg(outFilePath.c_str(), faceSideLength, faceSideLength, 4, cubemap[i].GetData(), 100);
            // }

            texture = Texture::CreateCubemap(cubemapData, faceSideLength, faceSideLength, imageFormat);


            stbi_image_free(equirectangularData);
            delete cubemapData;
        }
        else if (filePath.extension() == ".cube") // Load LUT (Look Up Table)
        {
            int size = 0;
            std::vector<Vec3> lut;

            std::ifstream file(filePath);
            if (!file.is_open()) { return nullptr; }

            std::string line;
            while (std::getline(file, line)) {
                if (line.empty() || line[0] == '#') continue;

                std::istringstream iss(line);
                std::string key;
                iss >> key;

                if (key == "LUT_3D_SIZE")
                {
                    iss >> size;
                    lut.reserve(size * size * size);
                }
                else if (isdigit(key[0]) || key[0] == '-' || key[0] == '.')
                {
                    // This is a LUT entry
                    iss.clear();
                    iss.str(line);
                    float r, g, b;
                    if (iss >> r >> g >> b)
                        lut.push_back({r, g, b});
                }
            }

            if (lut.size() != size * size * size) { return nullptr; }

            texture = Texture::CreateLUT(lut.data(), size, size, ImageFormat::R8G8B8_UNORM);
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


    TextureLoader::details::Bitmap::Bitmap(void* data, ImageFormat imageFormat, uint32 width, uint32 height) :
        m_MapData(),
        m_Width(width),
        m_Height(height),
        m_ImageFormat(imageFormat),
        m_BytesPerTexel(GetBytesPerTexel(imageFormat))
    {
        uint32 totalNumBytes = width * height * m_BytesPerTexel;
        m_MapData.resize(totalNumBytes);

        if (data)
        {
            memcpy(m_MapData.data(), data, m_MapData.size());
        }
    }


    UVec4 TextureLoader::details::Bitmap::GetPixel(uint32 x, uint32 y) const
    {
        if (x >= m_Width)
        {
            WARN("Trying to get pixel data with out of bounds x value: " << x << "(Image width is " << m_Width << ")")
            x = m_Width - 1;
        }
        if (y >= m_Height)
        {
            WARN("Trying to get pixel data with out of bounds y value: " << y << "(Image height is " << m_Height << ")")
            y = m_Height - 1;
        }

        uint32 yOffset = y * m_Width;
        uint32 componentOffset = (yOffset + x) * m_BytesPerTexel; // Considering the component size in the offset for the correct index
        uint32 pixelIndex = componentOffset;
        UVec4 component{0};
        component.x = m_MapData[pixelIndex];
        if (m_BytesPerTexel > 1) { component.y = m_MapData[pixelIndex + 1]; }
        if (m_BytesPerTexel > 2) { component.z = m_MapData[pixelIndex + 2]; }
        if (m_BytesPerTexel > 3) { component.w = m_MapData[pixelIndex + 3]; }
        return component;
    }


    void TextureLoader::details::Bitmap::SetPixel(uint32 x, uint32 y, const UVec4& pixelData)
    {
        if (x >= m_Width)
        {
            WARN("Trying to get pixel data with out of bounds x value: " << x << "(Image width is " << m_Width << ")")
            x = m_Width - 1;
        }
        if (y >= m_Height)
        {
            WARN("Trying to get pixel data with out of bounds y value: " << y << "(Image height is " << m_Height << ")")
            y = m_Height - 1;
        }

        uint32 yOffset = y * m_Width;
        uint32 componentOffset = (yOffset + x) * m_BytesPerTexel; // Considering the component size in the offset for the correct index
        uint32 pixelIndex = componentOffset;

        m_MapData[pixelIndex] = pixelData.x;
        if (m_BytesPerTexel > 1) { m_MapData[pixelIndex + 1] = pixelData.y; }
        if (m_BytesPerTexel > 2) { m_MapData[pixelIndex + 2] = pixelData.z; }
        if (m_BytesPerTexel > 3) { m_MapData[pixelIndex + 3] = pixelData.w; }
    }


    void TextureLoader::details::ConvertEquirectangularToCubemap(const Bitmap& equirectangularImageData, std::vector<Bitmap>& outCubemap)
    {
        uint32 faceSideLength = equirectangularImageData.GetPixelWidth() / 4;

        const uint32 numberOfFaces = 6;
        outCubemap.reserve(numberOfFaces);

        for (int i = 0; i < numberOfFaces; i++)
        {
            outCubemap.emplace_back(nullptr, ImageFormat::R8G8B8A8_UNORM, faceSideLength, faceSideLength);
        }

        uint32 maxPixelWidthIndex = equirectangularImageData.GetPixelWidth() - 1;
        uint32 maxPixelHeightIndex = equirectangularImageData.GetPixelHeight() - 1;

        for (int faceIndex = 0; faceIndex < numberOfFaces; faceIndex++)
        {
            for (int x = 0; x < faceSideLength; x++)
            {
                for (int y = 0; y < faceSideLength; y++)
                {
                    // Convert pixel coordinates to cartesian
                    Vec3 cartesianCoords = ConvertFaceCoordinatesToCartesian(x, y, faceIndex, faceSideLength);

                    // Convert cartesian to spherical
                    float rho = sqrtf(cartesianCoords.x * cartesianCoords.x + cartesianCoords.y * cartesianCoords.y);
                    float phi = atan2f(cartesianCoords.y, cartesianCoords.x);
                    float theta = atan2f(cartesianCoords.z, rho);

                    // Convert Spherical to UVs
                    float uClamped = (float)(phi + std::numbers::pi) / (2.0f * std::numbers::pi);
                    float vClamped = (float)((std::numbers::pi / 2.0f - theta) / std::numbers::pi);

                    float uScaled = uClamped * equirectangularImageData.GetPixelWidth();
                    float vScaled = vClamped * equirectangularImageData.GetPixelHeight();

                    int32 U = (int32)(roundf(uScaled));
                    int32 V = (int32)(roundf(vScaled));

                    // Clamp
                    if (U < 0) { U = 0; }
                    if (V < 0) { V = 0; }
                    if (U > maxPixelWidthIndex) { U = maxPixelWidthIndex; }
                    if (V > maxPixelHeightIndex) { V = maxPixelHeightIndex; }

                    const Vec4& pixelData = equirectangularImageData.GetPixel(U, V);
                    outCubemap[faceIndex].SetPixel(x, y, pixelData);
                }
            }
        }
    }


    Vec3 TextureLoader::details::ConvertFaceCoordinatesToCartesian(uint32 x, uint32 y, uint32 faceIndex, uint32 faceSideLength)
    {
        const float U = 2.0f * x / faceSideLength;
        const float V = 2.0f * y / faceSideLength;

        Vec3 cartesianCoords;
        switch (faceIndex)
        {
            case 0: cartesianCoords = { U - 1.0f, 1.0f, V - 1.0f }; break; // Positive X
            case 1: cartesianCoords = {1.0f - U, -1.0f,  V - 1.0f }; break; // Negative X
            case 2: cartesianCoords = {  1.0f - V,  U - 1.0f,  -1.0f }; break; // Positive Y (I flipped Y here for Vulkan!!)
            case 3: cartesianCoords = {  V - 1.0f,  U - 1.0f,  1.0f }; break; // Negative Y
            case 4: cartesianCoords = {  -1.0f, U - 1.0f,  V - 1.0f }; break; // Positive Z
            case 5: cartesianCoords = {  1.0f, 1.0f - U,  V - 1.0f }; break; // Negative Z
            default: ASTRAL_ERROR("Unsupported face index!");
        }
        return cartesianCoords;
    }

}
