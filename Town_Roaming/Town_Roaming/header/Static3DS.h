#ifndef __Static3DS_H__
#define __Static3DS_H__

#include "stdafx.h"
#include "Vector.h"                 /**< ����������ͷ�ļ� */
#include "GL_3dsLoder.h"
#include "C3DSModel.h"

//��Ҫ�����3DS�ļ�
#define MY_3DS_Balloon1 "balloon.3ds"
#define MY_3DS_Balloon2 "balloon.3ds"
#define MY_3DS_Jeep "jeep.3ds"

//#define MY_3DS_CKXT "plane.3ds"//ֱ����
/** ��̬ģ���� */
class Static3DS
{
public:
	/** ���캯������������ */
	Static3DS();
	~Static3DS();
	bool init();
	bool balloon_render();
	bool jeep_render(float car_x, float car_y, float car_z, float car_angle);
	Vector3 get_jeep_pos();
	void set_jeep_pos(Vector3 jeep_pos);
		
private:
	static Static3DS  *m_pStatic3DS; 

	GLfloat	    m_look[3];
	float		m_Angle;
	float		rad_xz;

	C3DSModel g_balloon1;		//����
	C3DSModel g_balloon2;		//����
	C3DSModel g_jeep;			//jeep
	float jeep_x, jeep_y, jeep_z;
	
};

#endif //__Static3DS_H__