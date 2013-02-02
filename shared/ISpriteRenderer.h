/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/

#pragma once
#include "interface.h"


namespace P3D
{

	class ISpriteRenderer
	{
	public:
		virtual bool Create()=0;
		virtual void Render()=0;
		virtual const Matrix* GetMatrix()=0;
	};

}