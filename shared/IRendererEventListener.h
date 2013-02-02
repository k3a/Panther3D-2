/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once

/// Event listener
/** Event listener is user-implemented interface. It is useful for gathering information
about various engine core module events like "map loading" "finished map loading". 
This is a good way how to catch device lost state or current render state.<br><br>
User of this interface can implement only needed methods.*/
class IRendererEventListener
{
public:
	/// This will be triggered few times with description of current action
	virtual void RendererLoading(const wchar* caption){};
};