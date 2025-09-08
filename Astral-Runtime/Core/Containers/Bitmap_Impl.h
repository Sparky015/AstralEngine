/**
* @file Bitmap_Impl.h
* @author Andrew Fagan
* @date 8/11/2025
*/


#pragma once

#include "Debug/Utilities/Error.h"
#include "Half/half.hpp"

#include <numbers>

namespace Astral {

   inline Bitmap::Bitmap(void* data, ImageFormat imageFormat, uint32 width, uint32 height) :
        m_MapData(),
        m_Width(width),
        m_Height(height),
        m_ImageFormat(imageFormat),
        m_BytesPerTexel(GetBytesPerTexel(imageFormat)),
        m_NumComponents(GetNumComponents(imageFormat))
    {
        uint32 totalNumBytes = width * height * m_BytesPerTexel;
        m_MapData.resize(totalNumBytes);

        if (data)
        {
            memcpy(m_MapData.data(), data, m_MapData.size());
        }
    }

    template <typename T>
    inline Vec4 Bitmap::GetPixel(uint32 x, uint32 y) const
    {
        if (x >= m_Width)
        {
            AE_WARN("Trying to get pixel data with out of bounds x value: " << x << "(Image width is " << m_Width << ")")
            x = m_Width - 1;
        }
        if (y >= m_Height)
        {
            AE_WARN("Trying to get pixel data with out of bounds y value: " << y << "(Image height is " << m_Height << ")")
            y = m_Height - 1;
        }

        uint32 yOffset = y * m_Width;
        uint32 componentOffset = (yOffset + x) * m_BytesPerTexel; // Considering the component size in the offset for the correct index
        uint32 pixelIndex = componentOffset;
        Vec4 component{0};
        memcpy(&component.x, &m_MapData[pixelIndex], sizeof(T));
        if (m_NumComponents > 1) { memcpy(&component.y, &m_MapData[pixelIndex + (1 * sizeof(T))], sizeof(T)); }
        if (m_NumComponents > 2) { memcpy(&component.z, &m_MapData[pixelIndex + (2 * sizeof(T))], sizeof(T)); }
        if (m_NumComponents > 3) { memcpy(&component.w, &m_MapData[pixelIndex + (3 * sizeof(T))], sizeof(T)); }
        return component;
    }


    template <typename T>
    inline void Bitmap::SetPixel(uint32 x, uint32 y, const Vec4& pixelData)
    {
        if (x >= m_Width)
        {
            AE_WARN("Trying to get pixel data with out of bounds x value: " << x << "(Image width is " << m_Width << ")")
            x = m_Width - 1;
        }
        if (y >= m_Height)
        {
            AE_WARN("Trying to get pixel data with out of bounds y value: " << y << "(Image height is " << m_Height << ")")
            y = m_Height - 1;
        }

        uint32 yOffset = y * m_Width;
        uint32 componentOffset = (yOffset + x) * m_BytesPerTexel; // Considering the component size in the offset for the correct index
        uint32 pixelIndex = componentOffset;

        *(T*)&m_MapData[pixelIndex] = pixelData.x;
        if (m_NumComponents > 1) { *(T*)&m_MapData[pixelIndex + (1 * sizeof(T))] = pixelData.y; }
        if (m_NumComponents > 2) { *(T*)&m_MapData[pixelIndex + (2 * sizeof(T))] = pixelData.z; }
        if (m_NumComponents > 3) { *(T*)&m_MapData[pixelIndex + (3 * sizeof(T))] = pixelData.w; }
    }


    inline void ConvertEquirectangularToCubemap(const Bitmap& equirectangularImageData, std::vector<Bitmap>& outCubemap)
    {
        uint32 faceSideLength = equirectangularImageData.GetPixelWidth() / 4;

        const uint32 numberOfFaces = 6;
        outCubemap.reserve(numberOfFaces);

        for (int i = 0; i < numberOfFaces; i++)
        {
            outCubemap.emplace_back(nullptr, ImageFormat::R16G16B16A16_UNORM, faceSideLength, faceSideLength);
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

                    const Vec4& pixelData = equirectangularImageData.GetPixel<float>(U, V);
                    outCubemap[faceIndex].SetPixel<half_float::half>(x, y, pixelData);
                }
            }
        }
    }


    inline Vec3 ConvertFaceCoordinatesToCartesian(uint32 x, uint32 y, uint32 faceIndex, uint32 faceSideLength)
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
            default: AE_ERROR("Unsupported face index!");
        }
        return cartesianCoords;
    }

}