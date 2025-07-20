/**
* @file TextureLoader.h
* @author Andrew Fagan
* @date 4/13/25
*/

#pragma once

#include "Asset/Asset.h"
#include "Core/SmartPointers.h"

#include <filesystem>

#include "Astral.h"
#include "Astral.h"
#include "Core/Math/Math.h"
#include "Renderer/RHI/Common/ImageFormats.h"

namespace Astral::TextureLoader {

    Ref<Asset> LoadAsset(const std::filesystem::path& filePath);

    namespace details {

        /**
         * @brief Represents the data of a 2D image
         */
        class Bitmap
        {
        public:
            explicit Bitmap(void* data, ImageFormat imageFormat, uint32 width, uint32 height);

            /**
             * @brief Gets the component data of a pixel
             * @param x The x coordinate of the pixel
             * @param y The y coordinate of the pixel
             * @return The component data for the pixel
             * @note The vec4 being returned will only contain the components for the format given
             */
            UVec4 GetPixel(uint32 x, uint32 y) const;

            /**
             * @brief Sets the component data of a pixel
             * @param x The x coordinate of the pixel
             * @param y The y coordinate of the pixel
             * @param pixelData The component data to set to the pixel.
             * @note Only the components for the given format will be taken from pixelData
             */
            void SetPixel(uint32 x, uint32 y, const UVec4& pixelData);

            /**
             * @brief Gets the width of the bitmap in pixels
             * @return The width of the bitmap in pixels
             */
            uint32 GetPixelWidth() const { return m_Width; }

            /**
             * @brief Gets the height of the map in pixels
             * @return The height of the bitmap in pixels
             */
            uint32 GetPixelHeight() const { return m_Height; }

            /**
             * @brief Gets the total size of the bitmap in bytes
             * @return The total size of the bitmap in bytes
             */
            uint32 GetTotalBitmapSize() const { return m_Width * m_Height * m_BytesPerTexel; }

            /**
             * @brief Gets a pointer to the data of the image
             * @return The data of the image
             */
            const void* GetData() const { return m_MapData.data(); }

        private:
            std::vector<uint8> m_MapData;
            ImageFormat m_ImageFormat;
            uint32 m_Width;
            uint32 m_Height;
            uint32 m_BytesPerTexel;
        };

        void ConvertEquirectangularToCubemap(const Bitmap& equirectangularImageData, std::vector<Bitmap>& outCubemap);

        Vec3 ConvertFaceCoordinatesToCartesian(uint32 x, uint32 y, uint32 faceIndex, uint32 faceSideLength);

    }

}
