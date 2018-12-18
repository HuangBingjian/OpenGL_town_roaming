#ifndef GL_FLAG_H
#define GL_FLAG_H
#include "GL_Base.h"
#include "cvector.h"

typedef	struct _Particle {
	float	fMass;
	float	fInvMass;
	CVector	vPosition;
	CVector	vVelocity;
	CVector	vAcceleration;
	CVector	vForces;
	BOOL	bLocked;
} Particle1;

typedef struct _ParticleRef {
	int	r;		// row index
	int c;		// column index
} ParticleRef;

typedef	struct	_Spring {
	ParticleRef		p1;			// reference to connected particle #1
	ParticleRef		p2;			// reference to connected particle #2
	float	k;					// tensile spring constant
	float	d;					// damping factor	
	float	L;					// rest length of spring
} Spring;

const int       NUMCOLUMNS            =10;         // 旗帜网格，将旗子分成10列7行的网格
const int       NUMROWS               =7;
const int       NUMSTRUCTURALSPRINGS  =NUMCOLUMNS*(NUMROWS+1) + NUMROWS*(NUMCOLUMNS+1) + NUMCOLUMNS*NUMROWS*2;

const float		GRAVITY		          =	-32.174f;
const float		SPRINGTENSIONCONSTANT =	600.0f  ;  //弹簧系数  t
const float		SPRINGSHEARCONSTANT	  =	600.0f  ;  //          s  
const float		SPRINGDAMPINGCONSTANT =	2.0f    ;  //          d
const float		DRAGCOEFFICIENT		  =	0.01f   ;

class GL_Flag
{
public:
	GL_Flag();
	~GL_Flag();
	void Update();
	void render(GLuint flag_tex, GLuint pole_tex, float flag_x, float flag_y, float flag_z);
	void SetFlagSize(float width,float height){m_flagwidth=width;m_flagheight=height;}
	////////////////////////////
    static GL_Flag *pFlag;
	float  *GetHeight(){return height;}
	float  *GetXpos  (){return xpos;}
	float  *GetZpos  (){return zpos;}

private:
		
	void	Initialize();

    void	CalcForces(Particle1 particles[NUMROWS+1][NUMCOLUMNS+1]);			
	int     tb_Rnd(int min, int max);
    void	StepSimulation(float dt);			// step dt time in the simulation
    void	SetWindVector(float x, float y, float z);


private:
	float *height;           // 旗帜的高度
	float *xpos;             // 位置 
	float *zpos;

	float               m_flagwidth;    //旗帜的宽度
	float               m_flagheight;   //旗帜的高度

	Particle1			Particles[NUMROWS+1][NUMCOLUMNS+1];
    Spring				StructuralSprings[NUMSTRUCTURALSPRINGS];
	CVector				WindVector;
    float				WindForceFactor;

    float               MASSPERFACE;
	float               CSTEP;
	float               RSTEP;
};

#endif