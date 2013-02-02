/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once
#include "interface.h"
#include "IConsole.h"
#include "ITexture.h"

namespace P3D
{

	//Renders some basic geometric objects.
	class IPrimitiveRenderer
	{
	public:
		virtual bool Init()=0;
		/** Add a line in screen coordinates */
		virtual void AddLine(Vec2i pos1, Vec2i pos2, ColorRGBA color)=0;
		/** Add a rectangle in screen coordinates */
		virtual void AddRectangle(Vec2i minPos, Vec2i maxPos, ColorRGBA color, ITexture* texture = 0)=0;
		/** Add a line in projection coordinates */
		virtual void AddLineRel(Vec2 pos1, Vec2 pos2, ColorRGBA color)=0;
		/** Add a rectangle in projection coordinates */
		virtual void AddRectangleRel(Vec2 minPos, Vec2 maxPos, ColorRGBA color, ITexture* texture = 0)=0;
		/** Render buffered geometry and frees it */
		virtual void Render()=0;

		/** Add a sequence of connected lines to render */
		virtual void AddLines3D(const Vec3 *points, UINT numPoints, const ColorRGBA &color)=0;

		/** Add a quad to render */
		virtual void AddQuad3D(const Vec3 &pos1, const Vec3 &pos2, ColorRGBA &color, ITexture *texture = 0)=0;

	};

};
