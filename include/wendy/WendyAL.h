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
#ifndef WENDY_WENDYAL_H
#define WENDY_WENDYAL_H
///////////////////////////////////////////////////////////////////////

/*! @defgroup openal OpenAL wrapper API
 */

///////////////////////////////////////////////////////////////////////

#if WENDY_INCLUDE_OPENAL

#include <wendy/ALContext.h>
#include <wendy/ALBuffer.h>
#include <wendy/ALSource.h>

#else
#error "OpenAL module is not enabled"
#endif

///////////////////////////////////////////////////////////////////////
#endif /*WENDY_WENDYAL_H*/
///////////////////////////////////////////////////////////////////////
