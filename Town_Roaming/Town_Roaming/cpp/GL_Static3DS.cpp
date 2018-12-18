// *************************************************************
// 3ds模型的应用：吉普车、热气球
// *************************************************************

#include "..\header\Static3DS.h"


Static3DS* Static3DS::m_pStatic3DS = NULL;

#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

/** 构造函数 */
Static3DS::Static3DS() :jeep_x(1088.0f), jeep_y(0.80f), jeep_z(-1070.0f)
{
	m_pStatic3DS = this;
	g_balloon1.init(970, 30.0f, -860.0f, 20.0f, 20.0f, 20.0f, MY_3DS_Balloon1);
	g_balloon2.init(1030, 30.0f, -860.0f, 20.0f, 20.0f, 20.0f, MY_3DS_Balloon2);
	g_jeep.init(jeep_x, jeep_y, jeep_z, 6.0f, 6.0f, 6.0f, MY_3DS_Jeep);

	m_Angle = 0;
}

Static3DS::~Static3DS()
{
}

bool Static3DS::init()
{
	CLoad3DS g_Load3ds;
	g_Load3ds.init(&g_balloon1.model3DS, MY_3DS_Balloon1);
	g_Load3ds.init(&g_balloon2.model3DS, MY_3DS_Balloon2);
	g_Load3ds.init(&g_jeep.model3DS, MY_3DS_Jeep);
	return TRUE ;
}

bool Static3DS::balloon_render()
{
	g_balloon1.render(0, 90, 0);
	g_balloon2.render(0, 290, 0);
	return true;
}

bool Static3DS::jeep_render(float car_x, float car_y, float car_z,float car_angle)
{
	glPushMatrix();
	glTranslatef(car_x, car_y, car_z);
	glScaled(6.0, 6.0, 6.0);
	glRotatef(0, 1, 0, 0);
	glRotatef(car_angle, 0, 1, 0);
	glRotatef(0, 0, 0, 1);
	glCallList(g_jeep.model3DS.showList);
	glPopMatrix();
	return true ;
}

Vector3 Static3DS::get_jeep_pos()
{
	Vector3 jeep_pos;
	jeep_pos.x = jeep_x;	jeep_pos.y = jeep_y;	jeep_pos.z = jeep_z;
	return jeep_pos;
}

void Static3DS::set_jeep_pos(Vector3 jeep_pos)
{
	jeep_x = jeep_pos.x ; jeep_y = jeep_pos.y; jeep_z = jeep_pos.z ;
}
