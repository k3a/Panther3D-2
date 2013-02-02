/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once

#ifdef WIN32
#include <windows.h>
#include <wingdi.h>
#endif

#include <GL/gl.h>
#include "GL/glext.h"

#ifdef WIN32
#include "GL/wglext.h"
#endif

namespace P3D
{

void LoadGLExt();
bool IsExtensionSupported(const char* szTargetExtension);

extern PFNGLGENBUFFERSARBPROC glGenBuffersARB;
extern PFNGLBINDBUFFERARBPROC glBindBufferARB;
extern PFNGLBUFFERDATAARBPROC glBufferDataARB;
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;
extern PFNGLMAPBUFFERARBPROC glMapBufferARB;
extern PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB;
extern PFNGLBLENDEQUATIONPROC glBlendEquation;
extern PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2DARB;
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC glCompressedTexSubImage2DARB;
extern PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB;

//Requires sm4.0
extern PFNGLDRAWARRAYSINSTANCEDEXTPROC glDrawArraysInstancedEXT;


}; // namespace
