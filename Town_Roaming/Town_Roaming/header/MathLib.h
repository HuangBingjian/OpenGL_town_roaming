#ifndef __MATHLIB__
#define __MATHLIB__

typedef	float	vec3_t[3];	// x,y,z
typedef	float	vec4_t[4];	// x,y,z,w

#define	EPSILON	0.001f
#define	Q_PI	3.14159265358979323846f

#define DotProduct(x,y) ((x)[0] * (y)[0] + (x)[1] * (y)[1] + (x)[2] * (y)[2])

BOOL VectorCompare(vec3_t Vector1, vec3_t Vector2);
void CrossProduct(vec3_t Vector1, vec3_t Vector2, vec3_t Cross);
void R_ConcatTransforms(const float In1[3][4], const float In2[3][4], float Out[3][4]);
void VectorIRotate(const vec3_t In1, const float In2[3][4], vec3_t Out);
void VectorTransform(const vec3_t In1, const float In2[3][4], vec3_t Out);
void AngleQuaternion(const vec3_t Angles, vec4_t Quaternion);
void QuaternionMatrix(const vec4_t Quaternion, float (*Matrix)[4]);
void QuaternionSlerp(const vec4_t In1, vec4_t In2, float Time, vec4_t Out);

#endif
