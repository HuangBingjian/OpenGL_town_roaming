// *************************************************************
// 数学方面，跳过，调用就行
// *************************************************************

#include "..\header\stdafx.h"
#include "..\header\MathLib.h"
BOOL VectorCompare(vec3_t Vector1, vec3_t Vector2)
{
	for (DWORD Loop = 0; Loop <3; Loop++)
	{
		if (fabs(Vector1[Loop] - Vector2[Loop]) > EPSILON)
		{
			return	FALSE;
		}
	}
			
	return	TRUE;
}

void CrossProduct(vec3_t Vector1, vec3_t Vector2, vec3_t Cross)
{
	Cross[0]	= Vector1[1] * Vector2[2] - Vector1[2] * Vector2[1];
	Cross[1]	= Vector1[2] * Vector2[0] - Vector1[0] * Vector2[2];
	Cross[2]	= Vector1[0] * Vector2[1] - Vector1[1] * Vector2[0];
}

void R_ConcatTransforms(const float In1[3][4], const float In2[3][4], float Out[3][4])
{
	Out[0][0]	= In1[0][0] * In2[0][0] + In1[0][1] * In2[1][0] + In1[0][2] * In2[2][0];
	Out[0][1]	= In1[0][0] * In2[0][1] + In1[0][1] * In2[1][1] + In1[0][2] * In2[2][1];
	Out[0][2]	= In1[0][0] * In2[0][2] + In1[0][1] * In2[1][2] + In1[0][2] * In2[2][2];
	Out[0][3]	= In1[0][0] * In2[0][3] + In1[0][1] * In2[1][3] + In1[0][2] * In2[2][3] + In1[0][3];
	Out[1][0]	= In1[1][0] * In2[0][0] + In1[1][1] * In2[1][0] + In1[1][2] * In2[2][0];
	Out[1][1]	= In1[1][0] * In2[0][1] + In1[1][1] * In2[1][1] + In1[1][2] * In2[2][1];
	Out[1][2]	= In1[1][0] * In2[0][2] + In1[1][1] * In2[1][2] + In1[1][2] * In2[2][2];
	Out[1][3]	= In1[1][0] * In2[0][3] + In1[1][1] * In2[1][3] + In1[1][2] * In2[2][3] + In1[1][3];
	Out[2][0]	= In1[2][0] * In2[0][0] + In1[2][1] * In2[1][0] + In1[2][2] * In2[2][0];
	Out[2][1]	= In1[2][0] * In2[0][1] + In1[2][1] * In2[1][1] + In1[2][2] * In2[2][1];
	Out[2][2]	= In1[2][0] * In2[0][2] + In1[2][1] * In2[1][2] + In1[2][2] * In2[2][2];
	Out[2][3]	= In1[2][0] * In2[0][3] + In1[2][1] * In2[1][3] + In1[2][2] * In2[2][3] + In1[2][3];
}

// rotate by the inverse of the matrix
void VectorIRotate(const vec3_t In1, const float In2[3][4], vec3_t Out)
{
	Out[0]	= In1[0] * In2[0][0] + In1[1] * In2[1][0] + In1[2] * In2[2][0];
	Out[1]	= In1[0] * In2[0][1] + In1[1] * In2[1][1] + In1[2] * In2[2][1];
	Out[2]	= In1[0] * In2[0][2] + In1[1] * In2[1][2] + In1[2] * In2[2][2];
}
void VectorTransform(const vec3_t In1, const float In2[3][4], vec3_t Out)
{
	Out[0]	= DotProduct(In1, In2[0]) + In2[0][3];
	Out[1]	= DotProduct(In1, In2[1]) + In2[1][3];
	Out[2]	= DotProduct(In1, In2[2]) + In2[2][3];
}

void AngleQuaternion(const vec3_t Angles, vec4_t Quaternion)
{
	float	Angle	= Angles[0] * 0.5f;
	float	Sin0	= (float)sin(Angle);
	float	Cos0	= (float)cos(Angle);

	Angle			= Angles[1] * 0.5f;
	float	Sin1	= (float)sin(Angle);
	float	Cos1	= (float)cos(Angle);

	Angle			= Angles[2] * 0.5f;
	float	Sin2	= (float)sin(Angle);
	float	Cos2	= (float)cos(Angle);

	Quaternion[0]	= Sin0 * Cos1 * Cos2 - Cos0 * Sin1 * Sin2;
	Quaternion[1]	= Cos0 * Sin1 * Cos2 + Sin0 * Cos1 * Sin2;
	Quaternion[2]	= Cos0 * Cos1 * Sin2 - Sin0 * Sin1 * Cos2;
	Quaternion[3]	= Cos0 * Cos1 * Cos2 + Sin0 * Sin1 * Sin2;
}

void QuaternionMatrix(const vec4_t Quaternion, float (*Matrix)[4])
{
	Matrix[0][0]	= 1.0f - 2.0f * Quaternion[1] * Quaternion[1] - 2.0f * Quaternion[2] * Quaternion[2];
	Matrix[1][0]	= 2.0f * Quaternion[0] * Quaternion[1] + 2.0f * Quaternion[3] * Quaternion[2];
	Matrix[2][0]	= 2.0f * Quaternion[0] * Quaternion[2] - 2.0f * Quaternion[3] * Quaternion[1];

	Matrix[0][1]	= 2.0f * Quaternion[0] * Quaternion[1] - 2.0f * Quaternion[3] * Quaternion[2];
	Matrix[1][1]	= 1.0f - 2.0f * Quaternion[0] * Quaternion[0] - 2.0f * Quaternion[2] * Quaternion[2];
	Matrix[2][1]	= 2.0f * Quaternion[1] * Quaternion[2] + 2.0f * Quaternion[3] * Quaternion[0];

	Matrix[0][2]	= 2.0f * Quaternion[0] * Quaternion[2] + 2.0f * Quaternion[3] * Quaternion[1];
	Matrix[1][2]	= 2.0f * Quaternion[1] * Quaternion[2] - 2.0f * Quaternion[3] * Quaternion[0];
	Matrix[2][2]	= 1.0f - 2.0f * Quaternion[0] * Quaternion[0] - 2.0f * Quaternion[1] * Quaternion[1];
}

void QuaternionSlerp(const vec4_t In1, vec4_t In2, float Time, vec4_t Out)
{
	float	Slerp1;
	float	Slerp2;
	DWORD Loop ;
	// Decide if one of the quaternions is backwards
	float Value1	= 0.0f;
	float Value2	= 0.0f;
	
	for (Loop = 0; Loop < 4; Loop++)
	{
		Value1	+= (In1[Loop] - In2[Loop]) * (In1[Loop] - In2[Loop]);
		Value2	+= (In1[Loop] + In2[Loop]) * (In1[Loop] + In2[Loop]);
	}
	
	if (Value1 > Value2)
	{
		for (Loop = 0; Loop < 4; Loop++)
		{
			In2[Loop]	= -In2[Loop];
		}
	}

	float	CosAngle	= In1[0] * In2[0] + In1[1] * In2[1] + In1[2] * In2[2] + In1[3] * In2[3];

	if ((1.0f + CosAngle) > 0.00000001f)
	{
		if ((1.0f - CosAngle) > 0.00000001f)
		{
			float	Omega		= (float)acos(CosAngle);
			float	SinAngle	= (float)sin(Omega);
			
			Slerp1				= (float)sin((1.0f - Time) * Omega) / SinAngle;
			Slerp2				= (float)sin(Time * Omega) / SinAngle;
		}
		
		else
		{
			Slerp1				= 1.0f - Time;
			Slerp2				= Time;
		}
		
		for (Loop = 0; Loop < 4; Loop++)
		{
			Out[Loop]	= Slerp1 * In1[Loop] + Slerp2 * In2[Loop];
		}
	}
	
	else
	{
		Out[0]	= -In1[1];
		Out[1]	= In1[0];
		Out[2]	= -In1[3];
		Out[3]	= In1[2];
		Slerp1	= (float)sin((1.0f - Time) * 0.5f * Q_PI);
		Slerp2	= (float)sin(Time * 0.5f * Q_PI);
		
		for (Loop = 0; Loop < 3; Loop++)
		{
			Out[Loop]	= Slerp1 * In1[Loop] + Slerp2 * Out[Loop];
		}
	}
}