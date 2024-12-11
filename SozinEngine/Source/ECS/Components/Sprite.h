//
// Created by Andrew Fagan on 11/28/24.
//

#pragma once

#include "pch.h"

#include "Renderer/Primitives/Texture.h"

#include "Renderer/Primitives/VertexArrayObject.h"

struct SpriteComponent
{
    SpriteComponent() : texture(nullptr), isUsed(false), vertexArrayObject(nullptr) {}
    explicit SpriteComponent(Renderer::Texture* texture, Renderer::VertexArrayObject* vertexArrayObject) : texture(texture), vertexArrayObject(vertexArrayObject), isUsed(true) {}
    Renderer::Texture* texture;
    Renderer::VertexArrayObject* vertexArrayObject;
    bool isUsed;
};