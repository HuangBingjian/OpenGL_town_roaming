// *************************************************************
// �������������Ʒ�ĺ���֮һ
// *************************************************************


#include <afx.h>
#include <string.h>								 /**< ���������� */
#include "..\header\stdafx.h"
#include "..\header\Camera.h"                    /**< ���������ͷ�ļ� */
#include "..\header\Vector.h"                    /**< ���������� */
#include "..\header\Font.h"

Camera* Camera::m_pCamera = NULL;

/** ���캯�� */
Camera::Camera()
{
	/** ��ʼ������ֵ */
	Vector3 zero = Vector3(0.0, 0.0, 0.0);		
	Vector3 view = Vector3(0.0, 1.0, 0.5);		
	Vector3 up   = Vector3(0.0, 0.0, 1.0);	
    
	/** ��ʼ������� */
	m_Position	= zero;					
	m_View		= view;				
	m_UpVector	= up;	
	m_Speed     = 0.1f;

	m_pCamera = this;
	initObstacle();
}


Camera::~Camera()
{
}

/** �����������λ��,������������� */
void Camera::setCamera( float positionX, float positionY, float positionZ,
				  		float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ)
{
	/** �������� */
	Vector3 Position	= Vector3(positionX, positionY, positionZ);
	Vector3 View		= Vector3(viewX, viewY, viewZ);
	Vector3 UpVector	= Vector3(upVectorX, upVectorY, upVectorZ);

	/** ��������� */
	m_Position = Position;	
	m_View     = View;			
	m_UpVector = UpVector;	
}


/**  ��ת���������  */
void Camera::rotateView(float angle, float x, float y, float z)
{
	Vector3 newView;

	/** ���㷽������ */
	Vector3 view = m_View - m_Position;		

	/** ���� sin ��cosֵ */
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	/** ������ת������xֵ */
	newView.x  = (cosTheta + (1 - cosTheta) * x * x)		* view.x;
	newView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* view.y;
	newView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* view.z;

	/** ������ת������yֵ */
	newView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* view.x;
	newView.y += (cosTheta + (1 - cosTheta) * y * y)		* view.y;
	newView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* view.z;

	/** ������ת������zֵ */
	newView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* view.x;
	newView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* view.y;
	newView.z += (cosTheta + (1 - cosTheta) * z * z)		* view.z;

	/** ����������ķ��� */
	m_View = m_Position + newView;
}

void Camera::initObstacle()
{	
/*	m_Obstacle[0].x = 739.0f;
	m_Obstacle[0].y = 3.0f;
	m_Obstacle[0].z = -347.0f;

	m_Obstacle[1].x = 718.0f;
	m_Obstacle[1].y = 3.0f;
	m_Obstacle[1].z = -347.0f;*/
}

bool Camera::isObstacle()
{	
	Vector3 v;
	for (int i = 0;i<MAX_OBSTACLE;i++)
	{
		v = m_Obstacle[i]-m_Position;
		if (sqrt(v.dotProduct(v))<2)
		{
			return true;
		}
	}
	return false;
}

/** �������ת����� */
void Camera::setViewByMouse()  
{
	POINT mousePos;	/**< ���浱ǰ���λ�� */
	BOOL flag = TRUE ;
	int middleX = GetSystemMetrics(SM_CXSCREEN) >> 1; /**< �õ���Ļ��ȵ�һ�� */
	int middleY = (GetSystemMetrics(SM_CYSCREEN)+50) >> 1; /**< �õ���Ļ�߶ȵ�һ�� */
	float angleX = 0.0f;							  /**< �����������ת�Ƕ� */
	float angleY = 0.0f;		                      /**< �����������ת�Ƕ� */					
	static float currentRotY = 0.0f;
	
	/** �õ���ǰ���λ�� */
	GetCursorPos(&mousePos);						
	ShowCursor(TRUE);	
	
	/**< �õ�����ƶ����� */
	angleX = (float)( (middleX - mousePos.x) ) / 100000.0f;		//�����1000���������ȣ���ֵԽ��������Խ��
	angleY = (float)( (middleY - mousePos.y) ) / 100000.0f;		

    static float lastRotY = 0.0f;      /**< ���ھ�̬������ת�Ƕ� */
 	lastRotY = currentRotY; 
	
	/** ���������������ת�Ƕ� */
	currentRotY += angleY;
 
	/** ���������ת���ȴ���1.0,���ǽ�ȡ��1.0����ת */
	if(currentRotY > 1.0f)     
	{
		currentRotY = 1.0f;
		
		/** ���ݱ���ĽǶ���ת���� */
		if(lastRotY != 1.0f) 
		{
			/** ͨ������ҵ�����ת����ֱ������ */
			Vector3 vAxis = m_View - m_Position;
			vAxis = vAxis.crossProduct(m_UpVector);
			vAxis = vAxis.normalize();
			
			///��ת
			m_AngleY = 1.0f - lastRotY ;
			rotateView( 1.0f - lastRotY, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	/** �����ת����С��-1.0,��Ҳ��ȡ��-1.0����ת */
	else if(currentRotY < -1.0f)
	{
		currentRotY = -1.0f;
				
		if(lastRotY != -1.0f)
		{
			
			/** ͨ������ҵ�����ת����ֱ������ */
			Vector3 vAxis = m_View - m_Position;
			vAxis = vAxis.crossProduct(m_UpVector);
			vAxis = vAxis.normalize();
			
			///��ת
			m_AngleY = 1.0f - lastRotY ;
			rotateView( -1.0f - lastRotY, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	/** �������תangleY�� */
	else 
	{	
		/** �ҵ�����ת����ֱ���� */
		Vector3 vAxis = m_View - m_Position;
		vAxis = vAxis.crossProduct(m_UpVector);
		vAxis = vAxis.normalize();
	
		///��ת
		m_AngleY = angleY ;
		rotateView(angleY, vAxis.x, vAxis.y, vAxis.z);
	}

	/** ����������ת����� */
	m_AngleX = angleX ;
	rotateView(angleX, 0, 1, 0);
}


/** �����ƶ������ */
void Camera::yawCamera(float speed)
{
	Vector3 yaw;
	Vector3 oldPos,oldView;
	Vector3 cross = m_View - m_Position;
	oldPos = m_Position;
	oldView = m_View;
	cross = cross.crossProduct(m_UpVector);

	//��һ������
	yaw = cross.normalize();
 
	m_Position.x += yaw.x * speed;
	m_Position.z += yaw.z * speed;

	m_View.x += yaw.x * speed;
	m_View.z += yaw.z * speed;
	
	/** ���б߽�����޶� */
	if(m_View.x > MAP_WIDTH - 20)
	{
		m_Position.x = oldPos.x ;
		m_View.x = oldView.x;
	}

	if(m_View.z > MAP_WIDTH - 20 )
	{
		m_Position.z = oldPos.z ;
		m_View.z = oldView.z;
	}
		
	

}

/** ǰ���ƶ������ */
void Camera::moveCamera(float speed)
{
	/** ���㷽������ */
	CString str1000;
	Vector3 vector = m_View - m_Position;
	vector = vector.normalize();         /**< ��λ�� */
	Vector3 oldPos,oldView;
	oldPos = m_Position;
	oldView = m_View;
	m_Position.x += vector.x * speed;    /**< �����ٶȸ���λ�� */
	m_Position.z += vector.z * speed;	
	//m_Position.y += vector.y * speed;


	m_View.x += vector.x * speed;		 /**< �����ٶȸ��·��� */	
	//m_View.y += vector.y * speed;
	m_View.z += vector.z * speed;
	/** ��������� */
	if (isObstacle())
	{
		m_Position.x = oldPos.x ;
		m_View.x = oldView.x;
		m_Position.z = oldPos.z ;
		m_View.z = oldView.z;

	}
	
    /** ���б߽�����޶� */	
	if(m_View.x > MAP_WIDTH - 20 || m_View.x < 20)
	{
		
		m_Position.x = oldPos.x ;
		m_View.x = oldView.x;

										// ����MFC�е��ַ�������
	//	str1000.Format("%f",m_View.x);    // ����Format����������ת�����ַ�������һ���������Ǳ�������
	//	MessageBox(NULL, str1000, "��ʾ", MB_OK);
		
	}

	if(m_View.z > MAP_WIDTH - 20 )
	{
		m_Position.z = oldPos.z ;
		m_View.z = oldView.z;
	}
	

}

/** �����ӵ� */
void Camera::setLook()
{
	
	/** �����ӿ� */
	gluLookAt(m_Position.x, m_Position.y, m_Position.z,	
			  m_View.x,	 m_View.y,     m_View.z,	
			  m_UpVector.x, m_UpVector.y, m_UpVector.z);
}

