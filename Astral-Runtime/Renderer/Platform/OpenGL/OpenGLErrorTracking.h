/**
* @file ErrorTracking.h
* @author Andrew Fagan
* @date 12/5/2024
*/

#pragma once

namespace Graphics {

#define GLCheckError() while (GLenum error = glGetError()) { WARN("[OpenGL] Error Code: " << error); }

}