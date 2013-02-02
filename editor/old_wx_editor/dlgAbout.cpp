/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/
#include "precompiled.h"

#include "dlgAbout.h"

namespace P3D
{

	dlgAbout::dlgAbout( wxWindow* parent )
	:
	dlgAboutGui( parent )
	{

	}

	void dlgAbout::OnClose( wxCommandEvent& event )
	{
		this->Destroy();
		//delete this;
	}

}