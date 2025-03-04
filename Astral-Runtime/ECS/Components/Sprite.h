//
// Created by Andrew Fagan on 11/28/24.
//

#pragma once

#include "pch.h"

#include "Renderer/AbstractionLayer/Primitives/Texture.h"

#include "Renderer/AbstractionLayer/Primitives/VertexArrayObject.h"

struct SpriteComponent
{
    SpriteComponent() : texture(nullptr), vertexArrayObject(nullptr), isUsed(false) {}
    explicit SpriteComponent(Graphics::Texture* texture, Graphics::VertexArrayObject* vertexArrayObject) : texture(texture), vertexArrayObject(vertexArrayObject), isUsed(true) {}
    Graphics::Texture* texture;
    Graphics::VertexArrayObject* vertexArrayObject;
    bool isUsed;
};