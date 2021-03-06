///////////////////////////////////////////////////////////////////////
// Wendy input library
// Copyright (c) 2009 Camilla Berglund <elmindreda@elmindreda.org>
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
#ifndef WENDY_INPUT_H
#define WENDY_INPUT_H
///////////////////////////////////////////////////////////////////////

#include <wendy/Core.h>
#include <wendy/Transform.h>
#include <wendy/Signal.h>

///////////////////////////////////////////////////////////////////////

namespace wendy
{
  namespace GL
  {
    class Context;
  }
}

///////////////////////////////////////////////////////////////////////

namespace wendy
{
  namespace input
  {

///////////////////////////////////////////////////////////////////////

/*! @brief Keyboard key enumeration.
 *  @ingroup input
 */
enum Key
{
  KEY_SPACE,
  KEY_APOSTROPHE,
  KEY_COMMA,
  KEY_MINUS,
  KEY_PERIOD,
  KEY_SLASH,
  KEY_0,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_5,
  KEY_6,
  KEY_7,
  KEY_8,
  KEY_9,
  KEY_SEMICOLON,
  KEY_EQUAL,
  KEY_A,
  KEY_B,
  KEY_C,
  KEY_D,
  KEY_E,
  KEY_F,
  KEY_G,
  KEY_H,
  KEY_I,
  KEY_J,
  KEY_K,
  KEY_L,
  KEY_M,
  KEY_N,
  KEY_O,
  KEY_P,
  KEY_Q,
  KEY_R,
  KEY_S,
  KEY_T,
  KEY_U,
  KEY_V,
  KEY_W,
  KEY_X,
  KEY_Y,
  KEY_Z,
  KEY_LEFT_BRACKET,
  KEY_BACKSLASH,
  KEY_RIGHT_BRACKET,
  KEY_GRAVE_ACCENT,
  KEY_WORLD_1,
  KEY_WORLD_2,
  KEY_ESCAPE,
  KEY_ENTER,
  KEY_TAB,
  KEY_BACKSPACE,
  KEY_INSERT,
  KEY_DELETE,
  KEY_RIGHT,
  KEY_LEFT,
  KEY_DOWN,
  KEY_UP,
  KEY_PAGE_UP,
  KEY_PAGE_DOWN,
  KEY_HOME,
  KEY_END,
  KEY_CAPS_LOCK,
  KEY_SCROLL_LOCK,
  KEY_NUM_LOCK,
  KEY_PRINT_SCREEN,
  KEY_PAUSE,
  KEY_F1,
  KEY_F2,
  KEY_F3,
  KEY_F4,
  KEY_F5,
  KEY_F6,
  KEY_F7,
  KEY_F8,
  KEY_F9,
  KEY_F10,
  KEY_F11,
  KEY_F12,
  KEY_F13,
  KEY_F14,
  KEY_F15,
  KEY_F16,
  KEY_F17,
  KEY_F18,
  KEY_F19,
  KEY_F20,
  KEY_F21,
  KEY_F22,
  KEY_F23,
  KEY_F24,
  KEY_F25,
  KEY_KP_0,
  KEY_KP_1,
  KEY_KP_2,
  KEY_KP_3,
  KEY_KP_4,
  KEY_KP_5,
  KEY_KP_6,
  KEY_KP_7,
  KEY_KP_8,
  KEY_KP_9,
  KEY_KP_DECIMAL,
  KEY_KP_DIVIDE,
  KEY_KP_MULTIPLY,
  KEY_KP_SUBTRACT,
  KEY_KP_ADD,
  KEY_KP_ENTER,
  KEY_KP_EQUAL,
  KEY_LEFT_SHIFT,
  KEY_LEFT_CONTROL,
  KEY_LEFT_ALT,
  KEY_LEFT_SUPER,
  KEY_RIGHT_SHIFT,
  KEY_RIGHT_CONTROL,
  KEY_RIGHT_ALT,
  KEY_RIGHT_SUPER,
  KEY_MENU
};

///////////////////////////////////////////////////////////////////////

/* @brief Mouse button enumeration.
 * @ingroup input
 */
enum Button
{
  BUTTON_LEFT,
  BUTTON_RIGHT,
  BUTTON_MIDDLE
};

///////////////////////////////////////////////////////////////////////

/*! @brief Input hook interface.
 *  @ingroup input
 *
 *  This is intended for hotkeys that should work regardless of which target
 *  currently has focus, such as a key to bring down the console.  It gets first
 *  pick of any input and can prevent it from being passed on to the current
 *  target.
 */
class Hook
{
public:
  /*! Destructor.
   */
  virtual ~Hook();
  /*! Called when a key has been pressed or released.
   *  @return @c true to prevent this event from reaching the current input
   *  target, or @c false to pass it on.
   */
  virtual bool onKeyPressed(Key key, bool pressed);
  /*! Called when a Unicode character has been input.
   *  @return @c true to prevent this event from reaching the current input
   *  target, or @c false to pass it on.
   */
  virtual bool onCharInput(uint32 character);
  /*! Called when a mouse button has been clicked or released.
   *  @return @c true to prevent this event from reaching the current input
   *  target, or @c false to pass it on.
   */
  virtual bool onButtonClicked(Button button, bool clicked);
  /*! Called when the mouse cursor has been moved.
   *  @return @c true to prevent this event from reaching the current input
   *  target, or @c false to pass it on.
   */
  virtual bool onCursorMoved(const ivec2& position);
  /*! Called when a scrolling device has been used.
   *  @return @c true to prevent this event from reaching the current input
   *  target, or @c false to pass it on.
   */
  virtual bool onScrolled(double x, double y);
};

///////////////////////////////////////////////////////////////////////

/*! @brief Input target interface.
 *  @ingroup input
 *
 *  This is intended for use by game modules such as menus, editors, the console
 *  and the game itself.
 */
class Target
{
public:
  /*! Destructor.
   */
  virtual ~Target();
  /*! Called when the window has been resized.
   */
  virtual void onWindowResized(uint width, uint height);
  /*! Called when a key has been pressed or released.
   */
  virtual void onKeyPressed(Key key, bool pressed);
  /*! Called when a Unicode character has been input.
   */
  virtual void onCharInput(uint32 character);
  /*! Called when a mouse button has been clicked or released.
   */
  virtual void onButtonClicked(Button button, bool clicked);
  /*! Called when the mouse cursor has been moved.
   */
  virtual void onCursorMoved(const ivec2& position);
  /*! Called when a scrolling device has been used.
   */
  virtual void onScrolled(double x, double y);
  /*! Called when this input target has lost or gained focus.
   */
  virtual void onFocusChanged(bool activated);
};

///////////////////////////////////////////////////////////////////////

/*! @brief Input manager.
 *  @ingroup input
 *
 *  This class provides basic HID (input) signals.
 */
class Window : public Singleton<Window>
{
public:
  /*! Destructor.
   */
  virtual ~Window();
  void captureCursor();
  void releaseCursor();
  /*! @return @c true if the specified key is pressed, otherwise @c false.
   *  @param[in] key The desired key.
   */
  bool isKeyDown(Key key) const;
  /*! @return @c true if the specified mouse button is pressed, otherwise @c false.
   *  @param[in] button The desired mouse button.
   */
  bool isButtonDown(Button button) const;
  bool isCursorCaptured() const;
  /*! @return The width, in pixels, of the mousable screen area.
   */
  uint getWidth() const;
  /*! @return The height, in pixels, of the mousable screen area.
   */
  uint getHeight() const;
  /*! @return The current mouse position.
   */
  ivec2 getCursorPosition() const;
  /*! Places the the mouse cursor at the specified position.
   *  @param[in] newPosition The desired mouse position.
   */
  void setCursorPosition(const ivec2& newPosition);
  Hook* getHook() const;
  void setHook(Hook* newHook);
  Target* getTarget() const;
  void setTarget(Target* newTarget);
  /*! @return The context underlying this input manager.
   */
  GL::Context& getContext() const;
  static bool createSingleton(GL::Context& context);
private:
  Window(GL::Context& context);
  Window(const Window& source);
  Window& operator = (const Window& source);
  void onWindowResized(uint width, uint height);
  static void keyboardCallback(GLFWwindow* window, int key, int action);
  static void characterCallback(GLFWwindow* window, int character);
  static void mousePosCallback(GLFWwindow* window, int x, int y);
  static void mouseButtonCallback(GLFWwindow* window, int button, int action);
  static void scrollCallback(GLFWwindow* window, double x, double y);
  GL::Context& context;
  GLFWwindow* handle;
  Hook* currentHook;
  Target* currentTarget;
  static Window* instance;
};

///////////////////////////////////////////////////////////////////////

/*! @ingroup input
 */
class SpectatorController
{
public:
  SpectatorController();
  void update(Time deltaTime);
  void release();
  void inputKeyPress(Key key, bool pressed);
  void inputButtonClick(Button button, bool clicked);
  void inputCursorOffset(const ivec2& offset);
  const Transform3& getTransform() const;
  void setPosition(const vec3& newPosition);
  float getAngleX() const;
  float getAngleY() const;
  void setRotation(float newAngleX, float newAngleY);
  float getSpeed() const;
  void setSpeed(float newSpeed);
private:
  enum Direction
  {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    FORWARD,
    BACK
  };
  Transform3 transform;
  float angleX;
  float angleY;
  float speed;
  bool directions[6];
  bool turbo;
};

///////////////////////////////////////////////////////////////////////

class TextController : public Target
{
public:
  TextController();
  void onKeyPressed(Key key, bool pressed);
  void onCharInput(uint32 character);
  const String& getText() const;
  void setText(const String& newText);
  size_t getCaretPosition() const;
  void setCaretPosition(size_t newPosition);
private:
  bool isCtrlKeyDown() const;
  String text;
  size_t caretPosition;
};

///////////////////////////////////////////////////////////////////////

  } /*namespace input*/
} /*namespace wendy*/

///////////////////////////////////////////////////////////////////////
#endif /*WENDY_INPUT_H*/
///////////////////////////////////////////////////////////////////////
