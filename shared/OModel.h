/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: am!go, kexik
*/

#pragma once
#include "ICamera.h"
#include "IMesh.h"
#include "ITexture.h"

#include "BaseObject.h"

#include "BufferRequisities.h"

#include "OMaterial.h"

#include <vector>

namespace P3D
{

	/// Subset of mesh which use the same material
	struct MeshSubset : public BaseObject
	{
	public:
		MeshSubset(){ 
			//REGISTER_OBJECT(MeshSubset); 
			REGISTER_PARAM(StartIndex, "Starting index from index buffer for this subset");
			//REGISTER_PARAM(StartVertex, "Starting vertex index from vertex buffers for this subset");
			REGISTER_PARAM(NumTriangles, "Number of triangles in this subset");

			REGISTER_PARAM(MaterialName, "Material for this subset");
			REGISTER_PARAM(MaterialInst, "Material instance data");

			materialPtr = 0; // mat. not loaded
		};

		IntParam StartIndex;
		//IntParam StartVertex;
		IntParam NumTriangles;
		StringParam MaterialName;
		MaterialInstance MaterialInst;

		// --- internal
		Material *materialPtr;
	};

	/// One vertex data stream (i.e. position or normal)
	struct MeshVertexData : BaseObject
	{
	public:
		MeshVertexData(){ 
			//REGISTER_OBJECT(MeshVertexData); 
				Usage.SetEx(_W("P3DVU_NONE"));
			REGISTER_PARAM(Usage, "Type of this vertex data");
				UsageIndex = 0;
			REGISTER_PARAM(UsageIndex, "Index of usage stream");
			REGISTER_PARAM(DataSize, "Vertex data size in float count");
			REGISTER_PARAM(Float1, "Float vertex data");
			REGISTER_PARAM(Float2, "Float2 vertex data");
			REGISTER_PARAM(Float3, "Float3 vertex data");
			REGISTER_PARAM(Float4, "Float4 vertex data");
		};

		StringParam Usage; //vertex usage type (positions, normals, ...)
		IntParam UsageIndex;
		IntParam DataSize; // in number of floats
		ArrayParam<FloatParam> Float1;
		ArrayParam<Vec2Param> Float2;
		ArrayParam<Vec3Param> Float3;
		ArrayParam<Vec4Param> Float4;
	};

	/// Structure containing all mesh vertex data like positions, normals, binormals and subsets
	struct MeshData : public BaseObject
	{
	public:
		MeshData(){ 
			//REGISTER_OBJECT(MeshData);
			REGISTER_PARAM(Subsets,"Mesh subsets");
			REGISTER_PARAM(DataStreams,"Mesh data streams");
			REGISTER_PARAM(Indices,"Mesh indices");

			REGISTER_PARAM(NumIndices,"Number of indices");
			REGISTER_PARAM(NumVerts,"Number of vertices");
		};

		ArrayParam<MeshSubset> Subsets;
		ArrayParam<MeshVertexData> DataStreams;
		ArrayParam<IntParam> Indices;

		IntParam NumIndices;
		IntParam NumVerts;

		// - private data for caching
		unsigned int vbId;
		unsigned int ibId;
	};

	/// Geometry chunk used for actual drawing
	struct GeometryChunk
	{
		MeshSubset* Subset;
		MeshData* Data;
		Matrix* WorldMatrix;
	};

	class Model : public BaseObject
	{
	public:
		/*bool Create(IMesh *mesh, Vec3 position, Vec3 rotation, Vec3 scale);
		void Render(unsigned int subset);
		ITexture *GetTexture(unsigned int subset);
		void SetTexture(unsigned int subset, ITexture* texture);
		const Matrix*  GetMatrix();
		const Matrix* GetPosRotMatrix();
		inline unsigned int GetNumSubsets() { return m_numSubsets; };
		void SetPosRotMatrix(const Matrix &mat);
		AABB GetAABB();*/
		/// Get mesh at 
		//MeshData& GetLODMesh(unsigned int level);
	public:
		ArrayParam<MeshData> LODs;
	};

};
