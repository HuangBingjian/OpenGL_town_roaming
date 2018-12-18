// *************************************************************
// ���ڹ���3dsģ�ͣ�ʵ��ģ�ͳ�ʼ����λ�á���С��ģ�Ͷ�Ӧ�ļ���
// 3dsΪ��̬ģ��
// *************************************************************

#include "..\header\C3DSModel.h"

C3DSModel* C3DSModel::m_pC3DSModel = NULL;

/** ���캯�� */
C3DSModel::C3DSModel()
{

	m_pC3DSModel = this;
	
}


C3DSModel::~C3DSModel()
{
}

bool C3DSModel::init(float xx,float yy,float zz,float xxScale,float yyScale,float zzScale,char *file)		// ��ʼ����λ�á���С��ģ�Ͷ�Ӧ�ļ�
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
bool C3DSModel::render( float degree_x,float degree_y, float degree_z)			// ��Ⱦ����ʾ
{
	glPushMatrix();			// �Լ����÷�����glPopMatrix()���ף��ض�λ
	glTranslatef(x,y,z);	// �ı�λ��
	glScaled(xScale,yScale,zScale);	// �ı��С
	glRotatef(degree_x, 1, 0, 0);	// ��תģ�ͣ�x��
	glRotatef(degree_y, 0, 1, 0);	// ��תģ�ͣ�y��
	glRotatef(degree_z, 0, 0, 1);	// ��תģ�ͣ�z��
	glCallList(model3DS.showList); 
	glPopMatrix();
	return true;
}