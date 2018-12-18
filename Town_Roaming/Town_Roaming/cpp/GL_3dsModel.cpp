// *************************************************************
// 用于构造3ds模型，实现模型初始化（位置、大小、模型对应文件）
// 3ds为静态模型
// *************************************************************

#include "..\header\C3DSModel.h"

C3DSModel* C3DSModel::m_pC3DSModel = NULL;

/** 构造函数 */
C3DSModel::C3DSModel()
{

	m_pC3DSModel = this;
	
}


C3DSModel::~C3DSModel()
{
}

bool C3DSModel::init(float xx,float yy,float zz,float xxScale,float yyScale,float zzScale,char *file)		// 初始化：位置、大小、模型对应文件
{	
	x = xx ;
	y = yy ;
	z = zz ;
	xScale	= xxScale;
	yScale = yyScale ;
	zScale = zzScale ;
	filename = file ;
	return true;
}
bool C3DSModel::render( float degree_x,float degree_y, float degree_z)			// 渲染即显示
{
	glPushMatrix();			// 自己查用法，和glPopMatrix()配套，重定位
	glTranslatef(x,y,z);	// 改变位置
	glScaled(xScale,yScale,zScale);	// 改变大小
	glRotatef(degree_x, 1, 0, 0);	// 旋转模型，x轴
	glRotatef(degree_y, 0, 1, 0);	// 旋转模型，y轴
	glRotatef(degree_z, 0, 0, 1);	// 旋转模型，z轴
	glCallList(model3DS.showList); 
	glPopMatrix();
	return true;
}