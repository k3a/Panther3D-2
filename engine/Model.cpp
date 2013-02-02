/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go, kexik
*/
#include "precompiled.h"

#include "Engine.h"
#include "OModel.h"

namespace P3D
{
	class CModel : public Model
	{
	public:
		// ----------------------------------------------
		CModel()
		{
			REGISTER_PARAM(LODs, "Model LOD levels"); // TODO: move to Model class?
		}
		
		// ----------------------------------------------
		~CModel()
		{
		}

		// ----------------------------------------------
		bool PreCache()
		{
			IGraphicsManager* gm = CEngine::instance()->mRenderer()->iGraphicsManager();

			bool res=true;
			for (unsigned int i=0; i<LODs.Size(); i++)
			{
				// -- load materials for all subsets
				for ( unsigned int si = 0; si < LODs[i].Subsets.Size(); si++ )
				{
					// create new material object for this subset
					Material* mat = (Material*)CEngine::cObjectManager()->CreateObject( _W("Material") );
					if ( LODs[i].Subsets[si].MaterialName.GetNumChars()==0 )
					{
						// subset don't have material assigned
						//mat->Load(CEngine::instance()->mFilesystem(), "materials/default.mat")
					}
					else
					{
						// load material from file LODs[i].Subsets[si].MaterialName
					}
					LODs[i].Subsets[si].materialPtr = mat;
				}

				// -- precache mesh data (materials, material instances, vertex/index buffers, ...)
				if ( !gm->CacheMeshData( LODs[i] ) ) res = false;
			}


			return res;
		}
	};

	REGISTER_OBJECT(CModel);

};
