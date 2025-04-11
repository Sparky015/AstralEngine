//
// Created by Andrew Fagan on 11/28/24.
//

#pragma once

#include "pch.h"

#include "Renderer/Primitives/Texture.h"

#include "Renderer/Primitives/VertexArrayObject.h"

struct SpriteComponent
{
    SpriteComponent() : texture(nullptr), vertexArrayObject(nullptr), isUsed(false) {}
    explicit SpriteComponent(Astral::Texture* texture, Astral::VertexArrayObject* vertexArrayObject) : texture(texture), vertexArrayObject(vertexArrayObject), isUsed(true) {}
    Astral::Texture* texture;
    Astral::VertexArrayObject* vertexArrayObject;
    bool isUsed;
};