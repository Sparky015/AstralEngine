/**
* @file CubeLUTLoader.cpp
* @author Andrew Fagan
* @date 9/4/25
*/

#include "CubeLUTLoader.h"

#include "Asset/Asset.h"
#include "Asset/Ref.h"
#include "Core/Math/Math.h"
#include "Renderer/RHI/Resources/Texture.h"
#include "Renderer/Common/CubeLUT.h"
#include "Debug/Utilities/Loggers.h"

namespace Astral::CubeLUTLoader {

    Ref<Asset> LoadAsset(const std::filesystem::path& filePath)
    {
        CubeLUT cubeLUT;

        if (filePath.extension() == ".cube") // Load LUT (Look Up Table)
        {

            std::vector<Vec4> cubeLUTData;
            std::vector<Vec4> shaperData;
            Vec2 shaperInputRange;

            std::ifstream file(filePath);
            if (!file.is_open()) { return nullptr; }

            uint32 shaperLinesToFill = 0;
            uint32 cubeLUTSideLength = 0;
            uint32 shaperSize = 0;
            std::string line;
            while (std::getline(file, line))
            {
                if (line.empty() || line[0] == '#') continue;

                std::istringstream iss(line);
                std::string key;
                iss >> key;

                if (key == "LUT_1D_SIZE")
                {
                    iss >> shaperSize;
                    shaperData.reserve(shaperSize);
                    shaperLinesToFill = shaperSize;
                }
                else if (key == "LUT_3D_SIZE")
                {
                    iss >> cubeLUTSideLength;
                    cubeLUTData.reserve(cubeLUTSideLength * cubeLUTSideLength * cubeLUTSideLength);
                }
                else if (key == "LUT_1D_INPUT_RANGE")
                {
                    iss >> shaperInputRange.x >> shaperInputRange.y;
                }
                else if (isdigit(key[0]) || key[0] == '-' || key[0] == '.') // If the line starts with a number (like -0.4, or 1.2 or .2)
                {
                    iss.clear(); // Clears any error state
                    iss.str(line);

                    float r, g, b;
                    iss >> r >> g >> b;

                    if (shaperLinesToFill > 0)
                    {
                        shaperData.push_back({r, g, b, 1.0f});
                        shaperLinesToFill--;
                    }
                    else
                    {
                        cubeLUTData.push_back({r, g, b, 1.0f});
                    }
                }
            }

            if (shaperData.size() != shaperSize)
            {
                WARN("CubeLUT Loader: Failed to parse all shaper data!");
                return nullptr;
            }
            if (cubeLUTData.size() != cubeLUTSideLength * cubeLUTSideLength * cubeLUTSideLength)
            {
                WARN("CubeLUT Loader: Failed to parse all cube lut data!");
                return nullptr;
            }

            cubeLUT.Shaper1D = Texture::Create1DTexture(shaperData.data(), shaperSize, ImageFormat::R32G32B32A32_SFLOAT);
            cubeLUT.LUT3D = Texture::Create3DTexture(cubeLUTData.data(), cubeLUTSideLength, cubeLUTSideLength, ImageFormat::R32G32B32A32_SFLOAT);
            cubeLUT.ShaperInputRange = shaperInputRange;
        }

        Ref<CubeLUT> cubeLUTRef = CreateRef<CubeLUT>(cubeLUT);
        return cubeLUTRef;
    }

}
