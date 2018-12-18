// *************************************************************
// 绘制飘扬的旗帜
// *************************************************************


#define _CRT_SECURE_NO_WARNINGS

#include "..\header\GL_Flag.h"
#include <math.h>

GL_Flag *GL_Flag::pFlag = NULL;

GL_Flag::GL_Flag():m_flagwidth(0.5),m_flagheight(3),WindForceFactor(100.0f)
{
    MASSPERFACE=100.0f/((NUMCOLUMNS*NUMROWS) * 2);
	CSTEP      =m_flagwidth  / NUMCOLUMNS;
	RSTEP	   =m_flagheight / NUMROWS   ;
	Initialize();      // 旗帜  

	pFlag=this;
}

GL_Flag::~GL_Flag()
{
	delete []height;
	delete []xpos;
	delete []zpos;
}

float  Magnitude(CVector vNormal)
{
	return (float)sqrt((vNormal.x * vNormal.x) + (vNormal.y * vNormal.y) + (vNormal.z * vNormal.z));
}
CVector  Normalize(CVector vNormal)
{
	float magnitude = Magnitude(vNormal);
	if (magnitude == 0)
		magnitude = 1;

	vNormal.x /= magnitude;
	vNormal.y /= magnitude;
	vNormal.z /= magnitude;

	return vNormal;
}
float  Dot(CVector vVector1, CVector vVector2)
{
	return ((vVector1.x * vVector2.x) + (vVector1.y * vVector2.y) + (vVector1.z * vVector2.z));
}

void GL_Flag::render(GLuint flag_tex,GLuint pole_tex,float flag_x,float flag_y,float flag_z)
{
	    glPushMatrix();		
		glTranslatef(flag_x, flag_y, flag_z);//旗子
		glRotatef(90, 0.0, 1.0, 0.0);
		int r, c;	
		
		glBindTexture(GL_TEXTURE_2D, flag_tex);  // 旗面
        glBegin(GL_TRIANGLE_STRIP);
		for(c=0; c<=NUMCOLUMNS-1; c++)
		{
			for(r=0; r<=NUMROWS; r++)
			{
				float x1=Particles[r][c].vPosition.x;
				float y1=Particles[r][c].vPosition.y;
				float z1=Particles[r][c].vPosition.z;

				float x2=Particles[r][c+1].vPosition.x;
				float y2=Particles[r][c+1].vPosition.y;
				float z2=Particles[r][c+1].vPosition.z;
				
				glTexCoord2f(r/(float)NUMROWS, 1-c/(float)NUMCOLUMNS);
				glVertex3f(x1, y1, z1);
				
				glTexCoord2f(r/(float)NUMROWS, 1-(c+1)/(float)NUMCOLUMNS);
				glVertex3f(x2, y2, z2);
			}
		}
		glEnd();
	    glPopMatrix();

		// 旗杆
		glPushMatrix();
		int nslice = 6;
		double t = 0.0; double dt = 2 * PI / nslice;
		glBindTexture(GL_TEXTURE_2D, pole_tex);
		glTranslatef(flag_x, 0, flag_z);
		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= nslice; ++j)
		{
			glTexCoord2f(t / (2 * PI), 1.0); glVertex3f(0.05 * cos(t), flag_y+m_flagheight, -sin(t) * 0.05);
			glTexCoord2f(t / (2 * PI), 0.0); glVertex3f(0.05 * cos(t), 0.0, -sin(t) * 0.05);
			t = t + dt;
		}
		glEnd();
		glPopMatrix();
}

void GL_Flag::Initialize()
{
	int	r, c;
	float f;
	CVector			L;
	int				count;

	for(r=0; r<=NUMROWS; r++)
	{
		for(c=0; c<=NUMCOLUMNS; c++)
		{
			// 计算节点质量
			if((r == 0) && (c == 0)) 
				f = 1;
			else if((r == NUMROWS) && (c == 0)) 
				f = 2;
			else if((r == 0) && (c == NUMCOLUMNS)) 
				f = 2;
			else if((r == NUMROWS) && (c == NUMCOLUMNS)) 
				f = 1;
			else if(((r == 0) || (r == NUMROWS)) && ((c != 0) && (c != NUMCOLUMNS))) 
				f = 3;
			else
				f = 6;

			Particles[r][c].fMass = (f * MASSPERFACE) / 3;
			Particles[r][c].fInvMass = 1 / Particles[r][c].fMass;
			
			// 初始节点的位置
			Particles[r][c].vPosition.x = c * CSTEP;
			Particles[r][c].vPosition.y = r * RSTEP;
			Particles[r][c].vPosition.z = 0.0f;

			// 初速度设为零
			Particles[r][c].vVelocity.x = 0.0f;
			Particles[r][c].vVelocity.y = 0.0f;
			Particles[r][c].vVelocity.z = 0.0f;

			Particles[r][c].vAcceleration.x = 0.0f;
			Particles[r][c].vAcceleration.y = 0.0f;
			Particles[r][c].vAcceleration.z = 0.0f;
			
			Particles[r][c].vForces.x = 0.0f;
			Particles[r][c].vForces.y = 0.0f;
			Particles[r][c].vForces.z = 0.0f;
			if(c == 0 )
				Particles[r][c].bLocked = TRUE;
			else
				Particles[r][c].bLocked = FALSE;

		}
	}
	
	// 设定构造的弹簧，连接临近的粒子
	count = 0;
	
	for(r=0; r<=NUMROWS; r++)
	{
		for(c=0; c<=NUMCOLUMNS; c++)
		{			
			if(c<NUMCOLUMNS)
			{
				StructuralSprings[count].p1.r = r;
				StructuralSprings[count].p1.c = c;
				StructuralSprings[count].p2.r = r;
				StructuralSprings[count].p2.c = c+1;
				StructuralSprings[count].k = SPRINGTENSIONCONSTANT;
				StructuralSprings[count].d = SPRINGDAMPINGCONSTANT;				
				L = Particles[r][c].vPosition - Particles[r][c+1].vPosition;
				StructuralSprings[count].L = Magnitude(L);				
				count++;
			}
			if(r<NUMROWS)
			{
				StructuralSprings[count].p1.r = r;
				StructuralSprings[count].p1.c = c;
				StructuralSprings[count].p2.r = r+1;
				StructuralSprings[count].p2.c = c;
				StructuralSprings[count].k = SPRINGTENSIONCONSTANT;
				StructuralSprings[count].d = SPRINGDAMPINGCONSTANT;				
				L = Particles[r][c].vPosition - Particles[r+1][c].vPosition;
				StructuralSprings[count].L = Magnitude(L);				
				count++;
			}
			if(r<NUMROWS && c<NUMCOLUMNS)
			{
				StructuralSprings[count].p1.r = r;
				StructuralSprings[count].p1.c = c;
				StructuralSprings[count].p2.r = r+1;
				StructuralSprings[count].p2.c = c+1;
				StructuralSprings[count].k = SPRINGSHEARCONSTANT;
				StructuralSprings[count].d = SPRINGDAMPINGCONSTANT;				
				L = Particles[r][c].vPosition - Particles[r+1][c+1].vPosition;
				StructuralSprings[count].L = Magnitude(L);				
				count++;
			}
			if(c>0 && r<NUMROWS)
			{
				StructuralSprings[count].p1.r = r;
				StructuralSprings[count].p1.c = c;
				StructuralSprings[count].p2.r = r+1;
				StructuralSprings[count].p2.c = c-1;
				StructuralSprings[count].k = SPRINGSHEARCONSTANT;
				StructuralSprings[count].d = SPRINGDAMPINGCONSTANT;				
				L = Particles[r][c].vPosition - Particles[r+1][c-1].vPosition;
				StructuralSprings[count].L = Magnitude(L);				
				count++;
			}
		}
	}

	
	WindVector.x = 1.0;
	WindVector.y = 1.0;
	WindVector.z = 1.0;

}

void GL_Flag::CalcForces(Particle1 particles[NUMROWS+1][NUMCOLUMNS+1])
{
	int	r, c, i, r1, c1, r2, c2;
	CVector	dragVector;
	CVector	f1, f2, d, v;
	float	L;

	// 所有的力设为零
	for(r=0; r<=NUMROWS; r++)
	{
		for(c=0; c<=NUMCOLUMNS; c++)
		{				
			particles[r][c].vForces.x = 0;
			particles[r][c].vForces.y = 0;
			particles[r][c].vForces.z = 0;
		}
	}

	// 计算重力和阻力
	for(r=0; r<=NUMROWS; r++)
	{
		for(c=0; c<=NUMCOLUMNS; c++)
		{				
			
			if(particles[r][c].bLocked == FALSE)
			{
				// 重力
				particles[r][c].vForces.y +=  (GRAVITY * particles[r][c].fMass);

				// 阻力
				dragVector = -particles[r][c].vVelocity;
				dragVector = Normalize(dragVector);
				particles[r][c].vForces += dragVector * (Magnitude(particles[r][c].vVelocity)*Magnitude(particles[r][c].vVelocity)) * DRAGCOEFFICIENT;

				// 风
				SetWindVector((float)tb_Rnd(0, 10), 0, (float)tb_Rnd(0, 1));
				WindVector=Normalize(WindVector);
				particles[r][c].vForces += WindVector * (float)tb_Rnd(0, (int)WindForceFactor);			
			}
		}
	}

	// 计算弹簧拉力
	for(i = 0; i<NUMSTRUCTURALSPRINGS; i++)
	{
		r1 = StructuralSprings[i].p1.r;
		c1 = StructuralSprings[i].p1.c;
		r2 = StructuralSprings[i].p2.r;
		c2 = StructuralSprings[i].p2.c;

		d = particles[r1][c1].vPosition - particles[r2][c2].vPosition;
		v = particles[r1][c1].vVelocity - particles[r2][c2].vVelocity;
		L = StructuralSprings[i].L;

		f1 = -(StructuralSprings[i].k * (Magnitude(d) - L) + StructuralSprings[i].d * ( Dot(v,d) / Magnitude(d) )) * ( d / Magnitude(d) );
		f2 = -f1;
		
		if(particles[r1][c1].bLocked == FALSE)
			particles[r1][c1].vForces += f1;

		if(particles[r2][c2].bLocked == FALSE)
			particles[r2][c2].vForces += f2;
	}
}

int GL_Flag::tb_Rnd(int min, int max)
{
	int	number;		
    number = (((abs(rand())%(max-min+1))+min));    
    
    if(number>max) 
	{
    	number = max;
    }
    
    if(number<min) 
	{
    	number = min;
    }	
		
	return number;
}

void GL_Flag::SetWindVector(float x, float y, float z)
{
	WindVector.x = x;
	WindVector.y = y;
	WindVector.z = z;
}


void GL_Flag::StepSimulation(float dt)
{
	CVector	Ae;
	int		r, c;
	int		check = 0;

	// 计算所有的力 
	CalcForces(Particles);

	// 积分
	for(r=0; r<=NUMROWS; r++)
	{
		for(c=0; c<=NUMCOLUMNS; c++)
		{				
			Ae = Particles[r][c].vForces * Particles[r][c].fInvMass;
			Particles[r][c].vAcceleration = Ae;
			Particles[r][c].vVelocity += Ae * dt;
			Particles[r][c].vPosition += Particles[r][c].vVelocity * dt;
		}
	}	

}

void GL_Flag::Update()
{
	StepSimulation(0.02f);
}
