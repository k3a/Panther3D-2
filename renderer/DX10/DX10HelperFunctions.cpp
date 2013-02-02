/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#include "precompiled.h"


#if defined(_DX) && _DX == 10

#include "symbols.h"
#include "DX10HelperFUnctions.h"
#include "../Renderer.h"

namespace P3D
{
	
	bool CheckHRResult(HRESULT hr)
	{

		switch(hr)
		{
		case S_OK:
			return true;
			break;
		case D3D10_ERROR_FILE_NOT_FOUND:
			CON(MSG_ERR, _W("HRESULT: The file was not found."));
			return false;
			break;
		case D3D10_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
			CON(MSG_ERR, _W("HRESULT: There are too many unique instances of a particular type of state object."));
			return false;
			break;
		case D3DERR_INVALIDCALL:
			CON(MSG_ERR, _W("HRESULT: The method call is invalid. For example, a method's parameter may not be a valid pointer."));
			return false;
			break;
		case D3DERR_WASSTILLDRAWING:
			CON(MSG_ERR, _W("HRESULT: The previous blit operation that is transferring information to or from this surface is incomplete."));
			return false;
			break;
		case E_FAIL:
			CON(MSG_ERR, _W("HRESULT: Attempted to create a device with the debug layer enabled and the layer is not installed."));
			return false;
			break;
		case E_INVALIDARG:
			CON(MSG_ERR, _W("HRESULT: An invalid parameter was passed to the returning function."));
			return false;
			break;
		case E_OUTOFMEMORY:
			CON(MSG_ERR, _W("HRESULT: Direct3D could not allocate sufficient memory to complete the call."));
			return false;
			break;
		case S_FALSE:
			CON(MSG_ERR, _W("HRESULT: Alternate success value, indicating a successful but nonstandard completion (the precise meaning depends on context)."));
			return true;
			break;
		default:
			CON(MSG_ERR, _W("HRESULT: Unknown HRESULT value."));
			return false;
		}
		
	}

}

#endif
