/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
#pragma once
#include "interface.h"
#include "IRenderableMaterial.h"
#include "types.h"

namespace P3D
{

class IRenderableObject : public IBaseInterface
{
public:
	/**
	\brief Sets transformation (matrix).

	Sets transformation of this object. It includes position, rotation and scale.

	\param mat Matrix to set.
	*/
	virtual void SetMatrix(Matrix &mat)=0;

	// TODO: SetPosition, SetRotation, SetScale -> modify object's internal matrix

	/**
	\brief Renders object subset.

	Renders part (subset) of object. Every subset have assigned material on it.

	\param subsetID Subset of object to be drawn.
	*/
	virtual void DrawSubset(DWORD subsetID)=0;

	/**
	\brief Assigns material to subset.

	Assigns previously created material to desired subset.

	\param subsetID Subset of object to assign material to.
	\param pMaterial Material pointer to assign.
	*/
	// KEX: NEMAM ESTE JASNO VE ZPUSOBU MANIPULACE S MATERIALY A PRIRAZOVANI...
	virtual void AssignMaterial(UINT subsetID, IRenderableMaterial* pMaterial)=0;


	/**
	\brief Retrieve internal mesh id.

	Retrieve internal mesh id used by resource manager. It can be easily converted
	to CMesh using CResourceManager::GetMesh();

	\return Internal mesh id number.
	\see CResourceManager::GetMesh()
	*/
	virtual DWORD GetMesh()=0;
};

};
