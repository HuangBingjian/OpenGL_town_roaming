#ifndef __C3DSModel_H__
#define __C3DSModel_H__

#include "stdafx.h"
#include "Vector.h"                 /**< 包含向量类头文件 */
#include "GL_3dsLoder.h"

/** 3DS模型类 */
class C3DSModel
{
public:
	/** 构造函数和析构函数 */

	C3DSModel();
	~C3DSModel();
	bool init(float x,float y,float z,float xScale,float yScale,float zScale,char *filename);
	bool render(float degree_x, float degree_y, float degree_z);



public:
	static C3DSModel  *m_pC3DSModel;    
	t3DModel model3DS;
	float x,y,z;			//模型出现的位置
	float xScale,yScale,zScale;		//模型缩放的比例
	char *filename;

};

#endif //__Static3DS_H__