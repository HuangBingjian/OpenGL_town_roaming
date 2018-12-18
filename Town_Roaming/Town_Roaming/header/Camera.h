#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "stdafx.h"
#include "Vector.h"                 /**< ����������ͷ�ļ� */
//#include "GLKey.h"

/** ������� */
class Camera
{
public:
	
	/** ���캯������������ */
	Camera();
	~Camera();
	
	/** ��������״̬���� */
	Vector3 getPosition()   {	return m_Position;		}
	Vector3 getView()	    {	return m_View;			}
	Vector3 getUpVector()   {	return m_UpVector;		}
	float   getSpeed()      {   return m_Speed;         }

	/** �����ٶ� */
	void setSpeed(float speed)
	{ 
		m_Speed  = speed;
	}

	/** �����������λ��, �۲����������� */
	void setCamera(float positionX, float positionY, float positionZ,
			 	   float viewX,     float viewY,     float viewZ,
				   float upVectorX, float upVectorY, float upVectorZ);

	/** ��ת��������� */
	void rotateView(float angle, float X, float Y, float Z);

	/** �����������������۲췽�� */
	void setViewByMouse();
	
    /** ����������ƶ� */
	void yawCamera(float speed);

	/** ǰ���ƶ������ */
	void moveCamera(float speed);
	
	/** ��������� */
	void setLook();

	void initObstacle();

	bool Camera::isObstacle();

    //�õ������ָ��
	static Camera* GetCamera(void) { return m_pCamera;}


public:
	/** ��������� */
	static Camera  *m_pCamera;      /**< ��ǰȫ�������ָ�� */
	Vector3        m_Position;      /**< λ�� */
	Vector3        m_View;          /**< ���� */
	Vector3        m_UpVector;      /**< �������� */
	float		   m_AngleX;
	float		   m_AngleY;
	float          m_Speed;         /**< �ٶ� */
	Vector3		   m_Obstacle[MAX_OBSTACLE];

};

#endif //__CAMERA_H__