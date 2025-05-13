//
// Created by Andrew Fagan on 11/28/24.
//

#pragma once

#include "pch.h"

#include "Renderer/RHI/Resources/Texture.h"

#include "Renderer/RHI/Resources/VertexArrayObject.h"

struct SpriteComponent
{
    SpriteComponent() : texture(nullptr), vertexArrayObject(nullptr), isUsed(false) {}
    explicit SpriteComponent(Astral::Texture* texture, Astral::VertexArrayObject* vertexArrayObject) : texture(texture), vertexArrayObject(vertexArrayObject), isUsed(true) {}
    Astral::Texture* texture;
    Astral::VertexArrayObject* vertexArrayObject;
    bool isUsed;
};