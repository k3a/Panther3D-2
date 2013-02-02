/* Panther3D Engine © 2004-2009 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
Authors: kexik
*/

#include "types.h"
#include "symbols.h"

#include <vector>
#include <stack>

namespace P3D {

/// Hierarchy node of kd-tree spacial hierarchy
/** Class representing a node in a tree hierarchy. A node has two children. 
	The node can be either an interior node (i.e., left and right child NULL) 
	or a leaf node, which holds the actual geometry. */
class CHierarchyNode
{
public:
/*	typedef stack<CHierarchyNode *> TraversalStack;
    typedef vector<BaseEntity *> GeometryList;
*/
	CHierarchyNode();
	~CHierarchyNode();
/*
	//! was this node visible the last time it was visited?
	bool Visible();
	//! last time this node was visited (in framenumber)
	int LastVisited();
	//! sets visible flag
	void SetVisible(bool visible);
	//! sets timestamp (current framenumber)
	void SetLastVisited(int lastVisited);
	//! is this node a leaf node (i.e., geometry)
	bool IsLeaf();
	//! renders the geometry in this node. returns number of rendered geometry
	int Render();
	//! returns occlusion query id
	int GetOcclusionQuery();
	//! sets occlusion query id
	void SetOcclusionQuery(int occlusionQuery);
	//! renders the bounding volume  (i.e., a axis aligned bounding box) of the node
	void RenderBoundingVolume();
	//! adds geometry to this node (rendered if this node is a leaf)
	void AddGeometry(Geometry *geometry);

	void SetLeftChild(HierarchyNode *child);
	void SetRightChild(HierarchyNode *child);

	HierarchyNode *GetParent();
    HierarchyNode *GetLeftChild();
	HierarchyNode *GetRightChild();

	//! compute bounding volume (i.e., a axis aligned bounding box) for this geometry.
	//void CalcBoundingVolume();
	const AABox& GetBoundingVolume();
	
	//! generates the kd-tree from this root-node, returns number of nodes in hierarchy.
	int	GenerateKdTree();

	//! returns number of nodes in hierarchy with this node.
	int GetNumHierarchyNodes();

	//! add children of this node ordered to a traversal stack with respect to the view point
	void PushChildrenOrdered(const Vector3 viewpoint, TraversalStack &traversalStack);

	//! if this node was culled, set the type of culling (frustum, query)
	void SetCulledType(int culledtype);
	//! returns type of culling
	int GetCulledType();
	//! set gl state to visible render bounding volume
	void RenderBoundingVolumeForVisualization();
	//! returns geometry list
	GeometryList &GetGeometry();

	//! visibly renders also bounding volume of this node
	static void SetRenderBoundingVolume(bool renderBoundingVolume);
	*/
	/**
		initialises the k-d tree and the static termination criteria.
		this method must be called after all geometry has been added to the root node
		and before the tree has been generated.
	*/
/*	static void InitKdTree(HierarchyNode *root);

	float GetDistance();
	void SetDistance(float distance);

	float mDistance;

protected:

	//! generates hierarchynode with specified upper and lower bounding volume points on specified depth
	HierarchyNode(const Vector3 boundLower, const Vector3 boundUpper, 
				 HierarchyNode *parent, int depth);

	enum {X_AXIS, Y_AXIS, Z_AXIS};

	//! criteria to stop the splitting of this kd-tree node
	bool SimpleEnough();
	//! split plane of the kd-tree (according to mSplitAxis, it is the x, y, or z axis)
	float ComputeSplitPlane();*/

	/** calculates a value that expresses if it's reasonable to split the 
	   node at the specified position (the smaller the value the better the split)
	   */
	/*float ComputeHeuristics(float pos);

	GeometryList mGeometry;
	//! id of this occlusion query of this node
	int mOcclusionQuery;

	HierarchyNode *mLeftChild;	
	HierarchyNode *mRightChild;
	HierarchyNode *mParent;
	
	bool  mVisible;
	int mLastVisited;
	bool mAABValid;
	bool mEnclosedSpaceValid;
	int mNumHierarchyNodes;
	
	AABox mBoundingBox;
	// the enclosed space of the pure kd-treenode (without considering the geometry)
	AABox mEnclosedSpace;
	
	int mSplitAxis;
	int mLastRendered;
	float mSplitValue;
	int mDepth;

	static bool sRenderBoundingVolume;
	static float sSplitBandwith;

	// --termination criteria

	static int sGeometryThreshold;
	// the maximum bounding box surface
	static float HierarchyNode::sSurfaceThreshold;
	// the maximum tree depth
	static int HierarchyNode::sMaxDepth;
	// the default bounding box drawing color
	float mBoxColor[3];
	// distance to the view point*/
};

}; // ns