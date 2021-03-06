///////////////////////////////////////////////////////////////////////
// Wendy OpenAL library
// Copyright (c) 2011 Camilla Berglund <elmindreda@elmindreda.org>
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
#ifndef WENDY_OPENAL_H
#define WENDY_OPENAL_H
///////////////////////////////////////////////////////////////////////

namespace wendy
{
  namespace AL
  {

///////////////////////////////////////////////////////////////////////

/*! @defgroup openal OpenAL wrapper API
 *
 *  These classes wrap parts of the OpenAL API, maintaining a rather close
 *  mapping to the underlying concepts, but providing useful services and a
 *  semblance of automatic resource management.
 */

///////////////////////////////////////////////////////////////////////

WENDY_CHECKFORMAT(1, bool checkAL(const char* format, ...));

WENDY_CHECKFORMAT(1, bool checkALC(const char* format, ...));

///////////////////////////////////////////////////////////////////////

  } /*namespace AL*/
} /*namespace wendy*/

///////////////////////////////////////////////////////////////////////
#endif /*WENDY_OPENAL_H*/
///////////////////////////////////////////////////////////////////////
