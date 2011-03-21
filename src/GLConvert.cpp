///////////////////////////////////////////////////////////////////////
// Wendy OpenGL library
// Copyright (c) 2010 Camilla Berglund <elmindreda@elmindreda.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any
// damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any
// purpose, including commercial applications, and to alter it and
// redistribute it freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you
//     must not claim that you wrote the original software. If you use
//     this software in a product, an acknowledgment in the product
//     documentation would be appreciated but is not required.
//
//  2. Altered source versions must be plainly marked as such, and
//     must not be misrepresented as being the original software.
//
//  3. This notice may not be removed or altered from any source
//     distribution.
//
///////////////////////////////////////////////////////////////////////

#include <wendy/Config.h>

#include <wendy/Core.h>
#include <wendy/Block.h>

#include <wendy/OpenGL.h>
#include <wendy/GLBuffer.h>
#include <wendy/GLTexture.h>
#include <wendy/GLProgram.h>
#include <wendy/GLContext.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <internal/GLConvert.h>

///////////////////////////////////////////////////////////////////////

namespace wendy
{
  namespace GL
  {

///////////////////////////////////////////////////////////////////////

GLenum convertToGL(IndexBuffer::Type type)
{
  switch (type)
  {
    case IndexBuffer::UINT8:
      return GL_UNSIGNED_BYTE;
    case IndexBuffer::UINT16:
      return GL_UNSIGNED_SHORT;
    case IndexBuffer::UINT32:
      return GL_UNSIGNED_INT;
  }

  logError("Invalid index buffer type %u", type);
  return 0;
}

GLenum convertToGL(VertexComponent::Type type)
{
  switch (type)
  {
    case VertexComponent::FLOAT32:
      return GL_FLOAT;
  }

  logError("Invalid vertex component type %u", type);
  return 0;
}

GLenum convertToGL(PixelFormat::Type type)
{
  switch (type)
  {
    case PixelFormat::UINT8:
      return GL_UNSIGNED_BYTE;
    case PixelFormat::UINT16:
      return GL_UNSIGNED_SHORT;
    case PixelFormat::UINT32:
      return GL_UNSIGNED_INT;
    case PixelFormat::FLOAT16:
      return GL_HALF_FLOAT_ARB;
    case PixelFormat::FLOAT32:
      return GL_FLOAT;
  }

  logError("No OpenGL equivalent for pixel format type %u", type);
  return 0;
}

GLenum convertToGL(PixelFormat::Semantic semantic)
{
  switch (semantic)
  {
    case PixelFormat::R:
      return GL_LUMINANCE;
    case PixelFormat::RG:
      return GL_LUMINANCE_ALPHA;
    case PixelFormat::RGB:
      return GL_RGB;
    case PixelFormat::RGBA:
      return GL_RGBA;
    case PixelFormat::DEPTH:
      return GL_DEPTH_COMPONENT;
  }

  logError("No OpenGL equivalent for pixel format semantic %u", semantic);
  return 0;
}

GLenum convertToGL(const PixelFormat& format)
{
  switch (format.getType())
  {
    case PixelFormat::UINT8:
    {
      switch (format.getSemantic())
      {
        case PixelFormat::R:
          return GL_LUMINANCE8;
        case PixelFormat::RG:
          return GL_LUMINANCE8_ALPHA8;
        case PixelFormat::RGB:
          return GL_RGB8;
        case PixelFormat::RGBA:
          return GL_RGBA8;
      }

      break;
    }

    case PixelFormat::UINT16:
    {
      if (format.getSemantic() == PixelFormat::DEPTH)
        return GL_DEPTH_COMPONENT16;

      break;
    }

    case PixelFormat::UINT24:
    {
      if (format.getSemantic() == PixelFormat::DEPTH)
        return GL_DEPTH_COMPONENT24;

      break;
    }

    case PixelFormat::UINT32:
    {
      if (format.getSemantic() == PixelFormat::DEPTH)
        return GL_DEPTH_COMPONENT32;

      break;
    }

    case PixelFormat::FLOAT16:
    {
      if (!GLEW_ARB_texture_float)
      {
        logError("Floating point textures (ARB_texture_float) not supported; cannot convert pixel format");
        return 0;
      }

      switch (format.getSemantic())
      {
        case PixelFormat::R:
          return GL_LUMINANCE16F_ARB;
        case PixelFormat::RG:
          return GL_LUMINANCE_ALPHA16F_ARB;
        case PixelFormat::RGB:
          return GL_RGB16F_ARB;
        case PixelFormat::RGBA:
          return GL_RGBA16F_ARB;
      }

      break;
    }

    case PixelFormat::FLOAT32:
    {
      if (!GLEW_ARB_texture_float)
      {
        logError("Floating point textures (ARB_texture_float) not supported; cannot convert pixel format");
        return 0;
      }

      switch (format.getSemantic())
      {
        case PixelFormat::R:
          return GL_LUMINANCE32F_ARB;
        case PixelFormat::RG:
          return GL_LUMINANCE_ALPHA32F_ARB;
        case PixelFormat::RGB:
          return GL_RGB32F_ARB;
        case PixelFormat::RGBA:
          return GL_RGBA32F_ARB;
      }

      break;
    }
  }

  logError("No OpenGL equivalent for pixel format \'%s\'",
           format.asString().c_str());
  return 0;
}

GLenum convertToGL(TextureType type)
{
  switch (type)
  {
    case TEXTURE_1D:
      return GL_TEXTURE_1D;
    case TEXTURE_2D:
      return GL_TEXTURE_2D;
    case TEXTURE_RECT:
      return GL_TEXTURE_RECTANGLE_ARB;
    case TEXTURE_CUBE:
      return GL_TEXTURE_CUBE_MAP;
  }

  logError("No OpenGL equivalent for texture type %u", type);
  return 0;
}

const char* asString(Sampler::Type type)
{
  switch (type)
  {
    case Sampler::SAMPLER_1D:
      return "sampler1D";
    case Sampler::SAMPLER_2D:
      return "sampler2D";
    case Sampler::SAMPLER_RECT:
      return "sampler2DRect";
    case Sampler::SAMPLER_CUBE:
      return "samplerCube";
  }

  logError("Invalid GLSL sampler type %u", type);
  return "INVALID";
}

const char* asString(Uniform::Type type)
{
  switch (type)
  {
    case Uniform::FLOAT:
      return "float";
    case Uniform::VEC2:
      return "vec2";
    case Uniform::VEC3:
      return "vec3";
    case Uniform::VEC4:
      return "vec4";
    case Uniform::MAT2:
      return "mat2";
    case Uniform::MAT3:
      return "mat3";
    case Uniform::MAT4:
      return "mat4";
  }

  logError("Invalid GLSL uniform type %u", type);
  return "INVALID";
}

const char* asString(Attribute::Type type)
{
  switch (type)
  {
    case Attribute::FLOAT:
      return "float";
    case Attribute::VEC2:
      return "vec2";
    case Attribute::VEC3:
      return "vec3";
    case Attribute::VEC4:
      return "vec4";
  }

  logError("Invalid GLSL attribute type %u", type);
  return "INVALID";
}

///////////////////////////////////////////////////////////////////////

  } /*namespace GL*/
} /*namespace wendy*/

///////////////////////////////////////////////////////////////////////
