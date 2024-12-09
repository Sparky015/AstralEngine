//
// Created by Andrew Fagan on 11/28/24.
//

#pragma once

#include "pch.h"

#include "Renderer/Primitives/Texture.h"

struct SpriteComponent
{
    SpriteComponent() : texture(nullptr), isUsed(false) {}
    explicit SpriteComponent(Renderer::Texture* texture) : texture(texture), isUsed(true) {}
    Renderer::Texture* texture;
    bool isUsed;
};