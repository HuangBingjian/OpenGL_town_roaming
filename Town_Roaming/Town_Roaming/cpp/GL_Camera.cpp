// *************************************************************
// 摄像机函数，作品的核心之一
// *************************************************************


#include <afx.h>
#include <string.h>								 /**< 包含向量类 */
#include "..\header\stdafx.h"
#include "..\header\Camera.h"                    /**< 包含摄像机头文件 */
#include "..\header\Vector.h"                    /**< 包含向量类 */
#include "..\header\Font.h"

Camera* Camera::m_pCamera = NULL;

/** 构造函数 */
Camera::Camera()
{
	/** 初始化向量值 */
	Vector3 zero = Vector3(0.0, 0.0, 0.0);		
	Vector3 view = Vector3(0.0, 1.0, 0.5);		
	Vector3 up   = Vector3(0.0, 0.0, 1.0);	
    
	/** 初始化摄像机 */
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

/** 设置摄像机的位置,朝向和向上向量 */
void Camera::setCamera( float positionX, float positionY, float positionZ,
				  		float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ)
{
	/** 构造向量 */
	Vector3 Position	= Vector3(positionX, positionY, positionZ);
	Vector3 View		= Vector3(viewX, viewY, viewZ);
	Vector3 UpVector	= Vector3(upVectorX, upVectorY, upVectorZ);

	/** 设置摄像机 */
	m_Position = Position;	
	m_View     = View;			
	m_UpVector = UpVector;	
}


/**  旋转摄像机方向  */
void Camera::rotateView(float angle, float x, float y, float z)
{
	Vector3 newView;

	/** 计算方向向量 */
	Vector3 view = m_View - m_Position;		

	/** 计算 sin 和cos值 */
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	/** 计算旋转向量的x值 */
	newView.x  = (cosTheta + (1 - cosTheta) * x * x)		* view.x;
	newView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* view.y;
	newView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* view.z;

	/** 计算旋转向量的y值 */
	newView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* view.x;
	newView.y += (cosTheta + (1 - cosTheta) * y * y)		* view.y;
	newView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* view.z;

	/** 计算旋转向量的z值 */
	newView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* view.x;
	newView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* view.y;
	newView.z += (cosTheta + (1 - cosTheta) * z * z)		* view.z;

	/** 更新摄像机的方向 */
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

/** 用鼠标旋转摄像机 */
void Camera::setViewByMouse()  
{
	POINT mousePos;	/**< 保存当前鼠标位置 */
	BOOL flag = TRUE ;
	int middleX = GetSystemMetrics(SM_CXSCREEN) >> 1; /**< 得到屏幕宽度的一半 */
	int middleY = (GetSystemMetrics(SM_CYSCREEN)+50) >> 1; /**< 得到屏幕高度的一半 */
	float angleX = 0.0f;							  /**< 摄像机左右旋转角度 */
	float angleY = 0.0f;		                      /**< 摄像机上下旋转角度 */					
	static float currentRotY = 0.0f;
	
	/** 得到当前鼠标位置 */
	GetCursorPos(&mousePos);						
	ShowCursor(TRUE);	
	
	/**< 得到鼠标移动方向 */
	angleX = (float)( (middleX - mousePos.x) ) / 100000.0f;		//这里的1000代表灵敏度，数值越高灵敏度越低
	angleY = (float)( (middleY - mousePos.y) ) / 100000.0f;		

    static float lastRotY = 0.0f;      /**< 用于静态保存旋转角度 */
 	lastRotY = currentRotY; 
	
	/** 跟踪摄像机上下旋转角度 */
	currentRotY += angleY;
 
	/** 如果上下旋转弧度大于1.0,我们截取到1.0并旋转 */
	if(currentRotY > 1.0f)     
	{
		currentRotY = 1.0f;
		
		/** 根据保存的角度旋转方向 */
		if(lastRotY != 1.0f) 
		{
			/** 通过叉积找到与旋转方向垂直的向量 */
			Vector3 vAxis = m_View - m_Position;
			vAxis = vAxis.crossProduct(m_UpVector);
			vAxis = vAxis.normalize();
			
			///旋转
			m_AngleY = 1.0f - lastRotY ;
			rotateView( 1.0f - lastRotY, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	/** 如果旋转弧度小于-1.0,则也截取到-1.0并旋转 */
	else if(currentRotY < -1.0f)
	{
		currentRotY = -1.0f;
				
		if(lastRotY != -1.0f)
		{
			
			/** 通过叉积找到与旋转方向垂直的向量 */
			Vector3 vAxis = m_View - m_Position;
			vAxis = vAxis.crossProduct(m_UpVector);
			vAxis = vAxis.normalize();
			
			///旋转
			m_AngleY = 1.0f - lastRotY ;
			rotateView( -1.0f - lastRotY, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	/** 否则就旋转angleY度 */
	else 
	{	
		/** 找到与旋转方向垂直向量 */
		Vector3 vAxis = m_View - m_Position;
		vAxis = vAxis.crossProduct(m_UpVector);
		vAxis = vAxis.normalize();
	
		///旋转
		m_AngleY = angleY ;
		rotateView(angleY, vAxis.x, vAxis.y, vAxis.z);
	}

	/** 总是左右旋转摄像机 */
	m_AngleX = angleX ;
	rotateView(angleX, 0, 1, 0);
}


/** 左右移动摄像机 */
void Camera::yawCamera(float speed)
{
	Vector3 yaw;
	Vector3 oldPos,oldView;
	Vector3 cross = m_View - m_Position;
	oldPos = m_Position;
	oldView = m_View;
	cross = cross.crossProduct(m_UpVector);

	//归一化向量
	yaw = cross.normalize();
 
	m_Position.x += yaw.x * speed;
	m_Position.z += yaw.z * speed;

	m_View.x += yaw.x * speed;
	m_View.z += yaw.z * speed;
	
	/** 进行边界检查和限定 */
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

/** 前后移动摄像机 */
void Camera::moveCamera(float speed)
{
	/** 计算方向向量 */
	CString str1000;
	Vector3 vector = m_View - m_Position;
	vector = vector.normalize();         /**< 单位化 */
	Vector3 oldPos,oldView;
	oldPos = m_Position;
	oldView = m_View;
	m_Position.x += vector.x * speed;    /**< 根据速度更新位置 */
	m_Position.z += vector.z * speed;	
	//m_Position.y += vector.y * speed;


	m_View.x += vector.x * speed;		 /**< 根据速度更新方向 */	
	//m_View.y += vector.y * speed;
	m_View.z += vector.z * speed;
	/** 更新摄像机 */
	if (isObstacle())
	{
		m_Position.x = oldPos.x ;
		m_View.x = oldView.x;
		m_Position.z = oldPos.z ;
		m_View.z = oldView.z;

	}
	
    /** 进行边界检查和限定 */	
	if(m_View.x > MAP_WIDTH - 20 || m_View.x < 20)
	{
		
		m_Position.x = oldPos.x ;
		m_View.x = oldView.x;

										// 这是MFC中的字符串变量
	//	str1000.Format("%f",m_View.x);    // 调用Format方法将变量转换成字符串，第一个参数就是变量类型
	//	MessageBox(NULL, str1000, "提示", MB_OK);
		
	}

	if(m_View.z > MAP_WIDTH - 20 )
	{
		m_Position.z = oldPos.z ;
		m_View.z = oldView.z;
	}
	

}

/** 设置视点 */
void Camera::setLook()
{
	
	/** 设置视口 */
	gluLookAt(m_Position.x, m_Position.y, m_Position.z,	
			  m_View.x,	 m_View.y,     m_View.z,	
			  m_UpVector.x, m_UpVector.y, m_UpVector.z);
}

