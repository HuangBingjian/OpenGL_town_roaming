#ifndef _MDLMODEL_H_
#define _MDLMODEL_H_

#include <windows.h>

#include "MdlFormat.h"

class TMDLModel
{
//  protected:
  public:
	vec3_t			Origin;					// Origin
	vec3_t			Rotation;				// Rotation
	long			CurrentSequence;		// Current sequence
	float			CurrentFrame;			// Current frame
	long			CurrentBodyPart;		// Current body part
	long			CurrentSkin;			// Current skin
	BYTE			Controller[4];			// Controllers
	BYTE			Blending[2];			// Animation blending
	BYTE			MouthPosition;			// Mouth position
	tagMDLHeader	*Header;				// Model header
	tagMDLModel		*Model;					// Model
	tagMDLHeader	*TextureHeader;			// Texture
	tagMDLSeqHeader	*AnimationHeader[32];	// Animation header
		
  public:
	void Init(char *Filename);		//
	void DrawModel();				//
	void AdvanceFrame(float Time);	//
	long SetSequence(long Sequence);//

	//ÐÂ½Ó¿Ú
	void AdvanceFrame2();	//

  protected:
	void ExtractBoundBox(float *Minimums, float *Maximums);
	
	long GetSequence();
	
	void GetSequenceInfo(float *FrameRate, float *GroundSpeed);
	
	float SetController(long ControllerIndex, float Value);
	
	float SetMouth(float Value);
	
	float SetBlending(long Blender, float Value);
	
	long SetBodyGroup(long Group, long Value);
	
	long SetSkin(long Value);

/////////////////////////////////////////////////////////////////////////////////////
//  protected:
	tagMDLHeader *LoadModel(char *Filename);

	tagMDLSeqHeader *LoadDemandSequences(char *Filename);

	void CalcBoneQuaternion(long Frame, float Value, tagMDLBone *Bone, tagAnimation *Anim, 
		float *Q);

	void CalcBonePosition(long Frame, float Value, tagMDLBone *Bone, tagAnimation *Anim, 
		float *Pos);

	void CalcRotations (vec3_t *Pos, vec4_t *Q, tagMDLSeqDescription *SeqDescription, 
		tagAnimation *Anim, float FrameValue);

	tagAnimation *GetAnim(tagMDLSeqDescription *SeqDescription);

	void SlerpBones(vec4_t Q1[], vec3_t Pos1[], vec4_t Q2[], vec3_t Pos2[], float Value);
	
	void SetUpBones();
	
	void DrawPoints();
	
	void Lighting(float *lv, long Bone, long Flags, vec3_t Normal);
	
	void SetupLighting();
	
	void SetupModel(long BodyPart);
	
	void UploadTexture(tagMDLTexture *Texture, BYTE *Data, BYTE *Palette);

};

#endif