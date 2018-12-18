// *************************************************************
// ���������ݶ࣬����Ϊ��Щ���������֣����ڼ��ϣ�û�з�װ��
// �����Ե÷��ߣ����ǿ�Ӧ�û��ܿ��ö��ɡ�����
// ����������̶��������������ȥ�޸��ˣ����иĽ���
// ��Ϊ�ֲ����ҰѸ������̷ֿ飬����ע�ͣ���������
// *************************************************************


// **********************************************************************************************************************
// ͷ�ļ�
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
// windows�����ý���йصĴ�����ӿڣ��������֣�
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib")
// **********************************************************************************************************************


// **********************************************************************************************************************
// ��ʼ��
HDC			hDC=NULL;		// Private GDI Device Context	 OpenGL��Ⱦ�豸��������
HGLRC		hRC=NULL;		// Permanent Rendering Context ������ɫ��������
HWND		hWnd=NULL;		// Holds Our Window Handle	�������ǵĴ��ھ��
HINSTANCE	hInstance;		// Holds The Instance Of The Application	��������ʵ��

bool	keys[256];			// Array Used For The Keyboard Routine	�������飬�ɴ�������ͻ
bool	isKeys[256];		//�����Ƿ�ס,һ�������л�����
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default	�����Ƿ񼤻�
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default	�Ƿ�ȫ��
bool	focus = true;
char *manual="    �ƶ�:\t\t\t    �ٶ�:\n"
			 "\tW �� \t��ǰ\t\tQ    \t����\n\tS  ��\t���\t\tE   \t����\n"
			 "\tA �� \t����\t\tShift\t�ܲ�\n\tD �� \t����\t\tEnter   \t����\n\n"
			 "    ��ҹ:\t\t\t    ����:\n"
			 "\t1    \t����    \t\t4    \t����\n\t2    \t�ƻ�    \t\t5   \t����\n"
			 "\t3    \tҹ��    \t\t6    \t��ѩ\n\n"
			 "    ����:\t\t\t    ��Ұ:\n"
			 "\tB    \t�س�    \t\tPgUp \t����\n\tP    \t��ͣ    \t\tPgDn\t�½�\n"
			 "\tL\t�ֵ�    \t\t������\t��ʱ��\n\tM\t����    \t\t����Ҽ�\t˳ʱ��\n"
			 "\tR\t�̻�\n\n"
			 "    ��Ϣ:\t\t\t    ��ʾ:\n"
			 "\tH    \t����    \t\t�����̻�\t����ҹ��\n\tT\t����    \t\t����Ѱ��\tͼ��ݺ�\n"
			 "\tI\t�汾\n\tEsc\t�˳�\n"; 
char *information = "\n\t������Ŀ:\t\t����OpenGL�����ⳡ������\n\t\t\t\t    ֮\n\t\t\t             ��������\n\n"
					"\t����:\t\t              �Ʊ���\n\n\n"
					"\tָ����ʦ:\t\t             ��������ʦ\n\n\n"
					"\t\t\t\t\t���ڴ�ѧ��Ϣ����ѧԺ\n\t\t\t\t\t\t2017";

//int		LBDown; //������
float   m_Fps;      // ֡�� 
GLFont  m_Font;		//����
Camera  m_Camera ;	//�������
CSkyBox m_SkyBox;	//��պ�
CGround m_Ground;	//�ذ�
GL_Flag     m_Flag;//����
CLoad3DS g_Load3ds;		//3DS������								
Static3DS m_Static3DS ;		//��̬����
CMDLOBJ m_MDLOBJ;	//��������
Snow m_snow;//��ѩ����
bool is_snow=0;
bool	m_RenderMode = false;	//����ģʽ ��仹������
float man_x, man_z,man_degree=0;
int man_action=0;//���ﶯ��
int man_turn = 0;//����ת��
int car_stop = 0;
Vector3 jeep_pos;
Vector3 camera_pos;
float camera_angle = 0;
float camera_rad = float(3.13149f * camera_angle / 180.0f);//�ƶ������ӽ�
bool jeep_first = 0;//��һ���ϳ�
bool man_move=1;//�����Ƿ��ƶ�
bool is_music = 0;//0ʱ������
bool is_meet = 0;//�Ƿ�����
bool is_drive = 0;//�˵��ӽǻ��ǳ����ӽ�
float	jeep_angle = 90;//���ĽǶ�
int is_night = 0;
float speed_camera = 0.2f;

GLfloat LightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };		//�������ֵRGBA
GLfloat LightDiffuse[] = { 0.2f, 0.2f, 0.2f, 0.2f };		//ɢ����ֵ
GLfloat LightPosition[] = { 1000.0f, 50.0f, -2.0f, 1.0f };	//����λ��XYZA

GLuint fogMode[] = { GL_EXP, GL_EXP2, GL_LINEAR };  // ������ģʽ
GLuint fogfilter = 0;     // ʹ����һ������
GLfloat fogColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };  // �����ɫ��Ϊ��ɫ
// ��ʱ���ص�����  
void CALLBACK TimeProc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime);

// **********************************************************************************************************************


// **********************************************************************************************************************
/** ����֡�� */
void CaculateFrameRate()
{
	static float framesPerSecond = 0.0f;	     /**< ������ʾ֡�� */
	static float lastTime = 0.0f;	             /**< ��¼�ϴ�ʱ�� */
	float currentTime = GetTickCount() * 0.001f; /**< ��õ�ǰʱ�� */
	framesPerSecond++;                           /**< ��ʾ֡������1 */
	if (currentTime - lastTime > 1.0f)           /** ���ʱ������1.0�� */
	{
		lastTime = currentTime;                  /**< ���浱ǰʱ�� */
		m_Fps = framesPerSecond;                 /**< ��ǰ֡������m_Fps */
		framesPerSecond = 0;                     /**< ��֡������ */
	}
}
// **********************************************************************************************************************

// **********************************************************************************************************************
// �����Ļ����
void PrintCurrentText()			
{
	char string[128];                               /**< ���ڱ�������ַ��� */
	glPushAttrib(GL_CURRENT_BIT);                   /**< ����������ɫ������Ϣ */
	int x, z;
	float car_pos = -1000;//7,1,8,2     9,3,10,4     11,5,12,6
	
	if (man_move == 1)//�򻯣�����ʵʱ���
	{
		CaculateFrameRate();                          /**< ����֡�� */
		glColor3f(0.0f, 1.0f, 1.0f);
		sprintf(string, "FPS:%d", (int)m_Fps);        /**< �ַ�����ֵ */
		m_Font.PrintText(string, -5.0f, 2.0f);

		glColor3f(0.0f, 1.0f, 1.0f);                      /**< ����������ɫ */
		if (man_turn == 0)
		{
			sprintf(string, "�ӽ�����:X=%3.1f Y=%3.1f Z =%3.1f ",
				m_Camera.getPosition().x - 1000, m_Camera.getPosition().y, -800 - m_Camera.getPosition().z); 
			m_Font.PrintText(string, -5.0f, 3.0f);
			x = man_x;
			z = man_z - 10.0f;
		}
		else if (man_turn == 1)
		{
			sprintf(string, "�ӽ�����:X=%3.1f Y=%3.1f Z =%3.1f ",
				m_Camera.getPosition().x - 1000 -10.0f, m_Camera.getPosition().y, -800 - m_Camera.getPosition().z-10.0f); 
			m_Font.PrintText(string, -5.0f, 3.0f);
			x = man_x - 10.0f;
			z = man_z ;
		}
		else if (man_turn == 2)
		{
			sprintf(string, "�ӽ�����:X=%3.1f Y=%3.1f Z =%3.1f ",
				m_Camera.getPosition().x - 1000, m_Camera.getPosition().y, -800 - m_Camera.getPosition().z-20.0f);
			m_Font.PrintText(string, -5.0f, 3.0f);
			x = man_x ;
			z = man_z + 10.0f;
		}
		else if (man_turn == 3)
		{
				sprintf(string, "�ӽ�����:X=%3.1f Y=%3.1f Z =%3.1f ",
					m_Camera.getPosition().x - 1000 + 10.0f, m_Camera.getPosition().y, -800 - m_Camera.getPosition().z-10.0f); 

			m_Font.PrintText(string, -5.0f, 3.0f);
			x = man_x + 10.0f;
			z = man_z;
		}

		if (x >= 990 && x <= 1010 && z <= -805 && z >= -825)
		{
			sprintf(string, "��ǰλ��:\t�������");
			m_Font.PrintText(string, -5.0f, 3.5f);
			sprintf(string, "��ǰ�ٶ�:%3.2f   ���H����ȡ��������", m_Camera.getSpeed());
			m_Font.PrintText(string, -5.0f, 2.5f);
		}
		else if (x >= 950 && x <= 990)
		{
			if (z <= -600 && z >= -850)
			{
				sprintf(string, "��ǰλ��:\t������� ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -850 && z >= -980)
			{
				sprintf(string, "��ǰλ��:\tŷ������ֵ� ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -1000 && z >= -1140)
			{
				sprintf(string, "��ǰλ��:\t�칫¥ ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -1160 && z >= -1400)
			{
				sprintf(string, "��ǰλ��:\t����¥ ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if ((z <= -980 && z >= -1000) || (z <= -1140 && z >= -1160))
			{
				sprintf(string, "��ǰλ��:\t��·�� ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else
			{
				sprintf(string, "��ǰλ��:\tɳ̲ ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
		}
		else if (x >= 1010 && x <= 1050)
		{
			if (z <= -600 && z >= -850)
			{
				sprintf(string, "��ǰλ��:\t�����Ҳ� ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -850 && z >= -980)
			{
				sprintf(string, "��ǰλ��:\tŷ������ֵ� ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -1000 && z >= -1140)
			{
				sprintf(string, "��ǰλ��:\t�칫¥ ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -1160 && z >= -1400)
			{
				sprintf(string, "��ǰλ��:\t����¥ ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if ((z <= -980 && z >= -1000) || (z <= -1140 && z >= -1160))
			{
				sprintf(string, "��ǰλ��:\t��·�� ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else
			{
				sprintf(string, "��ǰλ��:\tɳ̲ ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
		}
		else if (x <= 930 && x >= 600)
		{
			if (z <= -600 && z >= -980)
			{
				sprintf(string, "��ǰλ��:\t��Ȼ��԰ ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -1000 && z >= -1080)
			{
				sprintf(string, "��ǰλ��:\t����");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -1080 && z >= -1140)
			{
				sprintf(string, "��ǰλ��:\tҽԺ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -1160 && z >= -1400)
			{
				sprintf(string, "��ǰλ��:\t�߼��Ƶ� ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if ((z <= -980 && z >= -1000) || (z <= -1140 && z >= -1160))
			{
				sprintf(string, "��ǰλ��:\t��·�� ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else
			{
				sprintf(string, "��ǰλ��:\tɳ̲ ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
		}
		else if (x >= 1070 && x <= 1400)
		{
			if (z <= -600 && z >= -980)
			{
				sprintf(string, "��ǰλ��:\t�˶���");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -1000 && z >= -1080)
			{
				sprintf(string, "��ǰλ��:\tͼ���");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -1080 && z >= -1140)
			{
				sprintf(string, "��ǰλ��:\t����");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if (z <= -1160 && z >= -1400)
			{
				sprintf(string, "��ǰλ��:\tС��סլ ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else if ((z <= -980 && z >= -1000) || (z <= -1140 && z >= -1160))
			{
				sprintf(string, "��ǰλ��:\t��·�� ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
			else
			{
				sprintf(string, "��ǰλ��:\tɳ̲ ");
				m_Font.PrintText(string, -5.0f, 3.5f);
				sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
				m_Font.PrintText(string, -5.0f, 2.5f);
			}
		}
		else if ((x >= 990 && x <= 1010 && z <= -600 && z >= -1400) || (x >= 930 && x <= 950 && z <= -600 && z >= -1400) ||
			(x >= 1050 && x <= 1070 && z <= -600 && z >= -1400) || (x >= 600 && x <= 1400 && z <= -980 && z >= -1000) ||
			(x >= 600 && x <= 1400 && z <= -1140 && z >= -1160))
		{
			sprintf(string, "��ǰλ��:\t��·�� ");
			m_Font.PrintText(string, -5.0f, 3.5f);
			sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
			m_Font.PrintText(string, -5.0f, 2.5f);
		}
		else
		{
			sprintf(string, "��ǰλ��:\tɳ̲ ");
			m_Font.PrintText(string, -5.0f, 3.5f);
			sprintf(string, "��ǰ�ٶ�:%3.2f", m_Camera.getSpeed());
			m_Font.PrintText(string, -5.0f, 2.5f);
		}
		man_move = 0;
	}
// **********************************************************************************************************************
	

// **********************************************************************************************************************
// ��������·�Ͽ��ŵĳ������˺�����Ѳ���ͣ��
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
/** ��������� */
void UpdateCamera()
{
	if(is_drive == 0)
		m_Camera.setViewByMouse();

	/** ���̰�����Ӧ */
	if(keys[VK_SHIFT])                        /**< ����SHIFT��ʱ���� */
	{
		if (is_drive == 0)	m_Camera.setSpeed(0.5f);
	}
	if(!keys[VK_SHIFT])
	{
		m_Camera.setSpeed(speed_camera);
	}
	if (keys['Q'])                        /**< ����Q��ʱ���� */
	{
		speed_camera = m_Camera.getSpeed();
		if (speed_camera < 0.49f)
		{
			speed_camera += 0.01f;
			m_Camera.setSpeed(speed_camera);
		}
	}
	if (keys['E'])                        /**< ����E��ʱ���� */
	{
		speed_camera = m_Camera.getSpeed();
		if (speed_camera > 0.10)
		{
			speed_camera -= 0.01f;
			m_Camera.setSpeed(speed_camera);
		}
	}
	if (keys['H'] && !isKeys['H'])                        /**< ����H��ʱ */
	{
		isKeys['H'] = true;
		MessageBox(NULL, manual	, "�����ֲ�", MB_OK); 
	}
	if (!keys['H'])
	{
		isKeys['H'] = false;
	}
	if (keys['I'] && !isKeys['I'])                        /**< ����I��ʱ */
	{
		isKeys['I'] = true;
		MessageBox(NULL,information, "�汾��Ϣ",MB_OK);
	}
	if (!keys['I'])                        /**< ����I��ʱ */
	{
		isKeys['I'] = false;
	}
	if (keys[VK_UP]  || keys['W'] )   /**< ���Ϸ������'W'������ */
	{
		m_Camera.moveCamera(m_Camera.getSpeed());          /**< �ƶ������ */
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
	if (keys[VK_DOWN] || keys['S']) /**< ���·������'S'������ */
	{
		m_Camera.moveCamera(-m_Camera.getSpeed());         /**< �ƶ������ */
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
	if (keys[VK_LEFT] || keys['A'] ) /**< ���������'A'������ */
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
	if (keys[VK_RIGHT] || keys['D'] ) /**< ���ҷ������'D'������ */
	{
		if (is_drive == 0)
		{
			m_Camera.yawCamera(m_Camera.getSpeed());            /**< �ƶ������ */
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

	if (keys['L'] && !isKeys['L'])                        /**< ����L��ʱ������ */
	{
		isKeys['L'] = true;
		m_Ground.setlight();
	}
	if (!keys['L']) 
		isKeys['L'] = false;

// **********************************************************************************************************************
// ���������ֵ�
	if (keys['1'])                        /**< ����1��ʱ���� */
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
	if (keys['2'])                        /**< ����2��ʱ���� */
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
	if (keys['3'])                        /**< ����3��ʱ��ҹ */
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
	if (keys['4'])                        /**< ���� */
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
	if (keys['5'])							/**< ���� */
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
	if (keys['6'])                        /**< ��ѩ */
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

	if (keys['M'] && !isKeys['M'])                        /**< ����M��ʱ */
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
	if (!keys['M'])                        /**< ����I��ʱ */
	{
		isKeys['M'] = false;
	}
	if (keys['P'] && !isKeys['P'])                        /**< ����P��ʱ */
	{
		isKeys['P'] = true;
		{
			if (is_night == 0)
			{
				KillTimer(NULL, 1);
				mciSendString(TEXT("pause day"), NULL, 0, NULL);	MessageBox(NULL, "�������", "��ͣ", MB_OK);
				if (is_music == 0)									mciSendString(TEXT("resume day"), NULL, 0, NULL);
				SetTimer(NULL, 1, 20, TimeProc);
			}
			else if (is_night == 1)
			{
				KillTimer(NULL, 1);
				mciSendString(TEXT("pause night"), NULL, 0, NULL);	MessageBox(NULL, "�������", "��ͣ", MB_OK);
				if (is_music == 0)									mciSendString(TEXT("resume night"), NULL, 0, NULL);
				SetTimer(NULL, 1, 20, TimeProc);
			}
			else if (is_night == 2)
			{
				KillTimer(NULL, 1);
				mciSendString(TEXT("pause dusk"), NULL, 0, NULL);	MessageBox(NULL, "�������", "��ͣ", MB_OK);
				if (is_music == 0)									mciSendString(TEXT("resume dusk"), NULL, 0, NULL);
				SetTimer(NULL, 1, 20, TimeProc);
			}
			else if (is_night == 3)
			{
				KillTimer(NULL, 1);
				mciSendString(TEXT("pause sunday"), NULL, 0, NULL);	MessageBox(NULL, "�������", "��ͣ", MB_OK);
				if (is_music == 0)									mciSendString(TEXT("resume sunday"), NULL, 0, NULL);
			}
			else if (is_night == 4)
			{
				KillTimer(NULL, 1);
				mciSendString(TEXT("pause mist"), NULL, 0, NULL);	MessageBox(NULL, "�������", "��ͣ", MB_OK);
				if (is_music == 0)									mciSendString(TEXT("resume mist"), NULL, 0, NULL);
			}
			else if (is_night == 5)
			{
				KillTimer(NULL, 1);
				mciSendString(TEXT("pause snow"), NULL, 0, NULL);	MessageBox(NULL, "�������", "��ͣ", MB_OK);
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
	if ( keys['T'] && !isKeys['T']) /**< 'T'������ */
	{
		isKeys['T'] = true;
		m_RenderMode = !m_RenderMode; /*�л�����ģʽ */
		if (m_RenderMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}	
	if (!keys['T'])                        
	{
		isKeys['T'] = false;
	}
	if (keys['B']) /**< 'B'������ */
	{
		m_Camera.setCamera(1000, 3, -800, 1000, 3, -800 - 1, 0, 1, 0);	//XZ��ʼ����ò��ֻ�ܲ�1
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
	if (keys['R'] && !isKeys['R'])                        //< ����R��ʱ���̻� 
	{
		isKeys['R'] = true;
		if(is_night==0 || is_night ==1)
			FireInit();//�����̻�
	}
	if (!keys['R'] )                       
		isKeys['R'] = false;

	if (keys[VK_PRIOR])   /**< PageUp������ */
	{
		Vector3 v_Pos = m_Camera.getPosition();         /**< ���߸߶����� */
		Vector3 v_View = m_Camera.getView();
		if (v_Pos.y < 40)
		{
			v_Pos.y += 0.5;
			v_View.y += 0.5;
			m_Camera.setCamera(v_Pos.x, v_Pos.y, v_Pos.z, v_View.x, v_View.y, v_View.z, 0, 1, 0);
		}
	}
	if (keys[VK_NEXT])   /**< PagezDown������ */
	{
		Vector3 v_Pos = m_Camera.getPosition();         /**< ���߸߶��½� */
		Vector3 v_View = m_Camera.getView();
		if (v_Pos.y > 1.5)
		{
			v_Pos.y -= 0.5;
			v_View.y -= 0.5;
			m_Camera.setCamera(v_Pos.x, v_Pos.y, v_Pos.z, v_View.x, v_View.y, v_View.z, 0, 1, 0);
		}
	}
	if (keys[VK_RETURN] && !isKeys[VK_RETURN])   //�س���
	{
		isKeys[VK_RETURN] = true;
		if (is_drive == 0)
		{
			if (fabs(m_Static3DS.get_jeep_pos().x - man_x) < 15 && fabs(m_Static3DS.get_jeep_pos().z - man_z) < 15)
			{
				//��������������
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
	/** ���ݵ��θ߶ȸ�������� */
	Vector3 vPos = m_Camera.getPosition();                  /**< �õ���ǰ�����λ�� */
	Vector3 vNewPos = vPos; 
	man_x = vNewPos.x;
	man_z = vNewPos.z;

	/** ����������߶�Ϊ ���θ߶� + 10 */
//	vNewPos.y = (float)m_Terrain.getAveHeight(vPos.x,vPos.z ) + 10;
	//vNewPos.y =5.0f; // ��վ��λ�ø߶�
	/** �õ��߶Ȳ�ֵ */
//	float temp = vNewPos.y - vPos.y;

	/** ������������� */
	Vector3 vView = m_Camera.getView();
	//vView.y += 0.0f;

	/** ��������� */
	m_Camera.setCamera(vNewPos.x,  vNewPos.y,  vNewPos.z,
		vView.x,	   vView.y,	   vView.z,	 
		0, 1, 0);								

}
// **********************************************************************************************************************


// **********************************************************************************************************************

/** ������º��� */
void Update()						
{
	/** �û��Զ���ĸ��¹��� */

	/** ��������� */
	UpdateCamera();

	m_Flag.Update();
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window	��������OpenGL�����Ĵ�С
{
	if (height==0)										// Prevent A Divide By Zero By	 ��ֹ�����
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport	�ؽ���ǰ����
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix	ѡ��ͶӰ����
	glLoadIdentity();									// Reset The Projection Matrix	����ǰ����ָ���ԭʼ״̬

	// Calculate The Aspect Ratio Of The Window	���㴰�ں��ݱ�
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.05f,4000.0f);	//����͸��ͶӰ����
	//gluPerspective(
	//�Ƕ�,
	//�Ӿ���Ŀ�߱�,
	//��z�᷽���������֮��ľ���Ľ����������㣩,
	//��z�᷽���������֮��ľ����Զ��������յ㣩);
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix	ѡ��ģ�͹۲����	
	glLoadIdentity();									// Reset The Modelview Matrix	���õ�ǰ����
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
//	glEnable(GL_LIGHTING);
//	glDisable(GL_LIGHTING);
	
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading	������Ӱƽ��
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background	��Ϊ��ɫ
//	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);// ���ñ�������ɫΪ��������ɫ

	////////////NEW ///////////////////
	m_Camera.setCamera(1000, 3, -800, 1000, 3, -800 - 1, 0, 1, 0);	//XZ��ʼ����ò��ֻ�ܲ�1

	///////////NEW ///////////////////
	//glClearColor(��, ��, ��, ͸����);
	glClearDepth(1.0f);									// Depth Buffer Setup	������Ȼ���
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing	������Ȳ���
	
//	glEnable(GL_LIGHT0);
//	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do	��Ȳ��Ե�����	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations	͸������
	glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);

	//���ù�Դ����
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		//���û�����
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		//�����������
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);		//��Դ��λ��
	glEnable(GL_LIGHT1);								//�����Դһ
	
	glFogi(GL_FOG_MODE, fogMode[fogfilter]);  // ����������ģʽ
	glFogfv(GL_FOG_COLOR, fogColor);   // ���������ɫ
	glFogf(GL_FOG_DENSITY, 0.005f);   // ��������ܶ�
	glHint(GL_FOG_HINT, GL_DONT_CARE);   // ����ϵͳ��μ�������
	glFogf(GL_FOG_START, 1.0f);    // �����Ŀ�ʼλ��
	glFogf(GL_FOG_END, 5.0f);    // �����Ľ���λ��
//	glEnable(GL_FOG);     // ʹ������

	//��ʼ������ģ��
	m_MDLOBJ.init(&m_Camera);
	

	/** ��ʼ������ */
	if(!m_Font.InitFont())
		MessageBox(NULL,"��ʼ������ʧ��!","����",MB_OK);

	if(!m_Ground.init())
	{
		MessageBox(NULL,"��ʼ���ذ�ʧ��!","����",MB_OK);
		exit(0);
	}
	/** ��ʼ����� */
	if(!m_SkyBox.init())
	{
		MessageBox(NULL,"��ʼ�����ʧ��!","����",MB_OK);
		exit(0);
	}

	if(!m_Static3DS.init())
	{ 
		MessageBox(NULL,"��ʼ����̬ģ��ʧ��!","����",MB_OK);
		exit(0);

	}

	if (!m_snow.Init(5000))
	{
		MessageBox(NULL, "��ʼ����ѩʧ��!", "����", MB_OK);
		exit(0);
	}
	m_snow.Load("data\\textures\\snow.bmp", m_snow.m_texture);	       // ����ѩ

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
	//Sleep(20);	//��ʱ20����
	Vector3 vAxis = m_Camera.m_View - m_Camera.m_Position;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer	�����������Ȼ���
	//glEnable(GL_LIGHTING);
	glLoadIdentity();
	
	SetTimer(NULL,1, 1000, TimeProc);

	m_Camera.setLook();// �������λ��
		
	m_SkyBox.render(is_night);			//��Ⱦ���

	m_Ground.render();			//��Ⱦ�ذ�

	m_Flag.render(m_Ground.m_texture[109], m_Ground.m_texture[112], 880, 15, -910);//����
	m_Flag.render(m_Ground.m_texture[110], m_Ground.m_texture[112], 880, 14, -900);
	m_Flag.render(m_Ground.m_texture[111], m_Ground.m_texture[112], 880, 14, -920);

	if(is_night==2)		m_Static3DS.balloon_render();	//�����̬3DS
	
	if(is_snow==1)		m_snow.Render();//��ѩ

	m_Static3DS.jeep_render(m_Static3DS.get_jeep_pos().x, 1.2, m_Static3DS.get_jeep_pos().z, jeep_angle);//���ճ�

	m_MDLOBJ.render();//MDL����ģ��
		
	if (is_night == 0 || is_night == 1)	FireWorks();//�̻�������ҹ��

	if (is_drive == 0)
		m_MDLOBJ.man_render(man_turn, man_x, man_z, man_degree, man_action);//����λ�ü�����

	PrintCurrentText();

	glFlush();
	
	return TRUE;										// Everything Went OK
}


void CALLBACK TimeProc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{	
	man_move = 1;

	m_MDLOBJ.move(car_stop);
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window	�����رմ���
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop	����ԭʼ���� 0����˼�Ǵ�ע���������������
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
														//wglMakeCurrent( HDC hdc, HGLRC hglrc)��
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

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class	ע��������
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
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match	�������ƥ��Ľ��
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
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.	�ƶ�ʱ�ػ�����Ϊ����ȡ��DC
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon	���ڵ�ͼ��
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
/*	if (fullscreen)												// Attempt Fullscreen Mode?	���������ڴ洢��Ƶ���õĿռ�
	{
		DEVMODE dmScreenSettings;								// Device Mode	�豸ģʽ
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared	ȷ���ڴ����Ϊ��
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
		int wid = GetSystemMetrics(SM_CXSCREEN);		/**< ��ȡ��ǰ��Ļ�� */
		int hei = GetSystemMetrics(SM_CYSCREEN);		/**< ��ȡ��ǰ��Ļ�� */
		nX = (wid - (WindowRect.right-WindowRect.left)) / 2;                    /**< ���㴰�ھ����� */
		nY = (hei - (WindowRect.bottom-WindowRect.top)) / 2;			
		/// �������Ǵ��ڵĴ�С��ʹ��ͻ����Ĵ�СΪ�������õĴ�С
		//AdjustWindowRectEx(&WindowRect, windowStyle, 0, windowExtendedStyle);
		/// �жϴ��ڵ����Ͻ��Ƿ�������������
		if (WindowRect.left < 0)										/**< �������X����Ϊ�����ƶ����굽0�������������ڵ�λ�� */
		{
			WindowRect.right -= WindowRect.left;						
			WindowRect.left = 0;										
		}
		if (WindowRect.top < 0)											/**< �������Y����Ϊ�����ƶ����굽0�������������ڵ�λ�� */
		{
			WindowRect.bottom -= WindowRect.top;						
			WindowRect.top = 0;											
		}
		////////////////////NEW//////////////
//}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size	ʹ�������ᱻ�߿��ڸ�ס

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
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering	����֧��˫����
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
					m_Camera.setCamera(v_Pos.x + 10, v_Pos.y, v_Pos.z - 10, v_Pos.x + 9, v_Pos.y, v_Pos.z - 10, 0, 1, 0);	//��ת
					man_turn = 1;
				}
				else if (man_turn == 1)
				{
					man_degree = 180;
					m_Camera.setCamera(v_Pos.x - 10, v_Pos.y, v_Pos.z - 10, v_Pos.x - 10, v_Pos.y, v_Pos.z - 9, 0, 1, 0);//��ת
					man_turn = 2;
				}
				else if (man_turn == 2)
				{
					man_degree = 270;
					m_Camera.setCamera(v_Pos.x - 10, v_Pos.y, v_Pos.z + 10, v_Pos.x - 9, v_Pos.y, v_Pos.z + 10, 0, 1, 0);//��ת
					man_turn = 3;
				}
				else if (man_turn == 3)
				{
					man_degree = 0;
					m_Camera.setCamera(v_Pos.x + 10, v_Pos.y, v_Pos.z + 10, v_Pos.x + 10, v_Pos.y, v_Pos.z + 9, 0, 1, 0);//ǰת
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
					m_Camera.setCamera(v_Pos.x - 10, v_Pos.y, v_Pos.z - 10, v_Pos.x - 9, v_Pos.y, v_Pos.z - 10, 0, 1, 0);//��ת
					man_turn = 3;
				}
				else if (man_turn == 1)
				{
					man_degree = 0;
					m_Camera.setCamera(v_Pos.x - 10, v_Pos.y, v_Pos.z + 10, v_Pos.x - 10, v_Pos.y, v_Pos.z + 9, 0, 1, 0);//ǰת
					man_turn = 0;
				}
				else if (man_turn == 2)
				{
					man_degree = 90;
					m_Camera.setCamera(v_Pos.x + 10, v_Pos.y, v_Pos.z + 10, v_Pos.x + 9, v_Pos.y, v_Pos.z + 10, 0, 1, 0);	//��ת
					man_turn = 1;
				}
				else if (man_turn == 3)
				{
					man_degree = 180;
					m_Camera.setCamera(v_Pos.x + 10, v_Pos.y, v_Pos.z - 10, v_Pos.x + 10, v_Pos.y, v_Pos.z - 9, 0, 1, 0);//��ת
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
	if (MessageBox(NULL,"\n\t��ӭ�����������Σ�\n\n\n\t      �Ƿ�ȫ����", "��������", MB_YESNO ) == IDNO)
	{
		fullscreen=FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	//if (!CreateGLWindow("����OpenGL�����ⳡ������֮��������",800,600,16,fullscreen))

	if (fullscreen == TRUE)
	{
		if (!CreateGLWindow("����OpenGL�����ⳡ������֮��������", GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 16, fullscreen))
		{
			return 0;									// Quit If Window Was Not Created
		}
	}
	else
	{
		if (!CreateGLWindow("����OpenGL�����ⳡ������֮��������", 800, 600, 16, fullscreen))
		{
			return 0;									// Quit If Window Was Not Created
		}
	}


	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?	PeekMessage�ܲ���ס�����½�����Ϣ���
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

