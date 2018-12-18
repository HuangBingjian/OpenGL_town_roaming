// *************************************************************
// MDL是动态模型，3DS是静态模型，分清楚
// 上网找MDL查看器
// 初始化、渲染动作
// 看前3个函数：Init、AdvanceFrame2()、AdvanceFrame()
// *************************************************************

#include "..\header\stdafx.h"
#include "..\header\MdlModel.h"


DWORD	CurrentTexture	= 1;
vec3_t	TransformVertices[MAXVERTICES];	// Transformed vertices
vec3_t	LightValues[MAXVERTICES];		// Light surface normals
vec3_t	*TransformVertPtr;
vec3_t	*LightValuesPtr;
vec3_t	LightVector;					// Light vector in model reference frame
vec3_t	BoneLightVector[MAXBONES];		// Light vectors in bone reference frames
long	AmbientLightColor;				// Ambient world light
float	ShadeLight;						// Direct world light
vec3_t	LightColor;
float	BoneTransforms[MAXBONES][3][4];	// Bone transformation matrix

//初始化模型
void TMDLModel::Init(char *Filename)
{
	char	TextureName[256];
	char	SeqGroupName[256];

	Header	= LoadModel(Filename);

	if (0 == Header->NumTextures)
	{
		strcpy(TextureName, Filename);
//		strcpy(&TextureName[strlen(TextureName) - 4], "T.mdl");
		TextureHeader	= LoadModel(TextureName);
	}
	else
	{	
		TextureHeader	= Header;
	}

	if (Header->NumSeqGroups > 1)
	{
		for (long Loop = 1; Loop < Header->NumSeqGroups; Loop++)
		{
			strcpy(SeqGroupName, Filename);
			sprintf(&SeqGroupName[strlen(SeqGroupName) - 4], "%02d.mdl", Loop);
			AnimationHeader[Loop]	= LoadDemandSequences(SeqGroupName);
		}
	}
}

void TMDLModel::AdvanceFrame2()
{
	tagMDLSeqDescription	*SeqDescription	=
		(tagMDLSeqDescription *)((BYTE *)Header+Header->SequenceOffset) 
		+ CurrentSequence;

	CurrentFrame+=1;

	if (SeqDescription->NumFrames <= 1)
	{
		CurrentFrame = 0;
	}
	else
	{
		//当前动作序列帧数大于1，到达最后一帧后，当前帧置0		
		if ( CurrentFrame>=(SeqDescription->NumFrames - 1) )
		{
			CurrentFrame = 0;
		}
	}
}

void TMDLModel::AdvanceFrame(float Time)
{

	tagMDLSeqDescription	*SeqDescription	=
		(tagMDLSeqDescription *)((BYTE *)Header+Header->SequenceOffset) 
		+ CurrentSequence;

	if (Time > 0.1f)
	{
		Time	= 0.1f;
	}

	CurrentFrame	+= Time * SeqDescription->Timing;
	//正常持枪状态
	//Timing  : 16
	//NumFrames : 9
	if (SeqDescription->NumFrames <= 1)
	{
		CurrentFrame	= 0;
	}
	else
	{
		//如果当前动作序列帧数大于1，到达最后一帧后，当前帧置0
		//例如 NumFrames:9 CurrentFrame:8
		//限制表达式	(long)(CurrentFrame / (SeqDescription->NumFrames - 1))结果为1
		//例如 NumFrames:9 CurrentFrame:1 至 7
		//		(long)(CurrentFrame / (SeqDescription->NumFrames - 1))结果为0
		CurrentFrame	-= (long)(CurrentFrame / (SeqDescription->NumFrames - 1)) * 
			(SeqDescription->NumFrames - 1);
	}

}

void TMDLModel::ExtractBoundBox(float *Minimums, float *Maximums)
{
	tagMDLSeqDescription	*SeqDescription	= (tagMDLSeqDescription *)((BYTE *)Header + 
		Header->SequenceOffset);
	
	Minimums[0]	= SeqDescription[CurrentSequence].BoundingBoxMinimum[0];
	Minimums[1]	= SeqDescription[CurrentSequence].BoundingBoxMinimum[1];
	Minimums[2]	= SeqDescription[CurrentSequence].BoundingBoxMinimum[2];

	Maximums[0]	= SeqDescription[CurrentSequence].BoundingBoxMaximum[0];
	Maximums[1]	= SeqDescription[CurrentSequence].BoundingBoxMaximum[1];
	Maximums[2]	= SeqDescription[CurrentSequence].BoundingBoxMaximum[2];
}

//设置当前动作
long TMDLModel::SetSequence(long Sequence)
{
	if (Sequence > Header->NumSequences)
	{
		Sequence	= 0;
	}

	else if (Sequence < 0)
	{
		Sequence	= Header->NumSequences - 1;
	}

	CurrentSequence	= Sequence;
	CurrentFrame	= 0;

	return	CurrentSequence;
}

long TMDLModel::GetSequence()
{
	return	CurrentSequence;
}

void TMDLModel::GetSequenceInfo(float *FrameRate, float *GroundSpeed)
{
	tagMDLSeqDescription	*SeqDescription	= (tagMDLSeqDescription *)((BYTE *)Header + 
		Header->SequenceOffset) + CurrentSequence;

	if (SeqDescription->NumFrames > 1)
	{
		*FrameRate		= 256 * SeqDescription->Timing / (SeqDescription->NumFrames - 1);
		*GroundSpeed	= (float)sqrt(SeqDescription->LinearMovement[0] * 
			SeqDescription->LinearMovement[0] + SeqDescription->LinearMovement[1] * 
			SeqDescription->LinearMovement[1] + SeqDescription->LinearMovement[2] * 
			SeqDescription->LinearMovement[2]);

		*GroundSpeed	= *GroundSpeed * SeqDescription->Timing / (SeqDescription->NumFrames - 1);
	}
	
	else
	{
		*FrameRate		= 256.0f;
		*GroundSpeed	= 0.0f;
	}
}

float TMDLModel::SetController(long ControllerIndex, float Value)
{
	tagMDLBoneController	*BoneController	= 
		(tagMDLBoneController *)((BYTE *)Header + Header->BoneControllerOffset);
long Loop = 0;
	for (Loop = 0; Loop < Header->NumBoneControllers; Loop++, BoneController++)
	{
		if (BoneController->Index == ControllerIndex)
		{			
			break;
		}
	}
	
	if (Loop >= Header->NumBoneControllers)
	{
		return	Value;
	}

	if (BoneController->Type & (TRANSITION_XR | TRANSITION_YR | TRANSITION_ZR))
	{
		if (BoneController->End < BoneController->Start)
		{
			Value	= -Value;
		}
		
		if (BoneController->Start + 359.0f >= BoneController->End)
		{
			if (Value > ((BoneController->Start + BoneController->End) / 2.0f) + 180)
			{
				Value = Value - 360.0f;
			}

			if (Value < ((BoneController->Start + BoneController->End) / 2.0f) - 180)
			{				
				Value = Value + 360.0f;
			}
		}
		
		else
		{
			if (Value > 360.0f)
			{
				Value	= Value - (long)(Value / 360.0f) * 360.0f;
			}

			else if (Value < 0.0f)
			{
				Value	= Value + (long)((Value / -360.0f) + 1.0f) * 360.0f;
			}
		}
	}

	long	Setting	= (long)(255.0f * (Value - BoneController->Start) / (BoneController->End - 
		BoneController->Start));

	if (Setting < 0)
	{
		Setting	= 0;
	}
	
	else if (Setting > 255)
	{
		Setting	= 255;
	}

	Controller[ControllerIndex]	= (BYTE)Setting;

	return	Setting * (1.0f / 255.0f) * (BoneController->End - BoneController->Start) + 
		BoneController->Start;
}

float TMDLModel::SetMouth(float Value)
{
	tagMDLBoneController	*BoneController = 
		(tagMDLBoneController *)((BYTE *)Header + Header->BoneControllerOffset);

	for (long Loop = 0; Loop < Header->NumBoneControllers; Loop++, BoneController++)
	{
		if (4 == BoneController->Index)
		{
			break;
		}
	}

	if (BoneController->Type & (TRANSITION_XR | TRANSITION_YR | TRANSITION_ZR))
	{
		if (BoneController->End < BoneController->Start)
		{
			Value = -Value;
		}

		if (BoneController->Start + 359.0f >= BoneController->End)
		{
			if (Value > ((BoneController->Start + BoneController->End) / 2.0f) + 180.0f)
			{				
				Value = Value - 360.0f;
			}

			if (Value < ((BoneController->Start + BoneController->End) / 2.0f) - 180.0f)
			{
				Value = Value + 360.0f;
			}
		}
		
		else
		{
			if (Value > 360.0f)
			{
				Value	= Value - (long)(Value / 360.0f) * 360.0f;
			}

			else if (Value < 0.0f)
			{
				Value	= Value + (long)((Value / -360.0f) + 1.0f) * 360.0f;
			}
		}
	}

	long	Setting	= (long)(64.0f * (Value - BoneController->Start) / (BoneController->End - 
		BoneController->Start));

	if (Setting < 0)
	{
		Setting	= 0;
	}

	if (Setting > 64)
	{
		Setting	= 64;
	}

	MouthPosition	= (BYTE)Setting;

	return	Setting * (1.0f / 64.0f) * (BoneController->End - BoneController->Start) + 
		BoneController->Start;
}

float TMDLModel::SetBlending(long Blender, float Value)
{
	tagMDLSeqDescription	*SeqDescription	= 
		(tagMDLSeqDescription *)((BYTE *)Header + Header->SequenceOffset) + CurrentSequence;

	if (0 == SeqDescription->BlendType[Blender])
	{
		return	Value;
	}

	if (SeqDescription->BlendType[Blender] & (TRANSITION_XR | TRANSITION_YR | TRANSITION_ZR))
	{
		if (SeqDescription->BlendEnd[Blender] < SeqDescription->BlendStart[Blender])
		{
			Value = -Value;
		}

		if (SeqDescription->BlendStart[Blender] + 359.0f >= SeqDescription->BlendEnd[Blender])
		{
			if (Value > ((SeqDescription->BlendStart[Blender] + 
				SeqDescription->BlendEnd[Blender]) / 2.0f) + 180.0f)
			{
				Value = Value - 360.0f;
			}

			if (Value < ((SeqDescription->BlendStart[Blender] + 
				SeqDescription->BlendEnd[Blender]) / 2.0f) - 180.0f)
			{
				Value = Value + 360.0f;
			}
		}
	}

	long	Setting	= (long)(255.0f * (Value - SeqDescription->BlendStart[Blender]) / 
		(SeqDescription->BlendEnd[Blender] - SeqDescription->BlendStart[Blender]));

	if (Setting < 0)
	{
		Setting	= 0;
	}

	if (Setting > 255)
	{
		Setting = 255;
	}

	Blending[Blender]	= (BYTE)Setting;

	return	Setting * (1.0f / 255.0f) * (SeqDescription->BlendEnd[Blender] - 
		SeqDescription->BlendStart[Blender]) + SeqDescription->BlendStart[Blender];
}

long TMDLModel::SetBodyGroup(long Group, long Value)
{
	if (Group > Header->NumBodyParts)
	{
		return -1;
	}

	tagMDLBodyPart	*BodyPart	= 
		(tagMDLBodyPart *)((BYTE *)Header + Header->BodyPartOffset) + Group;

	long	Current	= (CurrentBodyPart / BodyPart->Base) % BodyPart->NumModels;

	if (Value >= BodyPart->NumModels)
	{
		return	Current;
	}

	CurrentBodyPart	= (CurrentBodyPart - (Current * BodyPart->Base) + (Value * BodyPart->Base));

	return	Value;
}

long TMDLModel::SetSkin(long Value)
{
	if (Value < Header->NumSkinFamilies)
	{
		return	CurrentSkin;
	}

	CurrentSkin	= Value;

	return	Value;
}

tagMDLHeader *TMDLModel::LoadModel(char *Filename)
{
	FILE	*Handle;
	long	Size;
	BYTE	*Buffer;

	Handle	= fopen(Filename, "rb");

	if (NULL == Handle)
	{
		printf("Unable to open %s\n", Filename);
		
		exit(1);
	}

	fseek(Handle, 0, SEEK_END);
	Size	= ftell(Handle);	
	
	rewind(Handle);

	Buffer	= new BYTE[Size];
	
	if (NULL == Buffer)
	{
		printf("Unable to allocate memory\n");
		
		exit(1);
	}

	fread(Buffer, Size, 1, Handle);
	fclose(Handle);

	tagMDLHeader	*MDLHeader	= (tagMDLHeader *)Buffer;
	tagMDLTexture	*Texture	= (tagMDLTexture *)(Buffer + MDLHeader->TextureOffset);
	
	if (MDLHeader->TextureOffset != 0)
	{
		for (long Loop = 0; Loop < MDLHeader->NumTextures; Loop++)
		{
			UploadTexture(&Texture[Loop], Buffer + Texture[Loop].Index, Buffer + 
				Texture[Loop].Width * Texture[Loop].Height + Texture[Loop].Index);
		}
	}

	return	(tagMDLHeader *)Buffer;
}

tagMDLSeqHeader *TMDLModel::LoadDemandSequences(char *Filename)
{
	FILE	*Handle;
	long	Size;
	BYTE	*Buffer;

	Handle	= fopen(Filename, "rb");

	if (NULL == Handle)
	{
		printf("Unable to open %s\n", Filename);
		
		exit(1);
	}

	fseek(Handle, 0, SEEK_END);
	Size	= ftell(Handle);	
	
	rewind(Handle);

	Buffer	= new BYTE[Size];
	
	if (NULL == Buffer)
	{
		printf("Unable to allocate memory\n");
		
		exit(1);
	}

	fread(Buffer, Size, 1, Handle);
	fclose(Handle);

	return	(tagMDLSeqHeader *)Buffer;
}

void TMDLModel::CalcBoneQuaternion(long Frame, float Value, tagMDLBone *Bone, tagAnimation *Anim, 
	float *Q)
{
	vec3_t			Angle1;
	vec3_t			Angle2;
	tagMDLAnimFrame	*AnimValue;

	for (long Loop = 0; Loop < 3; Loop++)
	{
		if (0 == Anim->Offset[Loop + 3])
		{
			Angle2[Loop]	= Angle1[Loop]	= Bone->Value[Loop + 3];
		}
		
		else
		{
			AnimValue	= (tagMDLAnimFrame *)((BYTE *)Anim + Anim->Offset[Loop + 3]);
			
			long Index	= Frame;
			
			while (AnimValue->Total <= Index)
			{
				Index		-= AnimValue->Total;
				AnimValue	+= AnimValue->Valid + 1;
			}
			
			if (AnimValue->Valid > Index)
			{
				Angle1[Loop]	= AnimValue[Index + 1].Value;

				if (AnimValue->Valid > Index + 1)
				{
					Angle2[Loop]	= AnimValue[Index + 2].Value;
				}
				
				else
				{
					if (AnimValue->Total > Index + 1)
					{
						Angle2[Loop]	= Angle1[Loop];
					}

					else
					{						
						Angle2[Loop]	= AnimValue[AnimValue->Valid + 2].Value;
					}
				}
			}
			
			else
			{
				Angle1[Loop]	= AnimValue[AnimValue->Valid].Value;
				
				if (AnimValue->Total > Index + 1)
				{
					Angle2[Loop]	= Angle1[Loop];
				}
				
				else
				{
					Angle2[Loop]	= AnimValue[AnimValue->Valid + 2].Value;
				}
			}
			
			Angle1[Loop]	= Bone->Value[Loop + 3] + Angle1[Loop] * Bone->Scale[Loop + 3];
			Angle2[Loop]	= Bone->Value[Loop + 3] + Angle2[Loop] * Bone->Scale[Loop + 3];
		}
	}

	if (!VectorCompare(Angle1, Angle2))
	{
		vec4_t	Q1;
		vec4_t	Q2;
		
		AngleQuaternion(Angle1, Q1);
		AngleQuaternion(Angle2, Q2);
		QuaternionSlerp(Q1, Q2, Value, Q);
	}
	
	else
	{
		AngleQuaternion(Angle1, Q);
	}
}

void TMDLModel::CalcBonePosition(long Frame, float Value, tagMDLBone *Bone, tagAnimation *Anim, 
	float *Pos)
{
	tagMDLAnimFrame	*AnimValue;

	for (long Loop = 0; Loop < 3; Loop++)
	{
		Pos[Loop]	= Bone->Value[Loop];
		
		if (Anim->Offset[Loop] != 0)
		{
			AnimValue	= (tagMDLAnimFrame *)((BYTE *)Anim + Anim->Offset[Loop]);
			
			long	Index	= Frame;
			
			while (AnimValue->Total <= Index)
			{
				Index		-= AnimValue->Total;
				AnimValue	+= AnimValue->Valid + 1;
			}
			
			if (AnimValue->Valid > Index)
			{
				if (AnimValue->Valid > Index + 1)
				{
					Pos[Loop]	+= (AnimValue[Index + 1].Value * (1.0f - Value) + Value * 
						AnimValue[Index + 2].Value) * Bone->Scale[Loop];
				}
				
				else
				{
					Pos[Loop]	+= AnimValue[Index + 1].Value * Bone->Scale[Loop];
				}
			}
			
			else
			{
				if (AnimValue->Total <= Index + 1)
				{
					Pos[Loop]	+= (AnimValue[AnimValue->Valid].Value * (1.0f - Value) + Value * 
						AnimValue[AnimValue->Valid + 2].Value) * Bone->Scale[Loop];
				}
				
				else
				{
					Pos[Loop]	+= AnimValue[AnimValue->Valid].Value * Bone->Scale[Loop];
				}
			}
		}
	}
}

void TMDLModel::CalcRotations(vec3_t *Pos, vec4_t *Q, tagMDLSeqDescription *SeqDescription, 
	tagAnimation *Anim, float FrameValue)
{
	long		Frame;
	float		Fractional;

	Frame		= (long)FrameValue;
	Fractional	= (FrameValue - Frame);

	tagMDLBone	*Bone	= (tagMDLBone *)((BYTE *)Header + Header->BoneOffset);
	
	for (long Loop = 0; Loop < Header->NumBones; Loop++, Bone++, Anim++) 
	{
		CalcBoneQuaternion(Frame, Fractional, Bone, Anim, Q[Loop]);
		CalcBonePosition(Frame, Fractional, Bone, Anim, Pos[Loop]);
	}

	if (SeqDescription->MotionType & TRANSITION_X)
	{
		Pos[SeqDescription->MotionBone][0]	= 0.0f;
	}

	if (SeqDescription->MotionType & TRANSITION_Y)
	{
		Pos[SeqDescription->MotionBone][1]	= 0.0f;
	}

	if (SeqDescription->MotionType & TRANSITION_Z)
	{
		Pos[SeqDescription->MotionBone][2]	= 0.0f;
	}
}

tagAnimation *TMDLModel::GetAnim(tagMDLSeqDescription *SeqDescription)
{
	tagMDLSeqGroup	*pseqgroup	= (tagMDLSeqGroup *)((BYTE *)Header + 
		Header->SeqGroupOffset) + SeqDescription->SeqGroup;

	if (SeqDescription->SeqGroup == 0)
	{
		return	(tagAnimation *)((BYTE *)Header + pseqgroup->Data + 
			SeqDescription->AnimOffset);
	}

	return	(tagAnimation *)((BYTE *)AnimationHeader[SeqDescription->SeqGroup] + 
		SeqDescription->AnimOffset);
}

void TMDLModel::SlerpBones(vec4_t Q1[], vec3_t Pos1[], vec4_t Q2[], vec3_t Pos2[], float Value)
{
	vec4_t	Q3;
	float	Inverse;

	if (Value < 0.0f)
	{
		Value	= 0.0f;
	}
	
	else if (Value > 1.0f)
	{
		Value	= 1.0f;
	}

	Inverse	= 1.0f - Value;

	for (long Loop = 0; Loop < Header->NumBones; Loop++)
	{
		QuaternionSlerp(Q1[Loop], Q2[Loop], Value, Q3);
		Q1[Loop][0]		= Q3[0];
		Q1[Loop][1]		= Q3[1];
		Q1[Loop][2]		= Q3[2];
		Q1[Loop][3]		= Q3[3];
		Pos1[Loop][0]	= Pos1[Loop][0] * Inverse + Pos2[Loop][0] * Value;
		Pos1[Loop][1]	= Pos1[Loop][1] * Inverse + Pos2[Loop][1] * Value;
		Pos1[Loop][2]	= Pos1[Loop][2] * Inverse + Pos2[Loop][2] * Value;
	}
}

//根据当前帧设置骨骼
void TMDLModel::SetUpBones()
{
	float			BoneMatrix[3][4];
	static vec3_t	Pos1[MAXBONES];
	static vec4_t	Q1[MAXBONES];
	static vec3_t	Pos2[MAXBONES];
	static vec4_t	Q2[MAXBONES];
	static vec3_t	Pos3[MAXBONES];
	static vec4_t	Q3[MAXBONES];
	static vec3_t	Pos4[MAXBONES];
	static vec4_t	Q4[MAXBONES];

	if (CurrentSequence >= Header->NumSequences)
	{
		CurrentSequence	= 0;
	}

	tagMDLSeqDescription	*SeqDescription	= (tagMDLSeqDescription *)((BYTE *)Header + 
		Header->SequenceOffset) + CurrentSequence;

	tagAnimation	*Anim	= GetAnim(SeqDescription);
	
	CalcRotations(Pos1, Q1, SeqDescription, Anim, CurrentFrame);

	if (SeqDescription->NumBlends > 1)
	{
		float	Value;

		Anim	+= Header->NumBones;
		CalcRotations(Pos2, Q2, SeqDescription, Anim, CurrentFrame);
		
		Value	= Blending[0] / 255.0f;

		SlerpBones(Q1, Pos1, Q2, Pos2, Value);

		if (4 == SeqDescription->NumBlends)
		{
			Anim	+= Header->NumBones;
			CalcRotations(Pos3, Q3, SeqDescription, Anim, CurrentFrame);

			Anim	+= Header->NumBones;
			CalcRotations(Pos4, Q4, SeqDescription, Anim, CurrentFrame);

			Value	= Blending[0] / 255.0f;
			SlerpBones(Q3, Pos3, Q4, Pos4, Value);

			Value	= Blending[1] / 255.0f;
			SlerpBones(Q1, Pos1, Q3, Pos3, Value);
		}
	}

	tagMDLBone	*Bones	= (tagMDLBone *)((BYTE *)Header + Header->BoneOffset);

	for (long Loop = 0; Loop < Header->NumBones; Loop++)
	{
		QuaternionMatrix(Q1[Loop], BoneMatrix);

		BoneMatrix[0][3]	= Pos1[Loop][0];
		BoneMatrix[1][3]	= Pos1[Loop][1];
		BoneMatrix[2][3]	= Pos1[Loop][2];

		if (-1 == Bones[Loop].Parent)
		{
			memcpy(BoneTransforms[Loop], BoneMatrix, sizeof(float) * 12);
		} 
		
		else
		{
			R_ConcatTransforms(BoneTransforms[Bones[Loop].Parent], BoneMatrix, 
				BoneTransforms[Loop]);
		}
	}
}

void TMDLModel::Lighting(float *lv, long Bone, long Flags, vec3_t Normal)
{
	float 	Illumination;
	float	LightCosine;

	Illumination	= (float)AmbientLightColor;

	if (Flags & LIGHT_FLATSHADE)
	{
		Illumination	+= ShadeLight * 0.8f;
	} 
	
	else 
	{
		float	Value;

		LightCosine	= DotProduct (Normal, BoneLightVector[Bone]);

		if (LightCosine > 1.0f)
		{			
			LightCosine = 1.0f;
		}

		Illumination	+= ShadeLight;

		Value	= 1.5f;
		
		if (Value <= 1.0f)
		{
			Value	= 1.0f;
		}

		LightCosine	= (LightCosine + (Value - 1.0f)) / Value; 
		
		if (LightCosine > 0.0f) 
		{
			Illumination	-= ShadeLight * LightCosine; 
		}
		
		if (Illumination <= 0)
		{
			Illumination	= 0;
		}
	}

	if (Illumination > 255) 
	{
		Illumination	= 255;
	}

	*lv	= Illumination / 255.0f;
}

void TMDLModel::SetupLighting()
{
	AmbientLightColor	= 32;
	ShadeLight			= 192;

	LightVector[0]		= 0.0f;
	LightVector[1]		= 0.0f;
	LightVector[2]		= -1.0f;

	LightColor[0]		= 1.0f;
	LightColor[1]		= 1.0f;
	LightColor[2]		= 1.0f;

	for (long Loop = 0; Loop < Header->NumBones; Loop++)
	{
		VectorIRotate(LightVector, BoneTransforms[Loop], BoneLightVector[Loop]);
	}
}

void TMDLModel::SetupModel(long BodyPart)
{
	if (BodyPart > Header->NumBodyParts)
	{
		BodyPart	= 0;
	}

	tagMDLBodyPart   *BodyPartPtr	= (tagMDLBodyPart *)((BYTE *)Header + Header->BodyPartOffset) + 
		BodyPart;

	long	Index	= CurrentBodyPart / BodyPartPtr->Base;
	Index			= Index % BodyPartPtr->NumModels;

	Model	= (tagMDLModel *)((BYTE *)Header + BodyPartPtr->ModelOffset) + Index;
}

//绘制模型
void TMDLModel::DrawModel()
{
	TransformVertPtr	= &TransformVertices[0];
	LightValuesPtr		= &LightValues[0];

	if (0 == Header->NumBodyParts)	return;
/*
	glPushMatrix();
    glTranslatef(Origin[0], Origin[1], Origin[2]);
    glRotatef(Rotation[1], 0, 0, 1);
    glRotatef(Rotation[0], 0, 1, 0);
    glRotatef(Rotation[2], 1, 0, 0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
*/
	SetUpBones();
	SetupLighting();
	for (long Loop = 0; Loop < Header->NumBodyParts; Loop++) 
	{
		SetupModel(Loop);
		DrawPoints();
	}
//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//	glPopMatrix();
}
/////////////////////////////////////////////////////////////
void TMDLModel::DrawPoints()//显示0
{
	float			*LightValue;
	long Loop ;
	BYTE			*BoneVertexPtr	= ((BYTE *)Header + Model->VertexInfoOffset);
	BYTE			*BoneNormalPtr	= ((BYTE *)Header + Model->NormalInfoOffset);
	tagMDLTexture	*Texture		= (tagMDLTexture *)((BYTE *)TextureHeader + 
		TextureHeader->TextureOffset);
	tagMDLMesh		*MeshPtr		= (tagMDLMesh *)((BYTE *)Header + Model->MeshOffset);
	vec3_t			*Vertices		= (vec3_t *)((BYTE *)Header + Model->VertexOffset);
	vec3_t			*Normals		= (vec3_t *)((BYTE *)Header + Model->NormalOffset);
	short			*SkinReference	= (short *)((BYTE *)TextureHeader + TextureHeader->SkinOffset);

	if (CurrentSkin != 0 && CurrentSkin < TextureHeader->NumSkinFamilies)
		SkinReference	+= (CurrentSkin * TextureHeader->NumSkinReferences);
	
	for (Loop = 0; Loop < Model->NumVertices; Loop++)
	{
		VectorTransform(Vertices[Loop], BoneTransforms[BoneVertexPtr[Loop]], 
			TransformVertPtr[Loop]);
	}

	LightValue	= (float *)LightValuesPtr;
	for (long OuterLoop = 0; OuterLoop < Model->NumMesh; OuterLoop++) 
	{
		long	Flags	= Texture[SkinReference[MeshPtr[OuterLoop].SkinReference]].Flags;
		for (long InnerLoop = 0; InnerLoop < MeshPtr[OuterLoop].NumNormals; InnerLoop++, 
			LightValue += 3, Normals++, BoneNormalPtr++)
		{
			float	Temp;
			Lighting(&Temp, *BoneNormalPtr, Flags, (float *)Normals);
			LightValue[0]	= Temp * LightColor[0];
			LightValue[1]	= Temp * LightColor[1];
			LightValue[2]	= Temp * LightColor[2];
		}
	}
//	glCullFace(GL_FRONT);
	for (Loop = 0; Loop < Model->NumMesh; Loop++) 
	{	
		MeshPtr				= (tagMDLMesh *)((BYTE *)Header + Model->MeshOffset) + Loop;
		short	*Triangles	= (short *)((BYTE *)Header + MeshPtr->TriangleOffset);
		float	U			= 1.0f / (float)Texture[SkinReference[MeshPtr->SkinReference]].Width;
		float	V			= 1.0f / (float)Texture[SkinReference[MeshPtr->SkinReference]].Height;
		
		glBindTexture(GL_TEXTURE_2D, Texture[SkinReference[MeshPtr->SkinReference]].Index);
		
		long	NumTriangles;
		while (NumTriangles = *(Triangles++))
		{
			if (NumTriangles < 0)
			{
				glBegin(GL_TRIANGLE_FAN);
				NumTriangles	= -NumTriangles;
			}
			else
				glBegin(GL_TRIANGLE_STRIP);

			for (; NumTriangles > 0; NumTriangles--, Triangles += 4)
			{
				glTexCoord2f(Triangles[2] * U, Triangles[3] * V);
				
				LightValue	= LightValuesPtr[Triangles[1]];
				glColor4f(LightValue[0], LightValue[1], LightValue[2], 1.0f);
				
				float	*Vertex	= TransformVertPtr[Triangles[0]];
				glVertex3f(Vertex[0], Vertex[1], Vertex[2]);
			}

			glEnd();
		}
	}
}
void TMDLModel::UploadTexture(tagMDLTexture *Texture, BYTE *Data, BYTE *Palette)
{	long	Row1[256];
	long	Row2[256];
	long	Column1[256];
	long	Column2[256];
	BYTE	*TextureBuffer;
	BYTE	*BufferPtr;
	long OutHeight;
	long OutWidth ; 
	for (OutWidth = 1; OutWidth < Texture->Width; OutWidth <<= 1);
	if (OutWidth > 256)	OutWidth = 256;
	for (OutHeight = 1; OutHeight < Texture->Height; OutHeight <<= 1);
	if (OutHeight > 256)	OutHeight = 256;
	TextureBuffer	= new BYTE[OutWidth * OutHeight * 4];
	if (NULL == TextureBuffer)
	{	printf("Unable to allocate memory.\n");
		exit(1);
	}
	BufferPtr	= TextureBuffer;
	long Loop;
	for (Loop = 0; Loop < OutWidth; Loop++)
	{
		Column1[Loop]	= (long)((Loop + 0.25f) * (Texture->Width / (float)OutWidth));
		Column2[Loop]	= (long)((Loop + 0.75f) * (Texture->Width / (float)OutWidth));
	}

	for (Loop = 0; Loop < OutHeight; Loop++)
	{
		Row1[Loop]	= (long)((Loop + 0.25f) * (Texture->Height / (float)OutHeight)) * 
			Texture->Width;
		
		Row2[Loop]	= (long)((Loop + 0.75f) * (Texture->Height / (float)OutHeight)) * 
			Texture->Width;
	}

	for (long OuterLoop = 0;  OuterLoop < OutHeight; OuterLoop++)
	{
		for (long InnerLoop = 0; InnerLoop < OutWidth; InnerLoop++, BufferPtr += 4)
		{
			BYTE	*Pixel1	= &Palette[Data[Row1[OuterLoop] + Column1[InnerLoop]] * 3];
			BYTE	*Pixel2	= &Palette[Data[Row1[OuterLoop] + Column2[InnerLoop]] * 3];
			BYTE	*Pixel3	= &Palette[Data[Row2[OuterLoop] + Column1[InnerLoop]] * 3];
			BYTE	*Pixel4	= &Palette[Data[Row2[OuterLoop] + Column2[InnerLoop]] * 3];

			BufferPtr[0]	= (Pixel1[0] + Pixel2[0] + Pixel3[0] + Pixel4[0]) >> 2;
			BufferPtr[1]	= (Pixel1[1] + Pixel2[1] + Pixel3[1] + Pixel4[1]) >> 2;
			BufferPtr[2]	= (Pixel1[2] + Pixel2[2] + Pixel3[2] + Pixel4[2]) >> 2;
			BufferPtr[3]	= 0xFF;
		}
	}

	glBindTexture(GL_TEXTURE_2D, CurrentTexture);		
	glTexImage2D(GL_TEXTURE_2D, 0, 3, OutWidth, OutHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
		TextureBuffer);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	Texture->Index	= CurrentTexture;

	CurrentTexture++;

	delete [] TextureBuffer;
}