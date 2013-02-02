/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#include "precompiled.h"

#ifdef _OGL

#include "glex.h"
#include "../Renderer.h"

#ifdef LINUX
#include <GL/glx.h>
#include <GL/glxext.h>
#endif

namespace P3D
{

PFNGLGENBUFFERSARBPROC glGenBuffersARB = NULL;
PFNGLBINDBUFFERARBPROC glBindBufferARB = NULL;
PFNGLBUFFERDATAARBPROC glBufferDataARB = NULL;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = NULL;
PFNGLMAPBUFFERARBPROC glMapBufferARB = NULL;
PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB = NULL;
PFNGLBLENDEQUATIONPROC glBlendEquation = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2DARB = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC glCompressedTexSubImage2DARB = NULL;
PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB = NULL;

//Requires sm4.0
PFNGLDRAWARRAYSINSTANCEDEXTPROC glDrawArraysInstancedEXT = NULL;

//-----------------------
inline void* glGetProcAddress(const char* s)
{
#if defined(WIN32)
	return wglGetProcAddress(s);
#elif defined (LINUX)
	return (void*)(glXGetProcAddressARB(reinterpret_cast<const GLubyte*>(s))); // ARB???
#endif
}


bool IsExtensionSupported(const char* szTargetExtension)
{
	const char *pszExtensions = NULL;
	const char *pszStart;
	const char *pszWhere, *pszTerminator;

	pszWhere = (const char *)strchr(szTargetExtension, ' ');
	if (pszWhere || *szTargetExtension == '\0')
		return false;

	pszExtensions = (const char*)glGetString(GL_EXTENSIONS);
	pszStart = pszExtensions;

	for (;;)
	{
		pszWhere = (const char *) strstr((const char *) pszStart, szTargetExtension);
		if (!pszWhere) break;

		pszTerminator = pszWhere + strlen(szTargetExtension);
		if (pszWhere == pszStart || *(pszWhere - 1) == ' ')
		{
			if (*pszTerminator == ' ' || *pszTerminator == '\0')
				return true;
		}
		pszStart = pszTerminator;
	}
	return false;// Nepodporováno
}


//-----------------------------
void LoadGLExt()
{
	// ****** VBO **************************************************************
	if(!IsExtensionSupported("GL_ARB_vertex_buffer_object"))
		CON(MSG_ERR_FATAL, _W("Vertex buffer object not supported! Consider upgrading graphic card driver!"));

	glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) glGetProcAddress("glGenBuffersARB");
	glBindBufferARB = (PFNGLBINDBUFFERARBPROC) glGetProcAddress("glBindBufferARB");
	glBufferDataARB = (PFNGLBUFFERDATAARBPROC) glGetProcAddress("glBufferDataARB");
	glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) glGetProcAddress("glDeleteBuffersARB");
	glMapBufferARB = (PFNGLMAPBUFFERARBPROC) glGetProcAddress("glMapBufferARB");
	glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC) glGetProcAddress("glUnmapBufferARB");

	glBlendEquation = (PFNGLBLENDEQUATIONPROC) glGetProcAddress("glBlendEquation");
	if (!glBlendEquation) CON(MSG_ERR_FATAL, _W("Extension Blend equation extension cannot be loaded!") );

	glCompressedTexImage2DARB = (PFNGLCOMPRESSEDTEXIMAGE2DARBPROC) glGetProcAddress("glCompressedTexImage2DARB");
	if (!glCompressedTexImage2DARB) CON(MSG_ERR_FATAL, _W("Extension glCompressedTexImage2DARB cannot be loaded!") );

	glCompressedTexSubImage2DARB = (PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC) glGetProcAddress("glCompressedTexSubImage2DARB");
	if (!glCompressedTexSubImage2DARB) CON(MSG_ERR_FATAL, _W("Extension glCompressedTexSubImage2DARB cannot be loaded!") );

	glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC) glGetProcAddress("glBufferSubDataARB");
	if (!glBufferSubDataARB) CON(MSG_ERR_FATAL, _W("Extension glBufferSubDataARB cannot be loaded!") );

	glDrawArraysInstancedEXT = (PFNGLDRAWARRAYSINSTANCEDEXTPROC) glGetProcAddress("glDrawArraysInstancedEXT");
	if (!glDrawArraysInstancedEXT) CON(MSG_ERR, _W("Extension glDrawArraysInstancedEXT cannot be loaded!") );

}

}; // namespace

#endif