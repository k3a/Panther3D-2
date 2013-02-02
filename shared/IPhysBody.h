/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go
*/
#pragma once
#include "interface.h"
#include "Matrix.h"

namespace P3D
{

	///Type of a physics shape
	enum ePhysShape
	{
		PHYSS_NONE,
		PHYSS_CUBE,
		PHYSS_SPHERE,
		PHYSS_CAPSULE,
		PHYSS_CONVEX,
		PHYSS_TRIANGLE
	};

	///All the Shape descriptors are derived from this struct.
	struct sPhysShapeDesc
	{
		///Local position of the shape.
		Vec3 position;
		///Describes a kind of the shape.
		ePhysShape shape;

	protected:
		sPhysShapeDesc()
		{
			shape = PHYSS_NONE;
			position = Vec3(0.0f, 0.0f, 0.0f);
		}
	};

	///A Box shape description.
	struct sBoxShapeDesc : public sPhysShapeDesc
	{
		sBoxShapeDesc()
		{
			shape = PHYSS_CUBE;
			size = Vec3(1.0f, 1.0f, 1.0f);
		}
		
		Vec3 size;
	};

	///A sphere shape description.
	struct sSphereShapeDesc : public sPhysShapeDesc
	{
		sSphereShapeDesc()
		{
			shape = PHYSS_SPHERE;
			radius = 1.0f;
		}

		float radius;
	};

	///A capsule shape description.
	struct sCapsuleShapeDesc : public sPhysShapeDesc
	{
		sCapsuleShapeDesc()
		{
			shape = PHYSS_CAPSULE;
			radius = 1.0f;
			height = 2.0f;
		}

		float radius;
		float height;
	};

	///a convex shape description.
	struct sConvexShapeDesc : public sPhysShapeDesc
	{
		sConvexShapeDesc()
		{
			shape = PHYSS_CAPSULE;
			numVertices = 0;
			vertices = 0;
		}
		unsigned int numVertices;
		Vec3 *vertices;
	};

	///a convex shape description.
	struct sTriangleMeshShapeDesc : public sPhysShapeDesc
	{
		sTriangleMeshShapeDesc()
		{
			shape = PHYSS_TRIANGLE;
			numVertices = 0;
			vertices = 0;
		}
		unsigned int numVertices;
		Vec3 *vertices;
		unsigned int numIndices;
		Vec3 *indices;
	};
	
	///Defines a shape for the physics module.
	class IPhysBody : public IBaseInterface
	{
	public :

		///Defines if the body is kinematic.
		virtual void SetKinematic(bool kinematic)=0;
		///Returns the transformation matrix.
		virtual const Matrix* GetMatrix()=0;
		///Not working yet.
		virtual bool Changed()=0;
		///Erases all the previosly created shapes and defines a new one.
		virtual bool SetShape(sPhysShapeDesc *desc)=0;
		///Adds a new shape to the body.
		virtual bool AddShape(sPhysShapeDesc *desc)=0;

	protected:
		///It is forbidden to release this object using the delete keyword. Use the appropriate manager's method instead.
		~IPhysBody(){};
	};
}
