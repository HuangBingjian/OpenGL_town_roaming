#ifndef _MDLFORMAT_H_
#define _MDLFORMAT_H_

#define	MAXVERTICES		2048
#define	MAXBONES		128

#include "MathLib.h"

// Lighting defines
#define LIGHT_FLATSHADE		0x0001
#define LIGHT_FULLBRIGHT	0x0004

// Transition defines
#define	TRANSITION_X		0x0001
#define	TRANSITION_Y		0x0002	
#define	TRANSITION_Z		0x0004
#define	TRANSITION_XR		0x0008
#define	TRANSITION_YR		0x0010
#define	TRANSITION_ZR		0x0020
#define	TRANSITION_TYPES	0x7FFF
#define	TRANSITION_RLOOP	0x8000

struct tagMDLHeader
{
	long	ID;						// File id
	long	Version;				// File version
	char	Name[64];				// Model name
	long	Length;					// File length
	vec3_t	EyePosition;			// Eye position
	vec3_t	Minimum;				// Ideal movement hull size
	vec3_t	Maximum;	
	vec3_t	BoundingBoxMinimum;		// Bounding box
	vec3_t	BoundingBoxMaximum;		
	long	Flags;					// Flags
	long	NumBones;				// Number of bones
	long	BoneOffset;				// Offset to bone data
	long	NumBoneControllers;		// Number of bone controllers
	long	BoneControllerOffset;	// Offset to bone controller data
	long	NumHitBoxes;			// Complex bounding boxes
	long	HitBoxOffset;				
	long	NumSequences;			// Animation sequences
	long	SequenceOffset;			//动作数
	long	NumSeqGroups;			// Demand loaded sequences
	long	SeqGroupOffset;
	long	NumTextures;			// Number of textures
	long	TextureOffset;			// File texture offset
	long	TextureData;			// Texture data
	long	NumSkinReferences;		// Replaceable textures
	long	NumSkinFamilies;
	long	SkinOffset;
	long	NumBodyParts;		
	long	BodyPartOffset;
	long	NumAttachments;			// Queryable attachable points
	long	AttachmentOffset;
	long	SoundTable;
	long	SoundOffset;
	long	NumSoundGroups;
	long	SoundGroupOffset;
	long	NumTransitions;			// Animation node to animation node transition graph
	long	TransitionOffset;
};

struct tagMDLCacheUser
{
	void	*Data;
};

struct tagMDLSeqHeader
{
	long	ID;
	long	Version;
	char	Name[64];
	long	Length;
};

struct tagMDLSeqGroup
{
	char			Label[32];		// Textual name
	char			Name[64];		// Filename
	tagMDLCacheUser	Cache;			// Cache index pointer
	long			Data;			// Hack for group 0
};

struct tagMDLSeqDescription
{
	char	Name[32];				// Sequence label
	float	Timing;					// Frame time
	long	Flags;					// Flags
	long	Activity;
	long	Actweight;
	long	NumEvents;
	long	EventIndex;
	long	NumFrames;				//每个动作的数
	long	NumPivots;				// Number of foot pivots
	long	PivotOffset;
	long	MotionType;	
	long	MotionBone;
	vec3_t	LinearMovement;
	long	AutoMovePosIndex;
	long	AutoMoveAngleIndex;
	vec3_t	BoundingBoxMinimum;		// Bounding box
	vec3_t	BoundingBoxMaximum;		
	long	NumBlends;
	long	AnimOffset;				// mstudioanim_t pointer relative to start of sequence group 
									// data[blend][bone][X, Y, Z, XR, YR, ZR]
	long	BlendType[2];			// X, Y, Z, XR, YR, ZR
	float	BlendStart[2];			// Starting value
	float	BlendEnd[2];			// Ending value
	long	BlendParent;
	long	SeqGroup;				// Sequence group for demand loading
	long	EntryNode;				// Transition node at entry
	long	ExitNode;				// Transition node at exit
	long	NodeFlags;				// Transition rules	
	long	NextSeq;				// Auto advancing sequences
};

struct tagMDLBone
{
	char	Name[32];				// Bone name for symbolic links
	long	Parent;					// Parent bone
	long	Flags;					// ??
	long	BoneController[6];		// Bone controller index, -1 == none
	float	Value[6];				// Default DoF values
	float	Scale[6];				// Scale for delta DoF values
};

struct tagMDLBoneController
{
	long	Bone;					// -1 == 0
	long	Type;					// X, Y, Z, XR, YR, ZR, M
	float	Start;
	float	End;
	long	Rest;					// Byte index value at rest
	long	Index;					// 0-3 user set controller, 4 mouth
};

struct tagAnimation
{
	WORD	Offset[6];
};

union tagMDLAnimFrame
{
	struct
	{
		BYTE	Valid;
		BYTE	Total;
	};

	short	Value;
};

struct tagMDLBodyPart
{
	char	Name[64];
	long	NumModels;
	long	Base;
	long	ModelOffset;			// Index into models array
};

struct tagMDLTexture
{
	char	Name[64];
	long	Flags;
	long	Width;
	long	Height;
	long	Index;
};

struct tagMDLModel
{
	char	Name[64];
	long	Type;
	float	BoundingRadius;
	long	NumMesh;
	long	MeshOffset;
	long	NumVertices;			// Number of unique vertices
	long	VertexInfoOffset;		// Vertex bone info
	long	VertexOffset;			// Vertex offset
	long	NumNormals;				// number of unique surface normals
	long	NormalInfoOffset;		// Normal bone info
	long	NormalOffset;			// Normal offset
	long	NumGroups;				// Deformation groups
	long	GroupOffset;
};

struct tagMDLMesh
{
	long	NumTriangles;
	long	TriangleOffset;
	long	SkinReference;
	long	NumNormals;				// Per mesh normals
	long	NormalOffset;			// Normal offset
};
struct ment_t//自定义取持枪位置结构
{	char				name[32];
	int					type;
	int					bone;
	vec3_t				org;	// attachment point
	vec3_t				vectors[3];
};
#endif
