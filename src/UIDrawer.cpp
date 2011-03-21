//////////////////////////////////////////////////////////////////////
// Wendy user interface library
// Copyright (c) 2006 Camilla Berglund <elmindreda@elmindreda.org>
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

#include <wendy/UIDrawer.h>

///////////////////////////////////////////////////////////////////////

namespace wendy
{
  namespace UI
  {

///////////////////////////////////////////////////////////////////////

Alignment::Alignment(HorzAlignment initHorizontal,
                     VertAlignment initVertical):
  horizontal(initHorizontal),
  vertical(initVertical)
{
}

void Alignment::set(HorzAlignment newHorizontal, VertAlignment newVertical)
{
  horizontal = newHorizontal;
  vertical = newVertical;
}

///////////////////////////////////////////////////////////////////////

void Drawer::begin(void)
{
  GL::Context& context = pool.getContext();
  context.setCurrentSharedProgramState(state);

  GL::Canvas& canvas = context.getCurrentCanvas();
  state->setOrthoProjectionMatrix(float(canvas.getWidth()),
                                  float(canvas.getHeight()));
}

void Drawer::end(void)
{
  pool.getContext().setCurrentSharedProgramState(NULL);
}

bool Drawer::pushClipArea(const Rect& area)
{
  GL::Context& context = pool.getContext();
  GL::Canvas& canvas = context.getCurrentCanvas();

  vec2 scale(1.f / float(canvas.getWidth()), 1.f / float(canvas.getHeight()));

  if (!clipAreaStack.push(area * scale))
    return false;

  context.setScissorArea(clipAreaStack.getTotal());
  return true;
}

void Drawer::popClipArea(void)
{
  if (clipAreaStack.getCount() == 1)
  {
    logError("Cannot pop empty clip area stack");
    return;
  }

  clipAreaStack.pop();

  GL::Context& context = pool.getContext();

  context.setScissorArea(clipAreaStack.getTotal());
}

void Drawer::drawPoint(const vec2& point, const vec4& color)
{
  Vertex2fv vertex;
  vertex.position = point;

  GL::VertexRange range;
  if (!pool.allocateVertices(range, 1, Vertex2fv::format))
    return;

  range.copyFrom(&vertex);

  setDrawingState(color, true);

  pool.getContext().render(GL::PrimitiveRange(GL::POINT_LIST, range));
}

void Drawer::drawLine(const Segment2& segment, const vec4& color)
{
  Vertex2fv vertices[2];
  vertices[0].position = segment.start;
  vertices[1].position = segment.end;

  GL::VertexRange range;
  if (!pool.allocateVertices(range, 2, Vertex2fv::format))
    return;

  range.copyFrom(vertices);

  setDrawingState(color, true);

  pool.getContext().render(GL::PrimitiveRange(GL::LINE_LIST, range));
}

void Drawer::drawTriangle(const Triangle2& triangle, const vec4& color)
{
  Vertex2fv vertices[3];
  vertices[0].position = triangle.P[0];
  vertices[1].position = triangle.P[1];
  vertices[2].position = triangle.P[2];

  GL::VertexRange range;
  if (!pool.allocateVertices(range, 3, Vertex2fv::format))
    return;

  range.copyFrom(vertices);

  setDrawingState(color, true);

  pool.getContext().render(GL::PrimitiveRange(GL::TRIANGLE_LIST, range));
}

void Drawer::drawBezier(const BezierCurve2& spline, const vec4& color)
{
  BezierCurve2::PointList points;
  spline.tessellate(points);

  GL::VertexRange range;
  if (!pool.allocateVertices(range, points.size(), Vertex2fv::format))
    return;

  // Realize vertices
  {
    GL::VertexRangeLock<Vertex2fv> vertices(range);

    for (unsigned int i = 0;  i < points.size();  i++)
      vertices[i].position = points[i];
  }

  setDrawingState(color, true);

  pool.getContext().render(GL::PrimitiveRange(GL::LINE_STRIP, range));
}

void Drawer::drawRectangle(const Rect& rectangle, const vec4& color)
{
  float minX, minY, maxX, maxY;
  rectangle.getBounds(minX, minY, maxX, maxY);

  if (maxX - minX < 1.f || maxY - minY < 1.f)
    return;

  maxX -= 1.f;
  maxY -= 1.f;

  Vertex2fv vertices[4];
  vertices[0].position = vec2(minX, minY);
  vertices[1].position = vec2(maxX, minY);
  vertices[2].position = vec2(maxX, maxY);
  vertices[3].position = vec2(minX, maxY);

  GL::VertexRange range;
  if (!pool.allocateVertices(range, 4, Vertex2fv::format))
    return;

  range.copyFrom(vertices);

  setDrawingState(color, true);

  pool.getContext().render(GL::PrimitiveRange(GL::LINE_LOOP, range));
}

void Drawer::fillTriangle(const Triangle2& triangle, const vec4& color)
{
  Vertex2fv vertices[3];
  vertices[0].position = triangle.P[0];
  vertices[1].position = triangle.P[1];
  vertices[2].position = triangle.P[2];

  GL::VertexRange range;
  if (!pool.allocateVertices(range, 3, Vertex2fv::format))
    return;

  range.copyFrom(vertices);

  setDrawingState(color, false);

  pool.getContext().render(GL::PrimitiveRange(GL::TRIANGLE_LIST, range));
}

void Drawer::fillRectangle(const Rect& rectangle, const vec4& color)
{
  float minX, minY, maxX, maxY;
  rectangle.getBounds(minX, minY, maxX, maxY);

  if (maxX - minX < 1.f || maxY - minY < 1.f)
    return;

  maxX -= 1.f;
  maxY -= 1.f;

  Vertex2fv vertices[4];
  vertices[0].position = vec2(minX, minY);
  vertices[1].position = vec2(maxX, minY);
  vertices[2].position = vec2(maxX, maxY);
  vertices[3].position = vec2(minX, maxY);

  GL::VertexRange range;
  if (!pool.allocateVertices(range, 4, Vertex2fv::format))
    return;

  range.copyFrom(vertices);

  setDrawingState(color, false);

  pool.getContext().render(GL::PrimitiveRange(GL::TRIANGLE_FAN, range));
}

void Drawer::blitTexture(const Rect& area, GL::Texture& texture)
{
  float minX, minY, maxX, maxY;
  area.getBounds(minX, minY, maxX, maxY);

  if (maxX - minX < 1.f || maxY - minY < 1.f)
    return;

  maxX -= 1.f;
  maxY -= 1.f;

  Vertex2ft2fv vertices[4];
  vertices[0].texCoord = vec2(0.f, 0.f);
  vertices[0].position = vec2(minX, minY);
  vertices[1].texCoord = vec2(1.f, 0.f);
  vertices[1].position = vec2(maxX, minY);
  vertices[2].texCoord = vec2(1.f, 1.f);
  vertices[2].position = vec2(maxX, maxY);
  vertices[3].texCoord = vec2(0.f, 1.f);
  vertices[3].position = vec2(minX, maxY);

  GL::VertexRange range;
  if (!pool.allocateVertices(range, 4, Vertex2ft2fv::format))
    return;

  range.copyFrom(vertices);

  if (texture.getFormat().getSemantic() == PixelFormat::RGBA)
    blitPass.setBlendFactors(GL::BLEND_SRC_ALPHA, GL::BLEND_ONE_MINUS_SRC_ALPHA);
  else
    blitPass.setBlendFactors(GL::BLEND_ONE, GL::BLEND_ZERO);

  blitPass.setSamplerState("image", &texture);
  blitPass.apply();

  pool.getContext().render(GL::PrimitiveRange(GL::TRIANGLE_FAN, range));

  blitPass.setSamplerState("image", NULL);
}

void Drawer::drawText(const Rect& area,
                      const String& text,
		      const Alignment& alignment,
		      const vec3& color)
{
  if (text.empty())
    return;

  Rect metrics = currentFont->getTextMetrics(text);

  vec2 penPosition;

  switch (alignment.horizontal)
  {
    case LEFT_ALIGNED:
      penPosition.x = area.position.x - metrics.position.x;
      break;
    case CENTERED_ON_X:
      penPosition.x = area.getCenter().x - metrics.getCenter().x;
      break;
    case RIGHT_ALIGNED:
      penPosition.x = (area.position.x + area.size.x) -
                      (metrics.position.x + metrics.size.x);
      break;
    default:
      logError("Invalid horizontal alignment");
      return;
  }

  switch (alignment.vertical)
  {
    case BOTTOM_ALIGNED:
      penPosition.y = area.position.y - metrics.position.y;
      break;
    case CENTERED_ON_Y:
      penPosition.y = area.getCenter().y - metrics.getCenter().y;
      break;
    case TOP_ALIGNED:
      penPosition.y = (area.position.y + area.size.y) -
                      (metrics.position.y + metrics.size.y);
      break;
    default:
      logError("Invalid vertical alignment");
      return;
  }

  currentFont->drawText(penPosition, vec4(color, 1.f), text);
}

void Drawer::drawText(const Rect& area,
                      const String& text,
		      const Alignment& alignment,
		      WidgetState state)
{
  switch (state)
  {
    case STATE_DISABLED:
      drawText(area, text, alignment, textColor);
      break;

    case STATE_NORMAL:
      drawText(area, text, alignment, textColor);
      break;

    case STATE_ACTIVE:
      drawText(area, text, alignment, textColor);
      break;

    case STATE_SELECTED:
      drawText(area, text, alignment, selectedTextColor);
      break;

    default:
      logError("Invalid widget state %u", state);
      break;
  }
}

void Drawer::drawWell(const Rect& area, WidgetState state)
{
  vec3 fillColor;

  switch (state)
  {
    case STATE_ACTIVE:
      fillColor = wellColor * 1.2f;
      break;
    case STATE_DISABLED:
      fillColor = wellColor * 0.8f;
      break;
    default:
      fillColor = wellColor;
      break;
  }

  fillRectangle(area, vec4(fillColor, 1.f));
  drawRectangle(area, vec4(vec3(0.f), 1.f));
}

void Drawer::drawFrame(const Rect& area, WidgetState state)
{
  vec3 fillColor;

  switch (state)
  {
    case STATE_ACTIVE:
      fillColor = widgetColor * 1.2f;
      break;
    case STATE_DISABLED:
      fillColor = widgetColor * 0.8f;
      break;
    default:
      fillColor = widgetColor;
      break;
  }

  fillRectangle(area, vec4(fillColor, 1.f));
  drawRectangle(area, vec4(vec3(0.f), 1.f));
}

void Drawer::drawHandle(const Rect& area, WidgetState state)
{
  drawFrame(area, state);
}

void Drawer::drawButton(const Rect& area, WidgetState state, const String& text)
{
  drawFrame(area, state);

  if (text.length())
    drawText(area, text);
}

const vec3& Drawer::getWidgetColor(void)
{
  return widgetColor;
}

void Drawer::setWidgetColor(const vec3& newColor)
{
  widgetColor = newColor;
}

const vec3& Drawer::getTextColor(void)
{
  return textColor;
}

void Drawer::setTextColor(const vec3& newColor)
{
  textColor = newColor;
}

const vec3& Drawer::getWellColor(void)
{
  return wellColor;
}

void Drawer::setWellColor(const vec3& newColor)
{
  wellColor = newColor;
}

const vec3& Drawer::getSelectionColor(void)
{
  return selectionColor;
}

void Drawer::setSelectionColor(const vec3& newColor)
{
  selectionColor = newColor;
}

const vec3& Drawer::getSelectedTextColor(void)
{
  return selectedTextColor;
}

void Drawer::setSelectedTextColor(const vec3& newColor)
{
  selectedTextColor = newColor;
}

render::Font& Drawer::getCurrentFont(void)
{
  return *currentFont;
}

render::Font& Drawer::getDefaultFont(void)
{
  return *defaultFont;
}

float Drawer::getDefaultEM(void) const
{
  return defaultFont->getHeight();
}

float Drawer::getCurrentEM(void) const
{
  return currentFont->getHeight();
}

void Drawer::setCurrentFont(render::Font* newFont)
{
  if (newFont)
    currentFont = newFont;
  else
    currentFont = defaultFont;
}

render::GeometryPool& Drawer::getGeometryPool(void) const
{
  return pool;
}

Drawer* Drawer::create(render::GeometryPool& pool)
{
  Ptr<Drawer> drawer(new Drawer(pool));
  if (!drawer->init())
    return NULL;

  return drawer.detachObject();
}

Drawer::Drawer(render::GeometryPool& initPool):
  pool(initPool)
{
}

bool Drawer::init(void)
{
  state = new GL::SharedProgramState();

  widgetColor = vec3(0.7f);
  textColor = vec3(0.f);
  wellColor = widgetColor * 1.2f;
  selectionColor = vec3(0.3f);
  selectedTextColor = vec3(1.f);

  clipAreaStack.push(Rect(0.f, 0.f, 1.f, 1.f));

  // Load default font
  {
    Path path("wendy/default.font");

    defaultFont = render::Font::read(pool, path);
    if (!defaultFont)
    {
      logError("Failed to load default UI font \'%s\'",
               path.asString().c_str());
      return false;
    }

    currentFont = defaultFont;
  }

  // Set up solid pass
  {
    Path path("wendy/UIDrawSolid.program");

    Ref<GL::Program> program = GL::Program::read(pool.getContext(), path);
    if (!program)
    {
      logError("Failed to load UI drawing shader program \'%s\'",
               path.asString().c_str());
      return false;
    }

    GL::ProgramInterface interface;
    interface.addUniform("color", GL::Uniform::VEC4);
    interface.addAttribute("wyPosition", GL::Attribute::VEC2);

    if (!interface.matches(*program, true))
    {
      logError("UI drawing shader program \'%s\' does not conform to the required interface",
               path.asString().c_str());
      return false;
    }

    drawPass.setProgram(program);
    drawPass.setCullMode(GL::CULL_NONE);
    drawPass.setDepthTesting(false);
    drawPass.setDepthWriting(false);
  }

  // Set up blitting pass
  {
    Path path("wendy/UIDrawMapped.program");

    Ref<GL::Program> program = GL::Program::read(pool.getContext(), path);
    if (!program)
    {
      logError("Failed to load UI blitting shader program \'%s\'",
               path.asString().c_str());
      return false;
    }

    GL::ProgramInterface interface;
    interface.addSampler("image", GL::Sampler::SAMPLER_2D);
    interface.addAttribute("wyPosition", GL::Attribute::VEC2);
    interface.addAttribute("wyTexCoord", GL::Attribute::VEC2);

    if (!interface.matches(*program, true))
    {
      logError("UI blitting shader program \'%s\' does not conform to the required interface",
               path.asString().c_str());
      return false;
    }

    blitPass.setProgram(program);
    blitPass.setCullMode(GL::CULL_NONE);
    blitPass.setDepthTesting(false);
    blitPass.setDepthWriting(false);
  }

  return true;
}

void Drawer::setDrawingState(const vec4& color, bool wireframe)
{
  drawPass.setUniformState("color", color);

  if (color.a == 1.f)
    drawPass.setBlendFactors(GL::BLEND_ONE, GL::BLEND_ZERO);
  else
    drawPass.setBlendFactors(GL::BLEND_SRC_ALPHA, GL::BLEND_ONE_MINUS_SRC_ALPHA);

  drawPass.setWireframe(wireframe);
  drawPass.apply();
}

///////////////////////////////////////////////////////////////////////

  } /*namespace UI*/
} /*namespace wendy*/

///////////////////////////////////////////////////////////////////////