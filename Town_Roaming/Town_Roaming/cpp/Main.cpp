// *************************************************************
// 主函数内容多，是因为有些功能如音乐（后期加上）没有封装，
// 所以显得繁冗，但是看应该还能看得懂吧。。。
// 算是面向过程而非面向对象，懒得去修改了，自行改进。
// 作为弥补，我把各个过程分块，附上注释，不懂再问
// *************************************************************


// **********************************************************************************************************************
// 头文件
#include "..\header\stdafx.h"		
#include "..\header\CMDLOBJ.h"
#include "..\header\Camera.h"		
#include "..\header\SkyBox.h"
#include "..\header\Ground.h"
#include "..\header\Font.h"
#include "..\header\GL_Base.h"
#include "..\header\Static3DS.h"
#include "..\header\GL_Snow.h"
#include "..\header\GL_Flag.h"
#include "..\header\GL_Firework.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <vector>
#include <gl\gl.h>										
#include <gl\glu.h>		
#include <GL\glut.h>  								
#include <gl\glaux.h>
#include <crtdbg.h>
#include <GL\glext.h>
#include <string>
#include <stdlib.h>
// windows中与多媒体有关的大多数接口（用于音乐）
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib")
// **********************************************************************************************************************


// **********************************************************************************************************************
// 初始化
HDC			hDC=NULL;		// Private GDI Device Context	 OpenGL渲染设备描述表句柄
HGLRC		hRC=NULL;		// Permanent Rendering Context 窗口着色描述表句柄
HWND		hWnd=NULL;		// Holds Our Window Handle	保存我们的窗口句柄
HINSTANCE	hInstance;		// Holds The Instance Of The Application	保存程序的实例

bool	keys[256];			// Array Used For The Keyboard Routine	键盘数组，可处理按键冲突
bool	isKeys[256];		//键盘是否按住,一般用于切换功能
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default	窗口是否激活
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default	是否全屏
bool	focus = true;
char *manual="    移动:\t\t\t    速度:\n"
			 "\tW ↑ \t向前\t\tQ    \t加速\n\tS  ↓\t向后\t\tE   \t减速\n"
			 "\tA ← \t向左\t\tShift\t跑步\n\tD → \t向右\t\tEnter   \t开车\n\n"
			 "    昼夜:\t\t\t    天气:\n"
			 "\t1    \t白天    \t\t4    \t阴天\n\t2    \t黄昏    \t\t5   \t雾天\n"
			 "\t3    \t夜晚    \t\t6    \t下雪\n\n"
			 "    功能:\t\t\t    视野:\n"
			 "\tB    \t回城    \t\tPgUp \t上升\n\tP    \t暂停    \t\tPgDn\t下降\n"
			 "\tL\t街灯    \t\t鼠标左键\t逆时针\n\tM\t音乐    \t\t鼠标右键\t顺时针\n"
			 "\tR\t烟花\n\n"
			 "    信息:\t\t\t    提示:\n"
			 "\tH    \t帮助    \t\t观赏烟花\t白天夜晚\n\tT\t骨骼    \t\t初次寻车\t图书馆后\n"
			 "\tI\t版本\n\tEsc\t退出\n"; 
char *information = "\n\t作者:\t\t笔尖\n";

//int		LBDown; //鼠标左键
float   m_Fps;      // 帧速 
GLFont  m_Font;		//字体
Camera  m_Camera ;	//摄像机类
CSkyBox m_SkyBox;	//天空盒
CGround m_Ground;	//地板
GL_Flag     m_Flag;//旗子
CLoad3DS g_Load3ds;		//3DS载入类								
Static3DS m_Static3DS ;		//静态景物
CMDLOBJ m_MDLOBJ;	//骨骼动画
Snow m_snow;//下雪动画
bool is_snow=0;
bool	m_RenderMode = false;	//绘制模式 填充还是线条
float man_x, man_z,man_degree=0;
int man_action=0;//人物动作
int man_turn = 0;//人物转向
int car_stop = 0;
Vector3 jeep_pos;
Vector3 camera_pos;
float camera_angle = 0;
float camera_rad = float(3.13149f * camera_angle / 180.0f);//移动车的视角
bool jeep_first = 0;//第一次上车
bool man_move=1;//人物是否移动
bool is_music = 0;//0时有声音
bool is_meet = 0;//是否遇到
bool is_drive = 0;//人的视角还是车的视角
float	jeep_angle = 90;//车的角度
int is_night = 0;
float speed_camera = 0.2f;

GLfloat LightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };		//环境光的值RGBA
GLfloat LightDiffuse[] = { 0.2f, 0.2f, 0.2f, 0.2f };		//散射光的值
GLfloat LightPosition[] = { 1000.0f, 50.0f, -2.0f, 1.0f };	//光照位置XYZA

GLuint fogMode[] = { GL_EXP, GL_EXP2, GL_LINEAR };  // 雾气的模式
GLuint fogfilter = 0;     // 使用哪一种雾气
GLfloat fogColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };  // 雾的颜色设为白色
// 定时器回调函数  
void CALLBACK TimeProc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime);

// **********************************************************************************************************************


// **********************************************************************************************************************
/** 计算帧速 */
void CaculateFrameRate()
{
	static float framesPerSecond = 0.0f;	     /**< 保存显示帧数 */
	static float lastTime = 0.0f;	             /**< 记录上次时间 */
	float currentTime = GetTickCount() * 0.001f; /**< 获得当前时间 */
	framesPerSecond++;                           /**< 显示帧数递增1 */
	if (currentTime - lastTime > 1.0f)           /** 如果时间差大于1.0秒 */
	{
		lastTime = currentTime;                  /**< 保存当前时间 */
		m_Fps = framesPerSecond;                 /**< 当前帧数传给m_Fps */
		framesPerSecond = 0;                     /**< 将帧数置零 */
	}
}
// **********************************************************************************************************************

// **********************************************************************************************************************
// 输出屏幕文字
void PrintCurrentText()			
{
	char string[128];                               /**< 用于保存输出字符串 */
	glPushAttrib(GL_CURRENT_BIT);                   /**< 保存现有颜色属性信息 */
	int x, z;
	float car_pos = -1000;//7,1,8,2     9,3,10,4     11,5,12,6
	
	if (man_move == 1)//简化，无需实时监测
	{
		CaculateFrameRate();                          /**< 计算帧速 */
		glColor3f(0.0f, 1.0f, 1.0f);
		sprintf(string, "FPS:%d", (int)m_Fps);        /**< 字符串赋值 */
		m_Font.PrintText(string, -5.0f, 2.0f);

		glColor3f(0.0f, 1.0f, 1.0f);                      /**< 设置文字颜色 */
		if (man_turn == 0)
		{
			sprintf(string, "视角坐标:X=%3.1f Y=%3.1f Z =%3.1f ",
				m_Camera.getPosition().x - 1000, m_Camera.getPosition().y, -800 - m_Camera.getPosition().z); 
			m_Font.PrintText(string, -5.0f, 3.0f);
			x = man_x;
			z = man_z - 10.0f;
		}
		else if (man_turn == 1)
		{
			sprintf(string, "视角坐标:X=%3.1f Y=%3.1f Z =%3.1f ",
				m_Camera.getPosition().x - 1000 -10.0f, m_Camera.getPosition().y, -800 - m_Camera.getPosition().z-10.0f); 
			m_Font.PrintText(string, -5.0f, 3.0f);
			x = man_x - 10.0f;
			z = man_z ;
		}
		else if (man_turn == 2)
		{
			sprintf(string, "视角坐标:X=%3.1f Y=%3.1f Z =%3.1f ",
				m_Camera.getPosition().x - 1000, m_Camera.getPosition().y, -800 - m_Camera.getPosition().z-20.0f);
			m_Font.PrintText(string, -5.0f, 3.0f);
			x = man_x ;
			z = man_z + 10.0f;
		}
		else if (man_turn == 3)
		{
				sprintf(string, "视角坐标:X=%3.1f Y=%3.1f Z =%3.1f ",
					m_Camera.getPosition().x - 1000 + 10.0f, m_Camera.getPosition().y, -800 - m_Camera.getPosition().z-10.0f); 

			m_Font.PrintText(string, -5.0f, 3.0f);
			x = man_x + 10.0f;
			z = man_z;
		}

		if (x >= 990 && x <= 1010 && z <= -805 && z >= -825)
		{
			sprintf(string, "当前位置:\t城镇入口");
			m_Font.PrintText(string, -5.0f, 3.5f);
			sprintf(string, "当前速度:%3.2f   点击H键获取操作方法", m_Camera.getSpeed());
			m_Font.PrintText(string, -5.0f, 2.5f);
		}
		else if (x >= 950 && x <= 990)
		{
			if (z <= -600 && z >= -850)
			{
				sprintf(string, "当前位置:\t城镇左侧 ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -850 && z >= -980)
			{
				sprintf(string, "当前位置:\t欧美风情街道 ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -1000 && z >= -1140)
			{
				sprintf(string, "当前位置:\t办公楼 ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -1160 && z >= -1400)
			{
				sprintf(string, "当前位置:\t居民楼 ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if ((z <= -980 && z >= -1000) || (z <= -1140 && z >= -1160))
			{
				sprintf(string, "当前位置:\t公路上 ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else
			{
				sprintf(string, "当前位置:\t沙滩 ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
		}
		else if (x >= 1010 && x <= 1050)
		{
			if (z <= -600 && z >= -850)
			{
				sprintf(string, "当前位置:\t城镇右侧 ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -850 && z >= -980)
			{
				sprintf(string, "当前位置:\t欧美风情街道 ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -1000 && z >= -1140)
			{
				sprintf(string, "当前位置:\t办公楼 ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -1160 && z >= -1400)
			{
				sprintf(string, "当前位置:\t居民楼 ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if ((z <= -980 && z >= -1000) || (z <= -1140 && z >= -1160))
			{
				sprintf(string, "当前位置:\t公路上 ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else
			{
				sprintf(string, "当前位置:\t沙滩 ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
		}
		else if (x <= 930 && x >= 600)
		{
			if (z <= -600 && z >= -980)
			{
				sprintf(string, "当前位置:\t自然公园 ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -1000 && z >= -1080)
			{
				sprintf(string, "当前位置:\t银行");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -1080 && z >= -1140)
			{
				sprintf(string, "当前位置:\t医院");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -1160 && z >= -1400)
			{
				sprintf(string, "当前位置:\t高级酒店 ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if ((z <= -980 && z >= -1000) || (z <= -1140 && z >= -1160))
			{
				sprintf(string, "当前位置:\t公路上 ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else
			{
				sprintf(string, "当前位置:\t沙滩 ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
		}
		else if (x >= 1070 && x <= 1400)
		{
			if (z <= -600 && z >= -980)
			{
				sprintf(string, "当前位置:\t运动场");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -1000 && z >= -1080)
			{
				sprintf(string, "当前位置:\t图书馆");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -1080 && z >= -1140)
			{
				sprintf(string, "当前位置:\t大超市");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -1160 && z >= -1400)
			{
				sprintf(string, "当前位置:\t小区住宅 ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if ((z <= -980 && z >= -1000) || (z <= -1140 && z >= -1160))
			{
				sprintf(string, "当前位置:\t公路上 ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else
			{
				sprintf(string, "当前位置:\t沙滩 ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
		}
		else if ((x >= 990 && x <= 1010 && z <= -600 && z >= -1400) || (x >= 930 && x <= 950 && z <= -600 && z >= -1400) ||
			(x >= 1050 && x <= 1070 && z <= -600 && z >= -1400) || (x >= 600 && x <= 1400 && z <= -980 && z >= -1000) ||
			(x >= 600 && x <= 1400 && z <= -1140 && z >= -1160))
		{
			sprintf(string, "当前位置:\t公路上 ");
			m_Font.PrintText(string, -5.0f, 3.5f);
			sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
			m_Font.PrintText(string, -5.0f, 2.5f);
		}
		else
		{
			sprintf(string, "当前位置:\t沙滩 ");
			m_Font.PrintText(string, -5.0f, 3.5f);
			sprintf(string, "当前速度:%3.2f", m_Camera.getSpeed());
			m_Font.PrintText(string, -5.0f, 2.5f);
		}
		man_move = 0;
	}
// **********************************************************************************************************************
	

// **********************************************************************************************************************
// 鸣笛声：路上开着的车遇到人后会鸣笛并且停下
	if (is_music == 0)
	{
		if (x >= 931 && x < 935)
		{
			car_pos = m_MDLOBJ.get_car_pos(1);
			if (fabs(z - car_pos) <= 15)
			{
				car_stop = 1;
				if (is_meet == 0)
				{
					mciSendString(TEXT("open data\\sound\\car.mp3 alias car"), NULL, 0, NULL);
					mciSendString(TEXT("play car "), NULL, 0, NULL);
					is_meet = 1;
				}
			}
			else
				car_stop = 0;
		}
		else if (x >= 936 && x < 940)
		{
			car_pos = m_MDLOBJ.get_car_pos(2);
			if (fabs(z - car_pos) <= 15)
			{
				car_stop = 2;
				if (is_meet == 0)
				{
					mciSendString(TEXT("open data\\sound\\car.mp3 alias car"), NULL, 0, NULL);
					mciSendString(TEXT("play car "), NULL, 0, NULL);
					is_meet = 1;
				}
			}
			else
				car_stop = 0;
		}
		else if (x >= 941 && x < 945)
		{
			car_pos = m_MDLOBJ.get_car_pos(3);
			if (fabs(z - car_pos) <= 15)
			{
				car_stop = 3;
				if (is_meet == 0)
				{
					mciSendString(TEXT("open data\\sound\\car.mp3 alias car"), NULL, 0, NULL);
					mciSendString(TEXT("play car "), NULL, 0, NULL);
					is_meet = 1;
				}
			}
			else
				car_stop = 0;
		}
		else if (x >= 946 && x < 950)
		{
			car_pos = m_MDLOBJ.get_car_pos(4);
			if (fabs(z - car_pos) <= 15)
			{
				car_stop = 4;
				if (is_meet == 0)
				{
					mciSendString(TEXT("open data\\sound\\car.mp3 alias car"), NULL, 0, NULL);
					mciSendString(TEXT("play car "), NULL, 0, NULL);
					is_meet = 1;
				}
			}
			else
				car_stop = 0;
		}
		else if (x >= 991 && x < 995)
		{
			car_pos = m_MDLOBJ.get_car_pos(5);
			if (fabs(z - car_pos) <= 15)
			{
				car_stop = 5;
				if (is_meet == 0)
				{
					mciSendString(TEXT("open data\\sound\\car.mp3 alias car"), NULL, 0, NULL);
					mciSendString(TEXT("play car "), NULL, 0, NULL);
					is_meet = 1;
				}
			}
			else
				car_stop = 0;
		}
		else if (x >= 996 && x < 1000)
		{
			car_pos = m_MDLOBJ.get_car_pos(6);
			if (fabs(z - car_pos) <= 15)
			{
				car_stop = 6;
				if (is_meet == 0)
				{
					mciSendString(TEXT("open data\\sound\\car.mp3 alias car"), NULL, 0, NULL);
					mciSendString(TEXT("play car "), NULL, 0, NULL);
					is_meet = 1;
				}
			}
			else
				car_stop = 0;
		}
		else if (x >= 1000 && x < 1004)
		{
			car_pos = m_MDLOBJ.get_car_pos(7);
			if (fabs(z - car_pos) <= 15)
			{
				car_stop = 7;
				if (is_meet == 0)
				{
					mciSendString(TEXT("open data\\sound\\car.mp3 alias car"), NULL, 0, NULL);
					mciSendString(TEXT("play car "), NULL, 0, NULL);
					is_meet = 1;
				}
			}
			else
				car_stop = 0;
		}
		else if (x >= 1005 && x < 1010)
		{
			car_pos = m_MDLOBJ.get_car_pos(8);
			if (fabs(z - car_pos) <= 15)
			{
				car_stop = 8;
				if (is_meet == 0)
				{
					mciSendString(TEXT("open data\\sound\\car.mp3 alias car"), NULL, 0, NULL);
					mciSendString(TEXT("play car "), NULL, 0, NULL);
					is_meet = 1;
				}
			}
			else
				car_stop = 0;
		}
		else if (x >= 1051 && x < 1055)
		{
			car_pos = m_MDLOBJ.get_car_pos(9);
			if (fabs(z - car_pos) <= 15)
			{
				car_stop = 9;
				if (is_meet == 0)
				{
					mciSendString(TEXT("open data\\sound\\car.mp3 alias car"), NULL, 0, NULL);
					mciSendString(TEXT("play car "), NULL, 0, NULL);
					is_meet = 1;
				}
			}
			else
				car_stop = 0;
		}
		else if (x >= 1056 && x < 1060)
		{
			car_pos = m_MDLOBJ.get_car_pos(10);
			if (fabs(z - car_pos) <= 15)
			{
				car_stop = 10;
				if (is_meet == 0)
				{
					mciSendString(TEXT("open data\\sound\\car.mp3 alias car"), NULL, 0, NULL);
					mciSendString(TEXT("play car "), NULL, 0, NULL);
					is_meet = 1;
				}
			}
			else
				car_stop = 0;
		}
		else if (x >= 1060 && x < 1065)
		{
			car_pos = m_MDLOBJ.get_car_pos(11);
			if (fabs(z - car_pos) <= 15)
			{
				car_stop = 11;
				if (is_meet == 0)
				{
					mciSendString(TEXT("open data\\sound\\car.mp3 alias car"), NULL, 0, NULL);
					mciSendString(TEXT("play car "), NULL, 0, NULL);
					is_meet = 1;
				}
			}
			else
				car_stop = 0;
		}
		else if (x >= 1065 && x < 1070)
		{
			car_pos = m_MDLOBJ.get_car_pos(12);
			if (fabs(z - car_pos) <= 15)
			{
				car_stop = 12;
				if (is_meet == 0)
				{
					mciSendString(TEXT("open data\\sound\\car.mp3 alias car"), NULL, 0, NULL);
					mciSendString(TEXT("play car "), NULL, 0, NULL);
					is_meet = 1;
				}
			}
			else
				car_stop = 0;
		}
		else
		{
			car_pos = -10000;
			car_stop = 0;
			is_meet = 0;
			mciSendString(TEXT("close car"), NULL, 0, NULL);
		}
	}
	else
		mciSendString(TEXT("pause car"), NULL, 0, NULL);

	glPopAttrib();
}
// **********************************************************************************************************************


// **********************************************************************************************************************
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
/** 更新摄像机 */
void UpdateCamera()
{
	if(is_drive == 0)
		m_Camera.setViewByMouse();

	/** 键盘按键响应 */
	if(keys[VK_SHIFT])                        /**< 按下SHIFT键时加速 */
	{
		if (is_drive == 0)	m_Camera.setSpeed(0.5f);
	}
	if(!keys[VK_SHIFT])
	{
		m_Camera.setSpeed(speed_camera);
	}
	if (keys['Q'])                        /**< 按下Q键时加速 */
	{
		speed_camera = m_Camera.getSpeed();
		if (speed_camera < 0.49f)
		{
			speed_camera += 0.01f;
			m_Camera.setSpeed(speed_camera);
		}
	}
	if (keys['E'])                        /**< 按下E键时减速 */
	{
		speed_camera = m_Camera.getSpeed();
		if (speed_camera > 0.10)
		{
			speed_camera -= 0.01f;
			m_Camera.setSpeed(speed_camera);
		}
	}
	if (keys['H'] && !isKeys['H'])                        /**< 按下H键时 */
	{
		isKeys['H'] = true;
		MessageBox(NULL, manual	, "操作手册", MB_OK); 
	}
	if (!keys['H'])
	{
		isKeys['H'] = false;
	}
	if (keys['I'] && !isKeys['I'])                        /**< 按下I键时 */
	{
		isKeys['I'] = true;
		MessageBox(NULL,information, "版本信息",MB_OK);
	}
	if (!keys['I'])                        /**< 按下I键时 */
	{
		isKeys['I'] = false;
	}
	if (keys[VK_UP]  || keys['W'] )   /**< 向上方向键或'W'键按下 */
	{
		m_Camera.moveCamera(m_Camera.getSpeed());          /**< 移动摄像机 */
		if (is_drive == 0)
		{
			if (man_turn == 0)	man_degree = 0; else	if (man_turn == 1)	man_degree = 90;
			else	if (man_turn == 2)	man_degree = 180; else	if (man_turn == 3)	man_degree = 270;
			if (m_Camera.getSpeed() >= 0.3)
				man_action = 2;
			else
				man_action = 1;
		}
		else
		{
			m_Camera.setSpeed(1.0f);
			jeep_pos.x = m_Static3DS.get_jeep_pos().x - (float)cos(camera_rad) * 1;
			jeep_pos.y = m_Static3DS.get_jeep_pos().y;
			jeep_pos.z = m_Static3DS.get_jeep_pos().z - (float)sin(camera_rad) * 1;
			m_Static3DS.set_jeep_pos(jeep_pos);
			m_Camera.setCamera(m_Static3DS.get_jeep_pos().x + (float)cos(camera_rad) * 10, 3.0, m_Static3DS.get_jeep_pos().z + (float)sin(camera_rad) * 10,
				m_Static3DS.get_jeep_pos().x , 2.0, m_Static3DS.get_jeep_pos().z , 0, 1, 0);
		}
	}
	if (keys[VK_DOWN] || keys['S']) /**< 向下方向键或'S'键按下 */
	{
		m_Camera.moveCamera(-m_Camera.getSpeed());         /**< 移动摄像机 */
		if (is_drive == 0)
		{
			if (man_turn == 0)	man_degree = 180; else	if (man_turn == 1)	man_degree = 270;
			else	if (man_turn == 2)	man_degree = 0; else	if (man_turn == 3)	man_degree = 90;
			if (m_Camera.getSpeed() >= 0.5)
				man_action = 2;
			else
				man_action = 1;
		}
		else
		{
			m_Camera.setSpeed(1.0f);
			jeep_pos.x = m_Static3DS.get_jeep_pos().x + (float)cos(camera_rad) * 1;
			jeep_pos.y = m_Static3DS.get_jeep_pos().y;
			jeep_pos.z = m_Static3DS.get_jeep_pos().z + (float)sin(camera_rad) * 1;
			m_Static3DS.set_jeep_pos(jeep_pos);
			m_Camera.setCamera(m_Static3DS.get_jeep_pos().x + (float)cos(camera_rad) * 10, 3.0, m_Static3DS.get_jeep_pos().z + (float)sin(camera_rad) * 10,
				m_Static3DS.get_jeep_pos().x, 2.0, m_Static3DS.get_jeep_pos().z, 0, 1, 0);
		}
	}
	if (keys[VK_LEFT] || keys['A'] ) /**< 向左方向键或'A'键按下 */
	{
		if (is_drive == 0)
		{
			m_Camera.yawCamera(-m_Camera.getSpeed());
			if (man_turn == 0)	man_degree = 90; else	if (man_turn == 1)	man_degree = 180;
			else	if (man_turn == 2)	man_degree = 270; else	if (man_turn == 3)	man_degree = 0;
			if (m_Camera.getSpeed() >= 0.5)
				man_action = 2;
			else
				man_action = 1;
		}
		else
		{
			m_Camera.setSpeed(1.0f);
			jeep_angle += 0.8;	  camera_angle -= 0.8;
			camera_rad = float(3.13149f * camera_angle / 180.0f);
			m_Camera.setCamera(m_Static3DS.get_jeep_pos().x+ (float)cos(camera_rad)*10, 3.0, m_Static3DS.get_jeep_pos().z + (float)sin(camera_rad) * 10,
				m_Static3DS.get_jeep_pos().x, 2.0, m_Static3DS.get_jeep_pos().z, 0, 1, 0);

		}
	}
	if (keys[VK_RIGHT] || keys['D'] ) /**< 向右方向键或'D'键按下 */
	{
		if (is_drive == 0)
		{
			m_Camera.yawCamera(m_Camera.getSpeed());            /**< 移动摄像机 */
			if (man_turn == 0)	man_degree = 270; else	if (man_turn == 1)	man_degree = 0;
			else	if (man_turn == 2)	man_degree = 90; else	if (man_turn == 3)	man_degree = 180;
			if (m_Camera.getSpeed() >= 0.5)
				man_action = 2;
			else
				man_action = 1;
		}
		else
		{
			m_Camera.setSpeed(1.0f);
			jeep_angle -= 0.8;    camera_angle += 0.8;
			camera_rad = float(3.13149f * camera_angle / 180.0f);
			m_Camera.setCamera(m_Static3DS.get_jeep_pos().x + (float)cos(camera_rad) * 10, 3.0, m_Static3DS.get_jeep_pos().z + (float)sin(camera_rad) * 10,
				m_Static3DS.get_jeep_pos().x, 2.0, m_Static3DS.get_jeep_pos().z, 0, 1, 0);
		}
	}

	if (!keys[VK_UP] && !keys['W'] && !keys[VK_DOWN] && !keys['S'] 
		&&!keys[VK_LEFT] && !keys['A'] && !keys[VK_RIGHT] &&  !keys['D'])
	{
		man_action = 0;
	}

	if (keys['L'] && !isKeys['L'])                        /**< 按下L键时灯亮灭 */
	{
		isKeys['L'] = true;
		m_Ground.setlight();
	}
	if (!keys['L']) 
		isKeys['L'] = false;

// **********************************************************************************************************************
// 天气、音乐等
	if (keys['1'])                        /**< 按下1键时白天 */
	{
		if (is_music == 0)
		{
			if (is_night == 1)
			{
				mciSendString(TEXT("pause night"), NULL, 0, NULL);				mciSendString(TEXT("play day repeat"), NULL, 0, NULL);
			}
			else if (is_night == 2)
			{
				mciSendString(TEXT("pause dusk"), NULL, 0, NULL);				mciSendString(TEXT("play day repeat"), NULL, 0, NULL);
			}
			else if (is_night == 3)
			{
				mciSendString(TEXT("pause sunday"), NULL, 0, NULL);				mciSendString(TEXT("play day repeat"), NULL, 0, NULL);
			}
			else if (is_night == 4)
			{
				mciSendString(TEXT("pause mist"), NULL, 0, NULL);				mciSendString(TEXT("play day repeat"), NULL, 0, NULL);
			}
			else if (is_night == 5)
			{
				mciSendString(TEXT("pause snow"), NULL, 0, NULL);				mciSendString(TEXT("play day repeat"), NULL, 0, NULL);
			}
		}
		else
		{
			mciSendString(TEXT("pause day"), NULL, 0, NULL);			mciSendString(TEXT("pause night"), NULL, 0, NULL);
			mciSendString(TEXT("pause dusk"), NULL, 0, NULL);			mciSendString(TEXT("pause sunday"), NULL, 0, NULL);
			mciSendString(TEXT("pause mist"), NULL, 0, NULL);			mciSendString(TEXT("pause snow"), NULL, 0, NULL);
		}
		glDisable(GL_LIGHTING);
		glDisable(GL_FOG);
		is_snow = 0;
		is_night = 0;
		m_Ground.closelight();
	}
	if (keys['2'])                        /**< 按下2键时傍晚 */
	{
		if (is_music == 0)
		{
			if (is_night == 0)
			{
				mciSendString(TEXT("pause day"), NULL, 0, NULL);				mciSendString(TEXT("play dusk repeat"), NULL, 0, NULL);
			}
			else if (is_night == 1)
			{
				mciSendString(TEXT("pause night"), NULL, 0, NULL);				mciSendString(TEXT("play dusk repeat"), NULL, 0, NULL);
			}
			else if (is_night == 3)
			{
				mciSendString(TEXT("pause sunday"), NULL, 0, NULL);				mciSendString(TEXT("play dusk repeat"), NULL, 0, NULL);
			}
			else if (is_night == 4)
			{
				mciSendString(TEXT("pause mist"), NULL, 0, NULL);				mciSendString(TEXT("play dusk repeat"), NULL, 0, NULL);
			}
			else if (is_night == 5)
			{
				mciSendString(TEXT("pause snow"), NULL, 0, NULL);				mciSendString(TEXT("play dusk repeat"), NULL, 0, NULL);
			}
		}
		else
		{
			mciSendString(TEXT("pause day"), NULL, 0, NULL);			mciSendString(TEXT("pause night"), NULL, 0, NULL);
			mciSendString(TEXT("pause dusk"), NULL, 0, NULL);			mciSendString(TEXT("pause sunday"), NULL, 0, NULL);
			mciSendString(TEXT("pause mist"), NULL, 0, NULL);			mciSendString(TEXT("pause snow"), NULL, 0, NULL);
		}
		glDisable(GL_LIGHTING);
		glDisable(GL_FOG);
		is_snow = 0;
		is_night = 2;
		m_Ground.closelight();
	}
	if (keys['3'])                        /**< 按下3键时黑夜 */
	{
		if (is_music == 0)
		{
			if (is_night == 0)
			{
				mciSendString(TEXT("pause day"), NULL, 0, NULL);				mciSendString(TEXT("play night repeat"), NULL, 0, NULL);
			}
			else if (is_night == 2)
			{
				mciSendString(TEXT("pause dusk"), NULL, 0, NULL);				mciSendString(TEXT("play night repeat"), NULL, 0, NULL);
			}
			else if (is_night == 3)
			{
				mciSendString(TEXT("pause sunday"), NULL, 0, NULL);				mciSendString(TEXT("play night repeat"), NULL, 0, NULL);
			}
			else if (is_night == 4)
			{
				mciSendString(TEXT("pause mist"), NULL, 0, NULL);				mciSendString(TEXT("play night repeat"), NULL, 0, NULL);
			}
			else if (is_night == 5)
			{
				mciSendString(TEXT("pause snow"), NULL, 0, NULL);				mciSendString(TEXT("play night repeat"), NULL, 0, NULL);
			}
		}
		else
		{
			mciSendString(TEXT("pause day"), NULL, 0, NULL);			mciSendString(TEXT("pause night"), NULL, 0, NULL);
			mciSendString(TEXT("pause dusk"), NULL, 0, NULL);			mciSendString(TEXT("pause sunday"), NULL, 0, NULL);
			mciSendString(TEXT("pause mist"), NULL, 0, NULL);			mciSendString(TEXT("pause snow"), NULL, 0, NULL);
		}
		FireInit();
	//	glDisable(GL_LIGHTING);
		glEnable(GL_LIGHTING);
		glDisable(GL_FOG);
		is_snow = 0;
		is_night = 1;
		m_Ground.openlight();
	}
	if (keys['4'])                        /**< 晴天 */
	{
		if (is_music == 0)
		{
			if (is_night == 0)
			{
				mciSendString(TEXT("pause day"), NULL, 0, NULL);				mciSendString(TEXT("play sunday repeat"), NULL, 0, NULL);
			}
			else if (is_night == 1)
			{
				mciSendString(TEXT("pause night"), NULL, 0, NULL);				mciSendString(TEXT("play sunday repeat"), NULL, 0, NULL);
			}
			else if (is_night == 2)
			{
				mciSendString(TEXT("pause dusk"), NULL, 0, NULL);				mciSendString(TEXT("play sunday repeat"), NULL, 0, NULL);
			}
			else if (is_night == 4)
			{
				mciSendString(TEXT("pause mist"), NULL, 0, NULL);				mciSendString(TEXT("play sunday repeat"), NULL, 0, NULL);
			}
			else if (is_night == 5)
			{
				mciSendString(TEXT("pause snow"), NULL, 0, NULL);				mciSendString(TEXT("play sunday repeat"), NULL, 0, NULL);
			}
		}
		else
		{
			mciSendString(TEXT("pause day"), NULL, 0, NULL);			mciSendString(TEXT("pause night"), NULL, 0, NULL);
			mciSendString(TEXT("pause dusk"), NULL, 0, NULL);			mciSendString(TEXT("pause sunday"), NULL, 0, NULL);
			mciSendString(TEXT("pause mist"), NULL, 0, NULL);			mciSendString(TEXT("pause snow"), NULL, 0, NULL);
		}

		glDisable(GL_LIGHTING);
		glDisable(GL_FOG);
		is_snow = 0;
		is_night = 3;
		m_Ground.closelight();
	}
	if (keys['5'])							/**< 雾天 */
	{
		glDisable(GL_LIGHTING);
		if (is_music == 0)
		{
			if (is_night == 0)
			{
				mciSendString(TEXT("pause day"), NULL, 0, NULL);				mciSendString(TEXT("play mist repeat"), NULL, 0, NULL);
			}
			else if (is_night == 1)
			{
				mciSendString(TEXT("pause night"), NULL, 0, NULL);				mciSendString(TEXT("play mist repeat"), NULL, 0, NULL);
			}
			else if (is_night == 2)
			{
				mciSendString(TEXT("pause dusk"), NULL, 0, NULL);				mciSendString(TEXT("play mist repeat"), NULL, 0, NULL);
			}
			else if (is_night == 3)
			{
				mciSendString(TEXT("pause sunday"), NULL, 0, NULL);				mciSendString(TEXT("play mist repeat"), NULL, 0, NULL);
			}
			else if (is_night == 5)
			{
				mciSendString(TEXT("pause snow"), NULL, 0, NULL);				mciSendString(TEXT("play mist repeat"), NULL, 0, NULL);
			}
		}
		else
		{
			mciSendString(TEXT("pause day"), NULL, 0, NULL);			mciSendString(TEXT("pause night"), NULL, 0, NULL);
			mciSendString(TEXT("pause dusk"), NULL, 0, NULL);			mciSendString(TEXT("pause sunday"), NULL, 0, NULL);
			mciSendString(TEXT("pause mist"), NULL, 0, NULL);			mciSendString(TEXT("pause snow"), NULL, 0, NULL);
		}
		glEnable(GL_FOG);
		is_snow = 0;
		is_night = 4;
		m_Ground.closelight();
	}
	if (keys['6'])                        /**< 下雪 */
	{
		glDisable(GL_LIGHTING);
		if (is_music == 0)
		{
			if (is_night == 0)
			{
				mciSendString(TEXT("pause day"), NULL, 0, NULL);				mciSendString(TEXT("play snow repeat"), NULL, 0, NULL);
			}
			else if (is_night == 1)
			{
				mciSendString(TEXT("pause night"), NULL, 0, NULL);				mciSendString(TEXT("play snow repeat"), NULL, 0, NULL);
			}
			else if (is_night == 2)
			{
				mciSendString(TEXT("pause dusk"), NULL, 0, NULL);				mciSendString(TEXT("play snow repeat"), NULL, 0, NULL);
			}
			else if (is_night == 3)
			{
				mciSendString(TEXT("pause sunday"), NULL, 0, NULL);				mciSendString(TEXT("play snow repeat"), NULL, 0, NULL);
			}
			else if (is_night == 4)
			{
				mciSendString(TEXT("pause mist"), NULL, 0, NULL);				mciSendString(TEXT("play snow repeat"), NULL, 0, NULL);
			}
		}
		else
		{
			mciSendString(TEXT("pause day"), NULL, 0, NULL);			mciSendString(TEXT("pause night"), NULL, 0, NULL);
			mciSendString(TEXT("pause dusk"), NULL, 0, NULL);			mciSendString(TEXT("pause sunday"), NULL, 0, NULL);
			mciSendString(TEXT("pause mist"), NULL, 0, NULL);			mciSendString(TEXT("pause snow"), NULL, 0, NULL);
		}
		glDisable(GL_FOG);
		is_snow = 1;
		is_night = 5;
		m_Ground.closelight();
	}

	if (keys['M'] && !isKeys['M'])                        /**< 按下M键时 */
	{
		isKeys['M'] = true;
		is_music = !is_music;
		if (is_music == 1)
		{
			if (is_night == 0)		mciSendString(TEXT("pause day"), NULL, 0, NULL);
			else if (is_night == 1)	mciSendString(TEXT("pause night"), NULL, 0, NULL);
			else if (is_night == 2)	mciSendString(TEXT("pause dusk"), NULL, 0, NULL);
			else if (is_night == 3)	mciSendString(TEXT("pause sunday"), NULL, 0, NULL);
			else if (is_night == 4)	mciSendString(TEXT("pause mist"), NULL, 0, NULL);
			else if (is_night == 5)	mciSendString(TEXT("pause snow"), NULL, 0, NULL);
		}
		else
		{
			if (is_night == 0)		mciSendString(TEXT("resume day"), NULL, 0, NULL);
			else if (is_night == 1)	mciSendString(TEXT("resume night"), NULL, 0, NULL);
			else if (is_night == 2)	mciSendString(TEXT("resume dusk"), NULL, 0, NULL);
			else if (is_night == 3)	mciSendString(TEXT("resume sunday"), NULL, 0, NULL);
			else if (is_night == 4)	mciSendString(TEXT("resume mist"), NULL, 0, NULL);
			else if (is_night == 5)	mciSendString(TEXT("resume snow"), NULL, 0, NULL);
		}
	}
	if (!keys['M'])                        /**< 按下I键时 */
	{
		isKeys['M'] = false;
	}
	if (keys['P'] && !isKeys['P'])                        /**< 按下P键时 */
	{
		isKeys['P'] = true;
		{
			if (is_night == 0)
			{
				KillTimer(NULL, 1);
				mciSendString(TEXT("pause day"), NULL, 0, NULL);	MessageBox(NULL, "点击继续", "暂停", MB_OK);
				if (is_music == 0)									mciSendString(TEXT("resume day"), NULL, 0, NULL);
				SetTimer(NULL, 1, 20, TimeProc);
			}
			else if (is_night == 1)
			{
				KillTimer(NULL, 1);
				mciSendString(TEXT("pause night"), NULL, 0, NULL);	MessageBox(NULL, "点击继续", "暂停", MB_OK);
				if (is_music == 0)									mciSendString(TEXT("resume night"), NULL, 0, NULL);
				SetTimer(NULL, 1, 20, TimeProc);
			}
			else if (is_night == 2)
			{
				KillTimer(NULL, 1);
				mciSendString(TEXT("pause dusk"), NULL, 0, NULL);	MessageBox(NULL, "点击继续", "暂停", MB_OK);
				if (is_music == 0)									mciSendString(TEXT("resume dusk"), NULL, 0, NULL);
				SetTimer(NULL, 1, 20, TimeProc);
			}
			else if (is_night == 3)
			{
				KillTimer(NULL, 1);
				mciSendString(TEXT("pause sunday"), NULL, 0, NULL);	MessageBox(NULL, "点击继续", "暂停", MB_OK);
				if (is_music == 0)									mciSendString(TEXT("resume sunday"), NULL, 0, NULL);
			}
			else if (is_night == 4)
			{
				KillTimer(NULL, 1);
				mciSendString(TEXT("pause mist"), NULL, 0, NULL);	MessageBox(NULL, "点击继续", "暂停", MB_OK);
				if (is_music == 0)									mciSendString(TEXT("resume mist"), NULL, 0, NULL);
			}
			else if (is_night == 5)
			{
				KillTimer(NULL, 1);
				mciSendString(TEXT("pause snow"), NULL, 0, NULL);	MessageBox(NULL, "点击继续", "暂停", MB_OK);
				if (is_music == 0)									mciSendString(TEXT("resume snow"), NULL, 0, NULL);
			}
		}
	}

// **********************************************************************************************************************


// **********************************************************************************************************************
	if (!keys['P'])
	{
		isKeys['P'] = false;
	}
	if ( keys['T'] && !isKeys['T']) /**< 'T'键按下 */
	{
		isKeys['T'] = true;
		m_RenderMode = !m_RenderMode; /*切换绘制模式 */
		if (m_RenderMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}	
	if (!keys['T'])                        
	{
		isKeys['T'] = false;
	}
	if (keys['B']) /**< 'B'键按下 */
	{
		m_Camera.setCamera(1000, 3, -800, 1000, 3, -800 - 1, 0, 1, 0);	//XZ初始向量貌似只能差1
		man_turn = 0;
		man_degree = 0;
		is_drive = 0;

		if (jeep_first == 1)
		{
			jeep_pos.x = 1030;
			jeep_pos.y = m_Static3DS.get_jeep_pos().y;
			jeep_pos.z = -830;
			m_Static3DS.set_jeep_pos(jeep_pos);
			mciSendString(TEXT("close jeep"), NULL, 0, NULL);
		}
	}
	if (keys['R'] && !isKeys['R'])                        //< 按下R键时放烟花 
	{
		isKeys['R'] = true;
		if(is_night==0 || is_night ==1)
			FireInit();//重置烟花
	}
	if (!keys['R'] )                       
		isKeys['R'] = false;

	if (keys[VK_PRIOR])   /**< PageUp键按下 */
	{
		Vector3 v_Pos = m_Camera.getPosition();         /**< 视线高度上升 */
		Vector3 v_View = m_Camera.getView();
		if (v_Pos.y < 40)
		{
			v_Pos.y += 0.5;
			v_View.y += 0.5;
			m_Camera.setCamera(v_Pos.x, v_Pos.y, v_Pos.z, v_View.x, v_View.y, v_View.z, 0, 1, 0);
		}
	}
	if (keys[VK_NEXT])   /**< PagezDown键按下 */
	{
		Vector3 v_Pos = m_Camera.getPosition();         /**< 视线高度下降 */
		Vector3 v_View = m_Camera.getView();
		if (v_Pos.y > 1.5)
		{
			v_Pos.y -= 0.5;
			v_View.y -= 0.5;
			m_Camera.setCamera(v_Pos.x, v_Pos.y, v_Pos.z, v_View.x, v_View.y, v_View.z, 0, 1, 0);
		}
	}
	if (keys[VK_RETURN] && !isKeys[VK_RETURN])   //回车键
	{
		isKeys[VK_RETURN] = true;
		if (is_drive == 0)
		{
			if (fabs(m_Static3DS.get_jeep_pos().x - man_x) < 15 && fabs(m_Static3DS.get_jeep_pos().z - man_z) < 15)
			{
				//加入启动引擎声
				mciSendString(TEXT("open data\\sound\\jeep.mp3 alias jeep"), NULL, 0, NULL);
				mciSendString(TEXT("play jeep "), NULL, 0, NULL);
				if (jeep_first == 0)
				{
					jeep_first = 1;	
					m_Camera.setCamera(1098.0f, 3.0f, -1070.0f, 1087.0f, 2.0f, -1070.0f, 0, 1, 0);
				}
				is_drive = 1;
			}
		}
		else
		{
			mciSendString(TEXT("close jeep"), NULL, 0, NULL);
			is_drive = 0;
			man_turn = 0 , man_degree = 180;
			m_Camera.setCamera(m_Static3DS.get_jeep_pos().x, 3.0, m_Static3DS.get_jeep_pos().z+13, 
				m_Static3DS.get_jeep_pos().x, 3.0, m_Static3DS.get_jeep_pos().z-12, 0, 1, 0);
		}
	}
	if (!keys[VK_RETURN])
	{
		isKeys[VK_RETURN] = false;
	}
	/** 根据地形高度更新摄像机 */
	Vector3 vPos = m_Camera.getPosition();                  /**< 得到当前摄像机位置 */
	Vector3 vNewPos = vPos; 
	man_x = vNewPos.x;
	man_z = vNewPos.z;

	/** 设置摄像机高度为 地形高度 + 10 */
//	vNewPos.y = (float)m_Terrain.getAveHeight(vPos.x,vPos.z ) + 10;
	//vNewPos.y =5.0f; // 你站的位置高度
	/** 得到高度差值 */
//	float temp = vNewPos.y - vPos.y;

	/** 更新摄像机方向 */
	Vector3 vView = m_Camera.getView();
	//vView.y += 0.0f;

	/** 设置摄像机 */
	m_Camera.setCamera(vNewPos.x,  vNewPos.y,  vNewPos.z,
		vView.x,	   vView.y,	   vView.z,	 
		0, 1, 0);								

}
// **********************************************************************************************************************


// **********************************************************************************************************************

/** 程序更新函数 */
void Update()						
{
	/** 用户自定义的更新过程 */

	/** 更新摄像机 */
	UpdateCamera();

	m_Flag.Update();
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window	重新设置OpenGL场景的大小
{
	if (height==0)										// Prevent A Divide By Zero By	 防止被零除
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport	重建当前窗口
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix	选择投影矩阵
	glLoadIdentity();									// Reset The Projection Matrix	将当前矩阵恢复成原始状态

	// Calculate The Aspect Ratio Of The Window	计算窗口横纵比
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.05f,4000.0f);	//设置透视投影矩阵
	//gluPerspective(
	//角度,
	//视景体的宽高比,
	//沿z轴方向的两裁面之间的距离的近处（深度起点）,
	//沿z轴方向的两裁面之间的距离的远处（深度终点）);
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix	选择模型观察矩阵	
	glLoadIdentity();									// Reset The Modelview Matrix	重置当前矩阵
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
//	glEnable(GL_LIGHTING);
//	glDisable(GL_LIGHTING);
	
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading	启用阴影平滑
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background	设为黑色
//	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);// 设置背景的颜色为雾气的颜色

	////////////NEW ///////////////////
	m_Camera.setCamera(1000, 3, -800, 1000, 3, -800 - 1, 0, 1, 0);	//XZ初始向量貌似只能差1

	///////////NEW ///////////////////
	//glClearColor(红, 绿, 蓝, 透明度);
	glClearDepth(1.0f);									// Depth Buffer Setup	启用深度缓存
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing	启用深度测试
	
//	glEnable(GL_LIGHT0);
//	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do	深度测试的类型	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations	透视修正
	glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);

	//设置光源参数
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		//设置环境光
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		//设置漫反射光
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);		//光源的位置
	glEnable(GL_LIGHT1);								//允许光源一
	
	glFogi(GL_FOG_MODE, fogMode[fogfilter]);  // 设置雾气的模式
	glFogfv(GL_FOG_COLOR, fogColor);   // 设置雾的颜色
	glFogf(GL_FOG_DENSITY, 0.005f);   // 设置雾的密度
	glHint(GL_FOG_HINT, GL_DONT_CARE);   // 设置系统如何计算雾气
	glFogf(GL_FOG_START, 1.0f);    // 雾气的开始位置
	glFogf(GL_FOG_END, 5.0f);    // 雾气的结束位置
//	glEnable(GL_FOG);     // 使用雾气

	//初始化各种模型
	m_MDLOBJ.init(&m_Camera);
	

	/** 初始化字体 */
	if(!m_Font.InitFont())
		MessageBox(NULL,"初始化字体失败!","错误",MB_OK);

	if(!m_Ground.init())
	{
		MessageBox(NULL,"初始化地板失败!","错误",MB_OK);
		exit(0);
	}
	/** 初始化天空 */
	if(!m_SkyBox.init())
	{
		MessageBox(NULL,"初始化天空失败!","错误",MB_OK);
		exit(0);
	}

	if(!m_Static3DS.init())
	{ 
		MessageBox(NULL,"初始化静态模型失败!","错误",MB_OK);
		exit(0);

	}

	if (!m_snow.Init(5000))
	{
		MessageBox(NULL, "初始化下雪失败!", "错误", MB_OK);
		exit(0);
	}
	m_snow.Load("data\\textures\\snow.bmp", m_snow.m_texture);	       // 制造雪

	mciSendString(TEXT("open data\\sound\\day.mp3 alias day"), NULL, 0, NULL);
	mciSendString(TEXT("open data\\sound\\night.mp3 alias night"), NULL, 0, NULL);
	mciSendString(TEXT("open data\\sound\\dusk.mp3 alias dusk"), NULL, 0, NULL);
	mciSendString(TEXT("open data\\sound\\sunday.mp3 alias sunday"), NULL, 0, NULL);
	mciSendString(TEXT("open data\\sound\\mist.mp3 alias mist"), NULL, 0, NULL);
	mciSendString(TEXT("open data\\sound\\snow.mp3 alias snow"), NULL, 0, NULL);
	mciSendString(TEXT("play day repeat"), NULL, 0, NULL);

	
	return TRUE;										// Initialization Went OK
}

int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{	
	//Sleep(20);	//延时20毫秒
	Vector3 vAxis = m_Camera.m_View - m_Camera.m_Position;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer	清屏和清除深度缓存
	//glEnable(GL_LIGHTING);
	glLoadIdentity();
	
	SetTimer(NULL,1, 1000, TimeProc);

	m_Camera.setLook();// 设置相机位置
		
	m_SkyBox.render(is_night);			//渲染天空

	m_Ground.render();			//渲染地板

	m_Flag.render(m_Ground.m_texture[109], m_Ground.m_texture[112], 880, 15, -910);//旗子
	m_Flag.render(m_Ground.m_texture[110], m_Ground.m_texture[112], 880, 14, -900);
	m_Flag.render(m_Ground.m_texture[111], m_Ground.m_texture[112], 880, 14, -920);

	if(is_night==2)		m_Static3DS.balloon_render();	//输出静态3DS
	
	if(is_snow==1)		m_snow.Render();//下雪

	m_Static3DS.jeep_render(m_Static3DS.get_jeep_pos().x, 1.2, m_Static3DS.get_jeep_pos().z, jeep_angle);//吉普车

	m_MDLOBJ.render();//MDL人物模型
		
	if (is_night == 0 || is_night == 1)	FireWorks();//烟花（白天夜晚）

	if (is_drive == 0)
		m_MDLOBJ.man_render(man_turn, man_x, man_z, man_degree, man_action);//人物位置及方向

	PrintCurrentText();

	glFlush();
	
	return TRUE;										// Everything Went OK
}


void CALLBACK TimeProc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{	
	man_move = 1;

	m_MDLOBJ.move(car_stop);
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window	正常关闭窗口
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop	返回原始桌面 0的意思是从注册表读出桌面的配置
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
														//wglMakeCurrent( HDC hdc, HGLRC hglrc)；
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class	注销窗口类
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
void KeysActions(BOOL *done)
{
	if (active)								// Program Active?
	{
		//SetCursorPos(GetSystemMetrics(SM_CXSCREEN) >> 1, GetSystemMetrics(SM_CYSCREEN) >> 1);	
		if (keys[VK_ESCAPE])				// Was ESC Pressed?
		{
			*done=TRUE;						// ESC Signalled A Quit
		}
		else								// Not Time To Quit, Update Screen
		{
			DrawGLScene();					// Draw The Scene
			SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
		}
	}
	else
	{
		//ReleaseCapture();
	}
	

}
bool ReleaseKey(bool key,bool *isKeys)
{
	if (!keys)
	{
		*isKeys=FALSE;
	}
	return TRUE;
}

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match	保存查找匹配的结果
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	int nX=0,nY=0;
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.	移动时重画，并为窗口取得DC
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon	窗口的图标
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
/*	if (fullscreen)												// Attempt Fullscreen Mode?	分配了用于存储视频设置的空间
	{
		DEVMODE dmScreenSettings;								// Device Mode	设备模式
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared	确保内存清空为零
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{*/
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
		////////////////NEW ///////////////
		int wid = GetSystemMetrics(SM_CXSCREEN);		/**< 获取当前屏幕宽 */
		int hei = GetSystemMetrics(SM_CYSCREEN);		/**< 获取当前屏幕高 */
		nX = (wid - (WindowRect.right-WindowRect.left)) / 2;                    /**< 计算窗口居中用 */
		nY = (hei - (WindowRect.bottom-WindowRect.top)) / 2;			
		/// 调整我们窗口的大小，使其客户区的大小为我们设置的大小
		//AdjustWindowRectEx(&WindowRect, windowStyle, 0, windowExtendedStyle);
		/// 判断窗口的左上角是否隐藏在桌面外
		if (WindowRect.left < 0)										/**< 如果窗口X坐标为负，移动坐标到0处，并调整窗口的位置 */
		{
			WindowRect.right -= WindowRect.left;						
			WindowRect.left = 0;										
		}
		if (WindowRect.top < 0)											/**< 如果窗口Y坐标为负，移动坐标到0处，并调整窗口的位置 */
		{
			WindowRect.bottom -= WindowRect.top;						
			WindowRect.top = 0;											
		}
		////////////////////NEW//////////////
//}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size	使场景不会被边框遮盖住

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								nX,nY,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering	必须支持双缓冲
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}


LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		
		case WM_ACTIVATE :							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE; // Program Is Active
			}
			else
			{
				//ReleaseCapture();
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}
		case WM_LBUTTONDOWN: 
		{	
			if (is_drive == 0)
			{
				Vector3 v_Pos = m_Camera.getPosition();
				Vector3 v_View = m_Camera.getView();
				if (man_turn == 0)
				{
					man_degree = 90;
					m_Camera.setCamera(v_Pos.x + 10, v_Pos.y, v_Pos.z - 10, v_Pos.x + 9, v_Pos.y, v_Pos.z - 10, 0, 1, 0);	//左转
					man_turn = 1;
				}
				else if (man_turn == 1)
				{
					man_degree = 180;
					m_Camera.setCamera(v_Pos.x - 10, v_Pos.y, v_Pos.z - 10, v_Pos.x - 10, v_Pos.y, v_Pos.z - 9, 0, 1, 0);//后转
					man_turn = 2;
				}
				else if (man_turn == 2)
				{
					man_degree = 270;
					m_Camera.setCamera(v_Pos.x - 10, v_Pos.y, v_Pos.z + 10, v_Pos.x - 9, v_Pos.y, v_Pos.z + 10, 0, 1, 0);//右转
					man_turn = 3;
				}
				else if (man_turn == 3)
				{
					man_degree = 0;
					m_Camera.setCamera(v_Pos.x + 10, v_Pos.y, v_Pos.z + 10, v_Pos.x + 10, v_Pos.y, v_Pos.z + 9, 0, 1, 0);//前转
					man_turn = 0;
				}
			}
			break;
		}
		case WM_RBUTTONDOWN:  
		{
			if (is_drive == 0)
			{
				Vector3 v_Pos = m_Camera.getPosition();
				Vector3 v_View = m_Camera.getView();
				if (man_turn == 0)
				{
					man_degree = 270;
					m_Camera.setCamera(v_Pos.x - 10, v_Pos.y, v_Pos.z - 10, v_Pos.x - 9, v_Pos.y, v_Pos.z - 10, 0, 1, 0);//右转
					man_turn = 3;
				}
				else if (man_turn == 1)
				{
					man_degree = 0;
					m_Camera.setCamera(v_Pos.x - 10, v_Pos.y, v_Pos.z + 10, v_Pos.x - 10, v_Pos.y, v_Pos.z + 9, 0, 1, 0);//前转
					man_turn = 0;
				}
				else if (man_turn == 2)
				{
					man_degree = 90;
					m_Camera.setCamera(v_Pos.x + 10, v_Pos.y, v_Pos.z + 10, v_Pos.x + 9, v_Pos.y, v_Pos.z + 10, 0, 1, 0);	//左转
					man_turn = 1;
				}
				else if (man_turn == 3)
				{
					man_degree = 180;
					m_Camera.setCamera(v_Pos.x + 10, v_Pos.y, v_Pos.z - 10, v_Pos.x + 10, v_Pos.y, v_Pos.z - 9, 0, 1, 0);//后转
					man_turn = 2;
				}
			}
			break;
		}
		case WM_LBUTTONUP:
		{
			break;
		}
		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop
	
	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"\n\t欢迎来到城镇漫游！\n\n\n\t      是否全屏？", "城镇漫游", MB_YESNO ) == IDNO)
	{
		fullscreen=FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	//if (!CreateGLWindow("基于OpenGL的虚拟场景构建之城镇漫游",800,600,16,fullscreen))

	if (fullscreen == TRUE)
	{
		if (!CreateGLWindow("基于OpenGL的虚拟场景构建之城镇漫游", GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 16, fullscreen))
		{
			return 0;									// Quit If Window Was Not Created
		}
	}
	else
	{
		if (!CreateGLWindow("基于OpenGL的虚拟场景构建之城镇漫游", 800, 600, 16, fullscreen))
		{
			return 0;									// Quit If Window Was Not Created
		}
	}


	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?	PeekMessage能不锁住程序下进行消息检查
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			KeysActions(&done);
			Update();
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}

