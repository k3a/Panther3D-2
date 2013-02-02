/* Panther3D Engine © 2004-2008 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kex1k
*/
// Reversity Mesh File Format

#pragma once
#include "types.h"
#include "Vector.h"

namespace P3D {

// *** CONFIGURATION
#define RM_MAJOR 3
#define RM_MINOR 0

/// Number of categories. \see RMHeader
#define RM_NUM_PARTS 7

/// Descriptor of one part of file.
struct sRMPart
{
	/// Offset of the data from the beginning of file
	long offset;
	/// Length of data. Can be x times longer than given structure.
	long length;
};

/// Reversity Mesh file header.
struct sRMHeader
{
	/// File identifier. Must be "RM".
	char magic[2];
	/// Major version of file format. All files with the same major are compatible.
	BYTE major;
	/// Minor version. After using some previously "reserved" field, this number should be increased.
	BYTE minor;
	/// Contents of file in parts. Each category can contain subsequently several same struct data.
	sRMPart contents[RM_NUM_PARTS];
};

/// One file information structure. \see sRMInfo
#define RM_FILE_INFO		0
/// One mesh subset. \see sRMSubset
#define RM_SUBSETS		1
/// Vertices described by vertex structure. \see sRMVertex
#define RM_VERTICES		2
/// Index buffer with 32bit indices. Without structure.
#define RM_INDICES		3
/// Bones making hierarchy. \see sRMBone
#define RM_BONES		4
/// Animations in this mesh. \see sRMAnimation
#define RM_ANIMATIONS		5
/// Animation frames for all bones in this mesh. \see sRMBoneAnimation
#define RM_BONE_ANIMATIONS	6

/// Mesh information structure. Can contain import path and setting. Longer strings are cut.
struct sRMInfo
{
	/// Long path to file, from which was this model imported and created. Useful for reimport.
	char importpath[512];
	/// Username of person who imported this mesh. :-P
	char username[32];
	/// DateTime stamp of file creation in string format YYYYMMDDHHMMSS
	char created[16];
	/// DateTime stamp of last (re)import in string format YYYYMMDDHHMMSS
	char imported[16];

	/// Keywords useful for searching
	char keywords[64];

	//- importer settings:

	/// Generate new normals. Normals in source file will not be used. You can generate new smooth normals ("ismoothnormals").
	bool igennewnormals;
	/// If generate new normals "igennewnormals" is true, it will or will not generate smooth normals.
	bool ismoothnormals;
	/// Don't import animation data? If true, only static mesh data and bone hierarchy will be imported.
	bool idontimportanim;
	/// Reorder triangles and optimize mesh? Slower to import but better at run-time rendering.
	bool ioptimize;
	/// Invert all infacing normal vectors? Not good for planar surfaces, but works in most cases.
	bool ifixinfacingnormals;

	bool reserved1;
	bool reserved2;
	bool reserved3;
};

/// Mesh subset. Mesh can contain one or more subsets, which are separate vertex and index buffers with one texture
struct sRMSubset
{
	char texture[256];
	unsigned int numvertices;
	unsigned int firstindex;
	unsigned int numindices;
	/// Has this subset texture coordinates?
	bool bTexCoords;
};

/// Vertex structure, 77 bytes all vectors in object space
struct sRMVertex
{
	/// Position of this vertex
	Vec3 position;
	/// UV texture coordinates
	Vec2 uv;
	/// Normal vector of this vertex
	Vec3 normal;
	/// Tangent vector of this vertex
	Vec3 tangent;
	/// Bitangent (incorrectly speaking binormal) vector of this vertex
	Vec3 bitangent;
	/// The strength of influence by bones in range [0,1]. Sum of all influences in one vertex must be 1.
	float weights[4];
	BYTE bones[4];
	BYTE numbones; // number of bones affecting this vertex [0-4]
};

/// Index
typedef unsigned int sRMIndex;

/// Bone
struct sRMBone
{
	/// Bone name
	char name[32];
	/// Position of bone relative to parent
	Vec3 position;
	/// Normalized quaternion relative to parent
	Vec3 orientation;

	/// First child bone id, level down
	BYTE firstchild;
	/// Next bone at the same level
	BYTE nextsibling;
};

/// Animation. Bone animations defined with startid and endid can contain animation data for several bones;
struct sRMAnimation
{
	/// Animation name
	char name[32];
	/// Animation length in ticks
	float length;
	/// Number of all bones affected by this animation. All these bones will be defined in bone animations.
	BYTE numbones;
	/// Beginning bone animation stream ID
	unsigned int firstboneanim;
	/// Number of bone animations
	unsigned int numboneanims;
};

/// Bone animation. These are new position and orientation data for one bone.
struct sRMBoneAnimation
{
	/// Time of this bone animation
	float time;
	/// ID of bone which is this bone animation for
	BYTE boneid;
	/// Position of bone relative to bone parent
	Vec3 position;
	/// Normalized quaternion relative to bone parent
	Vec3 orientation;
};

}; // namespace