/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"


#ifdef _OGL

#include "GLHelperFunctions.h"
#include "symbols.h"
#include "../Renderer.h"


namespace P3D
{

	bool CheckGLErrors(const wchar *info)
	{
		GLenum error;
		while(true) 
		{
			error = glGetError();
			if(error == GL_NO_ERROR)
			{
				//CON(MSG_INFO, " ...no errors");
				return false; //You're lucky
			}

			if(error == GL_INVALID_ENUM) CON(MSG_ERR, _W("%s: The enum argument is out of range!"), info);
			else if(error == GL_INVALID_VALUE) CON(MSG_ERR, _W("%s: The numeric argument is out of range!"), info);
			else if(error == GL_INVALID_OPERATION) CON(MSG_ERR, _W("%s: The operation is illegal in its current state!"), info);
			else if(error == GL_STACK_OVERFLOW) CON(MSG_ERR, _W("%s: The command would cause a stack overflow!"), info);
			else if(error == GL_STACK_UNDERFLOW) CON(MSG_ERR, _W("%s: The command would cause a stack underflow!"), info);
			else if(error == GL_OUT_OF_MEMORY) CON(MSG_ERR, _W("%s: Not enough memory is left to execute the command!"), info);
			else if(error == GL_TABLE_TOO_LARGE) CON(MSG_ERR, _W("%s: The specified table is too large!"), info);
			return true;
		}
	}

	void CheckFramebufferStatus()
	{
		GLenum status;
		status = (GLenum) glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		switch(status) {
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			printf("Unsupported framebuffer format\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			printf("Framebuffer incomplete, missing attachment\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT:
			printf("Framebuffer incomplete, duplicate attachment\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			printf("Framebuffer incomplete, attached images must have same dimensions\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			printf("Framebuffer incomplete, attached images must have same format\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			printf("Framebuffer incomplete, missing draw buffer\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			printf("Framebuffer incomplete, missing read buffer\n");
			break;
		default:
			assert(0);
		}
	}

}

#endif