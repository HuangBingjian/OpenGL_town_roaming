// *************************************************************
// ���Լ���д�Ļ��Ƶ���ĺ�����������·�����򳡡����򳡡�
// ·�ơ���ľ�ȣ��ǽ���������ϵ�Ļ�������ɵģ��ɶ��Բ
// û�취���ο����ϸĶ������ġ�
// *************************************************************


#include "..\header\Ground.h"
#include "..\header\GL_Flag.h"


CGround::CGround():length(2000.0f),width(2000.0f),height_base(0.2f), height_tree(1.5f),open_light(0)
{	
}

CGround::~CGround()
{
	for (int i = 0; i < NUM_TEXTURE; i++)
	{
		if (TextureImage[i])								//�����Ƿ����
		{
			if (TextureImage[i]->data)						//����ͼ���Ƿ����
			{
				free(TextureImage[i]->data);				//�ͷ�����ͼ����ռ���ڴ�
			}

			free(TextureImage[i]);							//�ͷ�ͼ��ṹ
		}
	}
}


AUX_RGBImageRec *LoadBMP(char *Filename)				// ��ȡλͼͼ��
{
	FILE *File = NULL;									// �ļ����

	if (!Filename)										// ȷ���ļ����Ѹ���
	{
		return NULL;									// ����ļ���δ�����򷵻�NULL
	}

	File = fopen(Filename, "r");							// ����ļ��Ƿ����

	if (File)											// �ļ��Ƿ����?
	{
		fclose(File);									// �ر��ļ�
		return auxDIBImageLoad(Filename);				// ��ȡλͼ������һ��ָ��
	}

	return NULL;										//��������ļ�ʧ���򷵻�NULL
}

void CGround::setlight()
{
	open_light = !open_light;
}
void CGround::openlight()
{
	open_light = 1;
}
void CGround::closelight()
{
	open_light = 0;
}

/** �����ʼ�� */
bool CGround::init()
{
	bool Status = FALSE;									//״̬ȷ��
	char filename[128] ;                                    /**< ���������ļ��� */
	memset(TextureImage, 0, sizeof(void *) * NUM_TEXTURE);           	//��ָ����ΪNULL
	char *bmpName[] = {	"outside"/*��Χ*/,
		/*1-5*/		"base"/*����*/,"road"/*��·*/,"football1"/*����1*/,"football2"/*����2*/,"branch"/*����*/,    
		/*6-10*/	"tree1"/*��1*/,"tree2"/*��2*/,"streetlight"/*�ֵ�*/,"run1"/*�ܽ��ܵ�*/,"basketball1"/*����1*/,
		/*11-15*/	"basketball2"/*����2*/,"basketball3"/*����3*/,"basketball4"/*����4*/,"park"/*��԰�ݵ�*/,"yellow"/*��ɫ*/,
		/*16-20*/	"road1"/*ʮ��·��*/,"library"/*ͼ���*/,"supermarket"/*����*/,"road4"/*���е�*/,"road5"/*���е�*/,
		/*21-25*/	"parkdoor2"/*��ɫ*/,"parkdoor1"/*��԰��������*/,"house3_1","house3_2","run2"/*�ܽ��ܵ�*/,
		/*26-30*/	"house1_1","house1_2", "house1_3", "house1_4","house1_5",
		/*31-35*/	"house2_1","house2_2","house4_1","house4_2","house5_1",
		/*36-40*/	"house5_2","house6_1", "house6_2", "house7_1","house7_2",
		/*41-45*/	"house8_1","house8_2", "house8_3", "house9_1","house9_2",
		/*46-50*/	"house10_1","house10_2","house11_1", "house11_2","house12_1",
		/*51-55*/	"house12_2","house13_1", "house13_2", "house14_1","house14_2",
		/*56-60*/	"house15_1","house15_2", "house16_1", "house16_2", "house17_1",
		/*61-65*/	"house17_2", "house18_1", "house18_2", "house19_1", "house19_2",
		/*66-70*/	"house20_1", "house20_2", "house21_1", "house21_2", "house22_1",
		/*71-75*/	"house22_2", "house23_1", "house23_2","house24_1","house24_2",
		/*76-80*/	"house25_1", "house25_2", "house26_1", "house26_2", "house27_1",
		/*81-85*/	"house27_2","house28_1", "house28_2","house29_1", "house29_2",
		/*86-90*/	"house30_1", "house30_2", "house31_1", "house31_2","house32_1",
		/*91-95*/	"house32_2","house33_1", "house33_2","house34_1", "house34_2",
		/*96-100*/	"house35_1", "house35_2","house36_1", "house36_2","market1",
		/*101-105*/ "market2","market3","library1","library2","library3",
		/*106-110*/ "hotel1","hotel2","hotel3","China","SZU",
		/*111-115*/ "SZU_CIE","flagpole","hospital_1","hospital_2","hospital_3",
		/*116-120*/ "hospital_4","hospital_5","hospital_6","bank_1","bank_2",
		/*121-125*/ "bank_3","bank_4"
	};
	for(int i=0; i< NUM_TEXTURE; i++)
	{
		sprintf(filename,"data/ground/%s",bmpName[i]);
		strcat(filename,".bmp");
		if(TextureImage[i] = LoadBMP(filename))        /**< ����λͼ�ļ� */
		{
			Status=TRUE;									//��StatusΪTRUE
			glGenTextures(1, &m_texture[i]);                        /**< ����һ������������� */
			glBindTexture(GL_TEXTURE_2D, m_texture[i]);             //��������
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[i]->sizeX, TextureImage[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[i]->data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);																	
		}
		else
		{
			MessageBox(NULL, "װ��λͼ�ļ�ʧ�ܣ�", "����", MB_OK);    /**< �������ʧ���򵯳��Ի��� */
			exit(0);
		}
	}
//	render();
	return Status;										//����Status��ֵ
}



void CGround::render()
{
	int i;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
//	glEnable(GL_CULL_FACE);

	///////////////////////////ɳ̲
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, m_texture[0]); 
    for(i = 0 ; i<width; i=i+600)//ɳ̲
	{
			glBegin(GL_QUADS);		
			glTexCoord2f(0.0f, 1.0f); glVertex3f(  i, 0, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(  i+600, 0,0 ); 
			glTexCoord2f(1.0f, 0.0f); glVertex3f( i+600, 0,  -600);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( i, 0, -600);
			glEnd();
	}
	for (i = 0; i<width; i = i + 600)//ɳ̲
	{
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(i, 0, -1400);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(i + 600, 0, -1400);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(i + 600, 0, -2000);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(i,0, -2000);
			glEnd();
	}
	for (i = -600; i>-1400; i = i - 600)//ɳ̲
	{
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 0, i);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(600, 0, i);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(600, 0, i-600);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0, 0, i-600);
			glEnd();
	}
	for (i = -600; i>-1400; i = i - 600)//ɳ̲
	{
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(1400, 0, i);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(2400, 0, i);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(2400, 0, i - 600);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(1400, 0, i - 600);
			glEnd();
	}
	///////////////////////800*800�ĳ��е�ͼ	
	glBindTexture(GL_TEXTURE_2D, m_texture[19]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(930, 0, -600);//��·������
		glTexCoord2f(0.0f, 0.0f); glVertex3f(950, 0, -600);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(950, 0, -1000);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(930, 0, -1000);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(990, 0, -600);//��·������
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1010, 0, -600);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1010, 0, -1000);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(990, 0, -1000);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1050, 0, -600);//��·������
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1070, 0, -600);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1070, 0, -1000);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1050, 0, -1000);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(930, 0, -1000);//��·������
		glTexCoord2f(0.0f, 0.0f); glVertex3f(950, 0, -1000);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(950, 0, -1400);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(930, 0, -1400);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(990, 0, -1000);//��·������
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1010, 0, -1000);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1010, 0, -1400);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(990, 0, -1400);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1050, 0, -1000);//��·������
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1070, 0, -1000);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1070, 0, -1400);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1050, 0, -1400);
		glEnd();
	
		glBindTexture(GL_TEXTURE_2D, m_texture[20]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(600, 0, -980);//��·�����
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1400, 0, -980);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1400, 0, -1000);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(600, 0, -1000);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(600, 0, -1140);//��·�����
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1400, 0, -1140);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1400, 0, -1160);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(600, 0, -1160);
		glEnd();
	
		glBindTexture(GL_TEXTURE_2D, m_texture[16]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(925, 0.1f, -975);//ʮ��·��
		glTexCoord2f(0.0f, 0.0f); glVertex3f(955, 0.1f, -975);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(955, 0.1f, -1005);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(925, 0.1f, -1005);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(925, 0.1f, -1135);//ʮ��·��
		glTexCoord2f(0.0f, 0.0f); glVertex3f(955, 0.1f, -1135);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(955, 0.1f, -1165);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(925, 0.1f, -1165);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(985, 0.1f, -975);//ʮ��·��
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1015, 0.1f, -975);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1015, 0.1f, -1005);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(985, 0.1f, -1005);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(985, 0.1f, -1135);//ʮ��·��
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1015, 0.1f, -1135);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1015, 0.1f, -1165);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(985, 0.1f, -1165);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1045, 0.1f, -975);//ʮ��·��
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1075, 0.1f, -975);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1075, 0.1f, -1005);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1045, 0.1f, -1005);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1045, 0.1f, -1135);//ʮ��·��
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1075, 0.1f, -1135);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1075, 0.1f, -1165);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1045, 0.1f, -1165);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, m_texture[14]);//��Ȼ��԰
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(820, 0.1f, -850);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(920, 0.1f, -850);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(920, 0.1f, -970);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(820, 0.1f, -970);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, m_texture[1]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(600, 0, -850);//���л���·��1a
		glTexCoord2f(0.0f, 0.0f); glVertex3f(890, 0, -850);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(890, 0, -980);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(600, 0, -980);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(890, 0, -850);//���л���·��1a
		glTexCoord2f(0.0f, 0.0f); glVertex3f(930, 0, -850);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(930, 0, -980);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(890, 0, -980);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(950, 0, -850);//���л���·��2a
		glTexCoord2f(0.0f, 0.0f); glVertex3f(990, 0, -850);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(990, 0, -980);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(950, 0, -980);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1010, 0, -850);//���л���·��3a
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1050, 0, -850);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1050, 0, -980);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1010, 0, -980);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1070, 0, -850);//���л���·��4a
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1080, 0, -850);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1080, 0, -980);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1070, 0, -980);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1070, 0, -970);//���л���·��4a
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1400, 0, -970);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1400, 0, -980);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1070, 0, -980);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1150, 0, -850);//���л���·��4a
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1400, 0, -850);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1400, 0, -980);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1150, 0, -980);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(600, 0, -1000);//���л���·��1b
		glTexCoord2f(0.0f, 0.0f); glVertex3f(890, 0, -1000);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(890, 0, -1140);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(600, 0, -1140);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(890, 0, -1000);//���л���·��1b
		glTexCoord2f(0.0f, 0.0f); glVertex3f(930, 0, -1000);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(930, 0, -1140);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(890, 0, -1140);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(950, 0, -1000);//���л���·��2b
		glTexCoord2f(0.0f, 0.0f); glVertex3f(990, 0, -1000);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(990, 0, -1140);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(950, 0, -1140);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1010, 0, -1000);//���л���·��3b
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1050, 0, -1000);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1050, 0, -1140);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1010, 0, -1140);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1070, 0, -1000);//���л���·��4b
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1400, 0, -1000);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1400, 0, -1140);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1070, 0, -1140);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(600, 0, -1160);//���л���·��1c
		glTexCoord2f(0.0f, 0.0f); glVertex3f(890, 0, -1160);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(890, 0, -1400);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(600, 0, -1400);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(890, 0, -1160);//���л���·��1c
		glTexCoord2f(0.0f, 0.0f); glVertex3f(930, 0, -1160);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(930, 0, -1400);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(890, 0, -1400);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(950, 0, -1160);//���л���·��2c
		glTexCoord2f(0.0f, 0.0f); glVertex3f(990, 0, -1160);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(990, 0, -1400);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(950, 0, -1400);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1010, 0, -1160);//���л���·��3c
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1050, 0, -1160);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1050, 0, -1400);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1010, 0, -1400);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1070, 0, -1160);//���л���·��4c
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1400, 0, -1160);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1400, 0, -1400);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1070, 0, -1400);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(600, 0, -600);//���л���·��ǰ
		glTexCoord2f(0.0f, 0.0f); glVertex3f(930, 0, -600);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(930, 0, -850);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(600, 0, -850);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(950, 0, -600);//���л���·��ǰ
		glTexCoord2f(0.0f, 0.0f); glVertex3f(990, 0, -600);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(990, 0, -850);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(950, 0, -850);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1010, 0, -600);//���л���·��ǰ
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1050, 0, -600);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1050, 0, -850);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1010, 0, -850);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1070, 0, -600);//���л���·��ǰ
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1400, 0, -600);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1400, 0, -850);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1070, 0, -850);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, m_texture[9]);	
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1080, 0, -850);//�ܽ��ܵ�
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1085, 0, -850);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1085, 0, -895);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1080, 0, -895);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1145, 0, -850);//�ܽ��ܵ�
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1150, 0, -850);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1150, 0, -895);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1145, 0, -895);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, m_texture[25]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1085, 0, -850);//�ܽ��ܵ�
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1145, 0, -850);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1145, 0, -855);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1085, 0, -855);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1085, 0, -890);//�ܽ��ܵ�
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1145, 0, -890);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1145, 0, -895);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1085, 0, -895);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, m_texture[13]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1080, 0, -895); // ���򳡵ذ�
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1085, 0, -895);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1085, 0, -970);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1080, 0, -970);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1100, 0, -895);// ���򳡵ذ�
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1105, 0, -895);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1105, 0, -965);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1100, 0, -965);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1120, 0, -895);// ���򳡵ذ�
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1125, 0, -895);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1125, 0, -965);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1120, 0, -965);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1140, 0, -895);// ���򳡵ذ�
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1150, 0, -895);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1150, 0, -965);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1140, 0, -965);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1085, 0, -895);// ���򳡵ذ�
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1145, 0, -895);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1145, 0, -900);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1080, 0, -900);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1085, 0, -930);// ���򳡵ذ�
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1145, 0, -930);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1145, 0, -935);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1080, 0, -935);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1085, 0, -965);// ���򳡵ذ�
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1150, 0, -965);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1150, 0, -970);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1080, 0, -970);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, m_texture[17]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1106, 24.5, -1012);//ͼ���
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1119, 24.5, -1012);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1119, 28.0, -1012);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1106, 28.0, -1012);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, m_texture[18]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1081, 22.0, -1090);//����
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1081, 26.0, -1090);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1081, 26.0, -1120);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1081, 22.0, -1120);
		glEnd();
	glPopMatrix();



	/////////////��������///////////////////
	building_render();
	glDisable(GL_CULL_FACE);//ȡ���޳�
	/////////////��ľ����///////////////////
	tree_render();
	/////////////�ֵƻ���///////////////////
	streetlight_render();
	/////////////�˶�������/////////////////
	playground_render();
	

	
}



void CGround::building_render()
{
	building(21,21,21,22,21, 919.0f, -921.0f, 1.0f, 10.0f, 5.0f, height_base);//��԰����   
	//����2a
	building(80, 80, 80, 80, 81,/*house27*/ 960.0f, -850.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(72, 72, 72, 72, 73,/*house23*/ 960.0f, -875.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(78, 78, 78, 78, 79,/*house26*/ 960.0f, -900.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(76, 76, 76, 76, 77,/*house25*/960.0f, -925.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(88, 88, 88, 88, 89,/*house31*/  960.0f, -950.0f, 20.0f, 20.0f, 20.0f, height_base);
	//����2b
	building(74, 74, 74, 74, 75,/*house24*/ 960.0f, -1010.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(66, 66, 66, 66, 67, /*house20*/960.0f, -1035.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(35, 35, 35, 35, 36,/*house5*/ 960.0f, -1060.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(37, 37, 37, 37, 38,/*house6*/ 960.0f, -1085.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(60, 60, 60, 60, 61, /*house17*/960.0f, -1110.0f, 20.0f, 20.0f, 20.0f, height_base);
	//����2c
	building(46, 46, 46, 46, 47,/*house10*/960.0f, -1170.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(62, 62, 62, 62, 63,/*house18*/960.0f, -1195.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(44, 44, 44, 44, 45,/*house9*/   960.0f, -1220.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(94, 94, 94, 94, 95 ,/*house34*/960.0f, -1245.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(98, 98, 98, 98, 99 ,/*house36*/960.0f, -1270.0f, 20.0f, 20.0f, 20.0f, height_base);
	//����3a
	building(50, 50, 50, 50, 51,/*house12*/  1020.0f, -850.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(52, 52, 52, 52, 53,/*house13*/ 1020.0f, -875.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(84, 84, 84, 84, 85,/*house29*/ 1020.0f, -900.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(86, 86, 86, 86, 87,/*house30*/ 1020.0f, -925.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(82, 82, 82, 82, 83,/*house28*/1020.0f, -950.0f, 20.0f, 20.0f, 20.0f, height_base);
	//����3b
	building(56, 56, 56, 56, 57,/*house15*/ 1020.0f, -1010.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(68, 68, 68, 68, 69, /*house21*/1020.0f, -1035.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(31, 31, 31, 31, 32,/*house2*/1020.0f, -1060.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(39, 39, 39, 39, 40,/*house7*/1020.0f, -1085.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(23, 23, 23, 23, 24,/*house3*/1020.0f, -1110.0f, 20.0f, 20.0f, 20.0f, height_base);
	//����3c
	building(48, 48, 48, 48, 49,/*house11*/1020.0f, -1170.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(70, 70, 70, 70, 71,/*house22*/ 1020.0f, -1195.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(64, 64, 64, 64, 65, /*house19*/  1020.0f, -1220.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(96, 96, 96, 96, 97,/*house35*/1020.0f, -1245.0f, 20.0f, 20.0f, 20.0f, height_base);
	building(90, 90, 90, 90, 91,/*house32*/ 1020.0f, -1270.0f, 20.0f, 20.0f, 20.0f, height_base);
	//����4c
	building(58, 58, 58, 58, 59,/*house16*/ 1080.0f, -1170.0f, 20.0f, 20.0f, 30.0f, height_base);
	building(54, 54, 54, 54, 55,/*house14*/1080.0f, -1195.0f, 20.0f, 20.0f, 30.0f, height_base);
	building(41, 41, 42, 42, 43,/*house8*/ 1080.0f, -1220.0f, 20.0f, 20.0f, 30.0f, height_base);
	building(92, 92, 92, 92, 93,/*house33*/ 1080.0f, -1245.0f, 20.0f, 20.0f, 30.0f, height_base);
	building(33, 33, 33, 33, 34,/*house4*/ 1080.0f, -1270.0f, 20.0f, 20.0f, 30.0f, height_base);
	
	//����1c
	building(106, 106, 107, 107, 108,/*hotel*/890.0f, -1175.0f, 30.0f, 10.0f, 30.0f, height_base);
	building(106, 106, 107, 107, 108,/*hotel*/890.0f, -1210.0f, 30.0f, 10.0f, 30.0f, height_base);
	building(106, 106, 107, 107, 108,/*hotel*/890.0f, -1245.0f, 30.0f, 10.0f, 30.0f, height_base);
	building(106, 106, 107, 107, 108,/*hotel*/890.0f, -1280.0f, 30.0f, 10.0f, 30.0f, height_base);
	
	double tt = 0.0; double dtt = 2 * 3.14159 / 5;
	glBindTexture(GL_TEXTURE_2D, m_texture[108]);

	glPushMatrix();	glTranslatef(897, 0, -1180);	glBegin(GL_QUAD_STRIP);	
	for (int j = 0; j <= 5; ++j)	{		
		glTexCoord2f(tt / (2 * 3.14159), 1.0); glVertex3f(0.0, 36.0, 0.0);
		glTexCoord2f(tt / (2 * 3.14159), 0.0); glVertex3f(5*cos(tt), 30.0, -sin(tt)*5);		tt = tt + dtt;	
	}
	glEnd();	glPopMatrix();
	glPushMatrix();	glTranslatef(912, 0, -1180);	glBegin(GL_QUAD_STRIP);
	for (int j = 0; j <= 5; ++j) {
		glTexCoord2f(tt / (2 * 3.14159), 1.0); glVertex3f(0.0, 36.0, 0.0);
		glTexCoord2f(tt / (2 * 3.14159), 0.0); glVertex3f(5 * cos(tt), 30.0, -sin(tt) * 5);		tt = tt + dtt;
	}
	glEnd();	glPopMatrix(); 

	glPushMatrix();	glTranslatef(897, 0, -1215);	glBegin(GL_QUAD_STRIP);
	for (int j = 0; j <= 5; ++j) {
		glTexCoord2f(tt / (2 * 3.14159), 1.0); glVertex3f(0.0, 36.0, 0.0);
		glTexCoord2f(tt / (2 * 3.14159), 0.0); glVertex3f(5 * cos(tt), 30.0, -sin(tt) * 5);		tt = tt + dtt;
	}
	glEnd();	glPopMatrix();
	glPushMatrix();	glTranslatef(912, 0, -1215);	glBegin(GL_QUAD_STRIP);
	for (int j = 0; j <= 5; ++j) {
		glTexCoord2f(tt / (2 * 3.14159), 1.0); glVertex3f(0.0, 36.0, 0.0);
		glTexCoord2f(tt / (2 * 3.14159), 0.0); glVertex3f(5 * cos(tt), 30.0, -sin(tt) * 5);		tt = tt + dtt;
	}
	glEnd();	glPopMatrix();
	glPushMatrix();	glTranslatef(897, 0, -1250);	glBegin(GL_QUAD_STRIP);
	for (int j = 0; j <= 5; ++j) {
		glTexCoord2f(tt / (2 * 3.14159), 1.0); glVertex3f(0.0, 36.0, 0.0);
		glTexCoord2f(tt / (2 * 3.14159), 0.0); glVertex3f(5 * cos(tt), 30.0, -sin(tt) * 5);		tt = tt + dtt;
	}
	glEnd();	glPopMatrix();
	glPushMatrix();	glTranslatef(912, 0, -1250);	glBegin(GL_QUAD_STRIP);
	for (int j = 0; j <= 5; ++j) {
		glTexCoord2f(tt / (2 * 3.14159), 1.0); glVertex3f(0.0, 36.0, 0.0);
		glTexCoord2f(tt / (2 * 3.14159), 0.0); glVertex3f(5 * cos(tt), 30.0, -sin(tt) * 5);		tt = tt + dtt;
	}
	glEnd();	glPopMatrix();
	glPushMatrix();	glTranslatef(897, 0, -1285);	glBegin(GL_QUAD_STRIP);
	for (int j = 0; j <= 5; ++j) {
		glTexCoord2f(tt / (2 * 3.14159), 1.0); glVertex3f(0.0, 36.0, 0.0);
		glTexCoord2f(tt / (2 * 3.14159), 0.0); glVertex3f(5 * cos(tt), 30.0, -sin(tt) * 5);		tt = tt + dtt;
	}
	glEnd();	glPopMatrix();
	glPushMatrix();	glTranslatef(912, 0, -1285);	glBegin(GL_QUAD_STRIP);
	for (int j = 0; j <= 5; ++j) {
		glTexCoord2f(tt / (2 * 3.14159), 1.0); glVertex3f(0.0, 36.0, 0.0);
		glTexCoord2f(tt / (2 * 3.14159), 0.0); glVertex3f(5 * cos(tt), 30.0, -sin(tt) * 5);		tt = tt + dtt;
	}
	glEnd();	glPopMatrix();

	//����
	building(101, 101, 100, 101, 102,/*market*/ 1081.1f, -1080.0f, 25.0f, 28.0f, 30.0f, height_base);
	building(101, 101, 100, 101, 102,/*market*/ 1081.1f, -1105.0f, 25.0f, 28.0f, 30.0f, height_base);	

	//ͼ���
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, m_texture[103]);    // ѡ������	
	glBegin(GL_QUADS); //����
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1104, 0, -1012); // ������ı��ε�����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1122, 0, -1012); // ������ı��ε�����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1122, 26, -1012);// ������ı��ε�����
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1104, 26, -1012); // ������ı��ε�����
	glEnd();
	glBegin(GL_QUADS);// �����
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1095, 0, -1020); // ������ı��ε�����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1104, 0, -1012); // ������ı��ε�����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1104, 26, -1012); // ������ı��ε�����
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1095, 26, -1020); // ������ı��ε�����
	glEnd();
	glBegin(GL_QUADS);// �Ҳ���
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1122, 0.0f, -1012); // ������ı��ε�����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1131, 0.0f, -1020); // ������ı��ε�����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1131, 26, -1020); // ������ı��ε�����
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1122, 26, -1012); // ������ı��ε�����
	glEnd();
	glBegin(GL_QUADS); //������
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1080, 0, -1020); // ������ı��ε�����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1095, 0, -1020); // ������ı��ε�����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1095, 26, -1020);// ������ı��ε�����
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1080, 26, -1020); // ������ı��ε�����
	glEnd();
	glBegin(GL_QUADS); //������
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1131, 0, -1020); // ������ı��ε�����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1146, 0, -1020); // ������ı��ε�����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1146, 26, -1020);// ������ı��ε�����
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1131, 26, -1020); // ������ı��ε�����
	glEnd();	
	glBegin(GL_QUADS);// ����
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1080, 0.0f, -1060); // ������ı��ε�����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1080, 0.0f, -1020); // ������ı��ε�����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1080, 26, -1020); // ������ı��ε�����
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1080, 26, -1060); // ������ı��ε�����
	glEnd();	
	glBegin(GL_QUADS);// ����
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1146, 0.0f, -1020); // ������ı��ε�����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1146, 0.0f, -1060); // ������ı��ε�����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1146, 26, -1060); // ������ı��ε�����
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1146, 26, -1020); // ������ı��ε�����
	glEnd();	
	glBindTexture(GL_TEXTURE_2D, m_texture[104]);    // ѡ������	
	glBegin(GL_QUADS);// ����
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1146, 0.0f, -1060); // ������ı��ε�����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1080, 0.0f, -1060); // ������ı��ε�����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1080, 26, -1060); // ������ı��ε�����
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1146, 26, -1060); // ������ı��ε�����
	glEnd();	
	glBindTexture(GL_TEXTURE_2D, m_texture[105]);    // ѡ������	
	glBegin(GL_QUADS);// ����
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1080, 26, -1020); // ������ı��ε�����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1146, 26, -1020); // ������ı��ε�����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1146, 26, -1060); // ������ı��ε�����
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1080, 26, -1060); // ������ı��ε�����
	glEnd();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1104, 26, -1012); // ������ı��ε�����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1122, 26, -1012); // ������ı��ε�����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1131, 26, -1020); // ������ı��ε�����
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1095, 26, -1020); // ������ı��ε�����
	glEnd();
	glPopMatrix();

	//ҽԺ
	building(113, 113, 113, 113, 114,/*hospital*/ 895.0f, -1080.0f, 25.0f, 17.0f, 28.0f, 15);
	building(113, 113, 113, 113, 114,/*hospital*/ 895.0f, -1113.0f, 25.0f, 17.0f, 28.0f, 15);
	building(117, 117, 116, 115, 114,/*hospital*/ 895.0f, -1080.0f, 25.0f, 50.0f, 15.0f, 0);
	building(114, 114, 118, 118, 114,/*hospital*/ 920.0f, -1082.0f, 0.2f,  13.0f, 30.0f, 28);
	building(114, 114, 118, 118, 114,/*hospital*/ 920.0f, -1115.0f, 0.2f,  13.0f, 30.0f, 28);
	
	//����	
//	building(120, 117, 116, 115, 120,/*bank*/ 860.0f, -1030.0f, 60.0f, 40.0f, 25.0f, 0);//��
//	building(117, 117, 116, 120, 120,/*bank*/ 860.0f, -1010.0f, 40.0f, 60.0f, 25.0f, 0);//��
	building(121, 121, 121, 121, 120,/*bank*/ 860.0f, -1030.0f, 60.0f, 15.0f, 22.0f, 0);//��
	building(121, 121, 121, 121, 120,/*bank*/ 860.0f, -1045.0f, 55.0f, 10.0f, 22.0f, 0);//��
	building(121, 121, 121, 121, 120,/*bank*/ 860.0f, -1055.0f, 60.0f, 15.0f, 22.0f, 0);//��
	building(121, 121, 121, 121, 120,/*bank*/ 860.0f, -1010.0f, 15.0f, 40.0f, 22.0f, 0);//��
	building(121, 121, 121, 121, 120,/*bank*/ 875.0f, -1015.0f, 10.0f, 40.0f, 22.0f, 0);//��
	building(121, 121, 121, 121, 120,/*bank*/ 885.0f, -1010.0f, 15.0f, 40.0f, 22.0f, 0);//��
	glPushMatrix();//����
	glBindTexture(GL_TEXTURE_2D, m_texture[120]);    // ѡ������
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(900, 22, -1010); // ������ı��ε�����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(920, 22, -1010); // ������ı��ε�����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(920, 22, -1030); // ������ı��ε�����
	glTexCoord2f(0.0f, 1.0f); glVertex3f(900, 22, -1030); // ������ı��ε�����
	glEnd();
	glPopMatrix();
	glPushMatrix();//Բ����
	glTranslatef(900, 0.0, -1030.0);
	glBindTexture(GL_TEXTURE_2D, m_texture[119]);
	glBegin(GL_QUAD_STRIP);
	int nslice = 15; double t = 0.0; double dt = 2 * PI / nslice;
	for (int j = 0; j <= nslice; ++j)
	{
		glTexCoord2f(t / (2 * PI), 1.0); glVertex3f(20*cos(t), 22.0, -sin(t)*20);
		glTexCoord2f(t / (2 * PI), 0.0); glVertex3f(20*cos(t), 0.0, -sin(t)*20);
		t = t + dt;
	}
	glEnd();
	glPopMatrix();
}



void CGround::tree_render()
{
	/////////////////·�ߵ���
	tree2(985, -825.0f - 18, height_tree, height_base, 1);
	tree1(985, -850.0f - 18, height_tree, height_base, 1);
	tree2(985, -875.0f - 18, height_tree, height_base, 1);
	tree1(985, -900.0f - 18, height_tree, height_base, 1);
	tree2(985, -925.0f - 18, height_tree, height_base, 1);
	tree1(985, -950.0f - 18, height_tree, height_base, 1);

	tree2(985, -1010.0f - 18, height_tree, height_base, 1);
	tree1(985, -1035.0f - 18, height_tree, height_base, 1);
	tree2(985, -1060.0f - 18, height_tree, height_base, 1);
	tree1(985, -1085.0f - 18, height_tree, height_base, 1);
	tree2(985, -1110.0f - 18, height_tree, height_base, 1);

	tree1(985, -1170.0f - 18, height_tree, height_base, 1);
	tree2(985, -1195.0f - 18, height_tree, height_base, 1);
	tree1(985, -1220.0f - 18, height_tree, height_base, 1);
	tree2(985, -1245.0f - 18, height_tree, height_base, 1);
	tree1(985, -1270.0f - 18, height_tree, height_base, 1);
/////////////////·�ߵ���
	tree2(1015, -825.0f - 18, height_tree, height_base, 1);
	tree1(1015, -850.0f - 18, height_tree, height_base, 1);
	tree2(1015, -875.0f - 18, height_tree, height_base, 1);
	tree1(1015, -900.0f - 18, height_tree, height_base, 1);
	tree2(1015, -925.0f - 18, height_tree, height_base, 1);
	tree1(1015, -950.0f - 18, height_tree, height_base, 1);

	tree2(1015, -1010.0f - 18, height_tree, height_base, 1);
	tree1(1015, -1035.0f - 18, height_tree, height_base, 1);
	tree2(1015, -1060.0f - 18, height_tree, height_base, 1);
	tree1(1015, -1085.0f - 18, height_tree, height_base, 1);
	tree2(1015, -1110.0f - 18, height_tree, height_base, 1);

	tree1(1015, -1170.0f - 18, height_tree, height_base, 1);
	tree2(1015, -1195.0f - 18, height_tree, height_base, 1);
	tree1(1015, -1220.0f - 18, height_tree, height_base, 1);
	tree2(1015, -1245.0f - 18, height_tree, height_base, 1);
	tree1(1015, -1270.0f - 18, height_tree, height_base, 1);
/////////////////·�ߵ���
	tree2(925, -825.0f - 18, height_tree, height_base, 1);
	tree1(925, -850.0f - 18, height_tree, height_base, 1);
	tree2(925, -875.0f - 18, height_tree, height_base, 1);
	tree1(925, -900.0f - 18, height_tree, height_base, 1);
	tree2(925, -925.0f - 18, height_tree, height_base, 1);
	tree1(925, -950.0f - 18, height_tree, height_base, 1);

	tree2(925, -1010.0f - 18, height_tree, height_base, 1);
	tree1(925, -1035.0f - 18, height_tree, height_base, 1);
	tree2(925, -1060.0f - 18, height_tree, height_base, 1);
	tree1(925, -1085.0f - 18, height_tree, height_base, 1);
	tree2(925, -1110.0f - 18, height_tree, height_base, 1);

	tree1(925, -1170.0f - 18, height_tree, height_base, 1);
	tree2(925, -1195.0f - 18, height_tree, height_base, 1);
	tree1(925, -1220.0f - 18, height_tree, height_base, 1);
	tree2(925, -1245.0f - 18, height_tree, height_base, 1);
	tree1(925, -1270.0f - 18, height_tree, height_base, 1);
/////////////////·�ߵ���
	tree2(955, -825.0f - 18, height_tree, height_base, 1);
	tree1(955, -850.0f - 18, height_tree, height_base, 1);
	tree2(955, -875.0f - 18, height_tree, height_base, 1);
	tree1(955, -900.0f - 18, height_tree, height_base, 1);
	tree2(955, -925.0f - 18, height_tree, height_base, 1);
	tree1(955, -950.0f - 18, height_tree, height_base, 1);

	tree2(955, -1010.0f - 18, height_tree, height_base, 1);
	tree1(955, -1035.0f - 18, height_tree, height_base, 1);
	tree2(955, -1060.0f - 18, height_tree, height_base, 1);
	tree1(955, -1085.0f - 18, height_tree, height_base, 1);
	tree2(955, -1110.0f - 18, height_tree, height_base, 1);

	tree1(955, -1170.0f - 18, height_tree, height_base, 1);
	tree2(955, -1195.0f - 18, height_tree, height_base, 1);
	tree1(955, -1220.0f - 18, height_tree, height_base, 1);
	tree2(955, -1245.0f - 18, height_tree, height_base, 1);
	tree1(955, -1270.0f - 18, height_tree, height_base, 1);
/////////////////·�ߵ���
	tree2(1045, -825.0f - 18, height_tree, height_base, 1);
	tree1(1045, -850.0f - 18, height_tree, height_base, 1);
	tree2(1045, -875.0f - 18, height_tree, height_base, 1);
	tree1(1045, -900.0f - 18, height_tree, height_base, 1);
	tree2(1045, -925.0f - 18, height_tree, height_base, 1);
	tree1(1045, -950.0f - 18, height_tree, height_base, 1);

	tree2(1045, -1010.0f - 18, height_tree, height_base, 1);
	tree1(1045, -1035.0f - 18, height_tree, height_base, 1);
	tree2(1045, -1060.0f - 18, height_tree, height_base, 1);
	tree1(1045, -1085.0f - 18, height_tree, height_base, 1);
	tree2(1045, -1110.0f - 18, height_tree, height_base, 1);

	tree1(1045, -1170.0f - 18, height_tree, height_base, 1);
	tree2(1045, -1195.0f - 18, height_tree, height_base, 1);
	tree1(1045, -1220.0f - 18, height_tree, height_base, 1);
	tree2(1045, -1245.0f - 18, height_tree, height_base, 1);
	tree1(1045, -1270.0f - 18, height_tree, height_base, 1);
/////////////////·�ߵ���
	tree2(1075, -825.0f - 18, height_tree, height_base, 1);
	tree1(1075, -850.0f - 18, height_tree, height_base, 1);
	tree2(1075, -875.0f - 18, height_tree, height_base, 1);
	tree1(1075, -900.0f - 18, height_tree, height_base, 1);
	tree2(1075, -925.0f - 18, height_tree, height_base, 1);
	tree1(1075, -950.0f - 18, height_tree, height_base, 1);

	tree2(1075, -1010.0f - 18, height_tree, height_base, 1);
	tree1(1075, -1035.0f - 18, height_tree, height_base, 1);
	tree2(1075, -1060.0f - 18, height_tree, height_base, 1);
	tree1(1075, -1085.0f - 18, height_tree, height_base, 1);
	tree2(1075, -1110.0f - 18, height_tree, height_base, 1);

	tree1(1075, -1170.0f - 18, height_tree, height_base, 1);
	tree2(1075, -1195.0f - 18, height_tree, height_base, 1);
	tree1(1075, -1220.0f - 18, height_tree, height_base, 1);
	tree2(1075, -1245.0f - 18, height_tree, height_base, 1);
	tree1(1075, -1270.0f - 18, height_tree, height_base, 1);

	/////////////////��԰����
	for (int i = 826; i < 920; i += 2)
	{
		tree1(i, -852, height_tree, height_base, 1);
		tree1(i, -968, height_tree, height_base, 1);
	}
	for (int j = -852; j>-968; j -= 2)
	{
		tree1(826, j, height_tree, height_base, 1);
	}
	for (int j = -852; j>-896; j -= 2)
	{
		tree1(920, j, height_tree, height_base, 1);
	}
	for (int j = -932; j>-968; j -= 2)
	{
		tree1(920, j, height_tree, height_base, 1);
	}
	for (int i = 830; i < 870; i += 8)
		for (int j = -856; j>-964; j -= 8)
		{
			tree2(i, j, height_tree, height_base, 1);
		}

}

void CGround::streetlight_render()
{
	glPushMatrix();	glTranslatef(986, 0, -850.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(986, 0, -875.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(986, 0, -900.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(986, 0, -925.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(986, 0, -950.0f - 22);	streetlight();	glPopMatrix();
	
	glPushMatrix();	glTranslatef(986, 0, -1010.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(986, 0, -1035.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(986, 0, -1060.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(986, 0, -1085.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(986, 0, -1110.0f - 22);	streetlight();	glPopMatrix();

	glPushMatrix();	glTranslatef(986, 0, -1170.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(986, 0, -1195.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(986, 0, -1220.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(986, 0, -1245.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(986, 0, -1270.0f - 22);	streetlight();	glPopMatrix();
		
	glPushMatrix();	glTranslatef(1014, 0, -850.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1014, 0, -875.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1014, 0, -900.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1014, 0, -925.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1014, 0, -950.0f - 22);	streetlight();	glPopMatrix();

	glPushMatrix();	glTranslatef(1014, 0, -1010.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1014, 0, -1035.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1014, 0, -1060.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1014, 0, -1085.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1014, 0, -1110.0f - 22);	streetlight();	glPopMatrix();

	glPushMatrix();	glTranslatef(1014, 0, -1170.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1014, 0, -1195.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1014, 0, -1220.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1014, 0, -1245.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1014, 0, -1270.0f - 22);	streetlight();	glPopMatrix();

	glPushMatrix();	glTranslatef(926, 0, -850.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(926, 0, -875.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(926, 0, -900.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(926, 0, -925.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(926, 0, -950.0f - 22);	streetlight();	glPopMatrix();

	glPushMatrix();	glTranslatef(926, 0, -1010.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(926, 0, -1035.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(926, 0, -1060.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(926, 0, -1085.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(926, 0, -1110.0f - 22);	streetlight();	glPopMatrix();

	glPushMatrix();	glTranslatef(926, 0, -1170.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(926, 0, -1195.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(926, 0, -1220.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(926, 0, -1245.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(926, 0, -1270.0f - 22);	streetlight();	glPopMatrix();

	glPushMatrix();	glTranslatef(954, 0, -850.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(954, 0, -875.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(954, 0, -900.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(954, 0, -925.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(954, 0, -950.0f - 22);	streetlight();	glPopMatrix();

	glPushMatrix();	glTranslatef(954, 0, -1010.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(954, 0, -1035.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(954, 0, -1060.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(954, 0, -1085.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(954, 0, -1110.0f - 22);	streetlight();	glPopMatrix();

	glPushMatrix();	glTranslatef(954, 0, -1170.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(954, 0, -1195.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(954, 0, -1220.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(954, 0, -1245.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(954, 0, -1270.0f - 22);	streetlight();	glPopMatrix();

	glPushMatrix();	glTranslatef(1046, 0, -850.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1046, 0, -875.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1046, 0, -900.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1046, 0, -925.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1046, 0, -950.0f - 22);	streetlight();	glPopMatrix();

	glPushMatrix();	glTranslatef(1046, 0, -1010.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1046, 0, -1035.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1046, 0, -1060.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1046, 0, -1085.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1046, 0, -1110.0f - 22);	streetlight();	glPopMatrix();

	glPushMatrix();	glTranslatef(1046, 0, -1170.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1046, 0, -1195.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1046, 0, -1220.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1046, 0, -1245.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1046, 0, -1270.0f - 22);	streetlight();	glPopMatrix();

	glPushMatrix();	glTranslatef(1074, 0, -850.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1074, 0, -875.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1074, 0, -900.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1074, 0, -925.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1074, 0, -950.0f - 22);	streetlight();	glPopMatrix();

	glPushMatrix();	glTranslatef(1074, 0, -1010.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1074, 0, -1035.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1074, 0, -1060.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1074, 0, -1085.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1074, 0, -1110.0f - 22);	streetlight();	glPopMatrix();

	glPushMatrix();	glTranslatef(1074, 0, -1170.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1074, 0, -1195.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1074, 0, -1220.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1074, 0, -1245.0f - 22);	streetlight();	glPopMatrix();
	glPushMatrix();	glTranslatef(1074, 0, -1270.0f - 22);	streetlight();	glPopMatrix();
	
}

void CGround::playground_render()
{
	football();//����
	glPushMatrix();	glTranslatef(1085.0f, 0, -930.f);basketball();glPopMatrix();//����
	glPushMatrix();	glTranslatef(1105.0f, 0, -930.f);basketball();glPopMatrix();
	glPushMatrix();	glTranslatef(1125.0f, 0, -930.f);basketball();glPopMatrix();
	glPushMatrix();	glTranslatef(1085.0f, 0, -965.f); basketball(); glPopMatrix();//����
	glPushMatrix();	glTranslatef(1105.0f, 0, -965.f); basketball(); glPopMatrix();
	glPushMatrix();	glTranslatef(1125.0f, 0, -965.f); basketball(); glPopMatrix();
//	tennis();

}


void  CGround::building(int tex1,int tex2,int tex3,int tex4,int tex5, float x, float z, float w, float l, float h, float h_add)//������ţ�����X������Z����ȣ����ȣ��߶ȣ�΢���߶ȣ���ת�Ƕ�
{
	glPushMatrix();
	// ǰ��
	glBindTexture(GL_TEXTURE_2D, m_texture[tex1]);    // ѡ������
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, h_add, z); // ������ı��ε�����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + w, h_add, z); // ������ı��ε�����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + w, h, z);// ������ı��ε�����
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, h, z); // ������ı��ε�����
	glEnd();
	// ����
	glBindTexture(GL_TEXTURE_2D, m_texture[tex2]);    // ѡ������
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + w, h_add, z - l); // ������ı��ε�����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, h_add, z - l); // ������ı��ε�����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, h, z - l); // ������ı��ε�����
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + w, h, z - l); // ������ı��ε�����
	glEnd();
	// ����
	glBindTexture(GL_TEXTURE_2D, m_texture[tex3]);    // ѡ������
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, h_add, z - l); // ������ı��ε�����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, h_add, z); // ������ı��ε�����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, h, z); // ������ı��ε�����
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, h, z - l); // ������ı��ε�����
	glEnd();
	// ����
	glBindTexture(GL_TEXTURE_2D, m_texture[tex4]);    // ѡ������
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + w, h_add, z); // ������ı��ε�����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + w, h_add, z - l); // ������ı��ε�����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + w, h, z - l); // ������ı��ε�����
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + w, h, z); // ������ı��ε�����
	glEnd();
	// ����
	glBindTexture(GL_TEXTURE_2D, m_texture[tex5]);    // ѡ������
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + w, h, z - l); // ������ı��ε�����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, h, z - l); // ������ı��ε�����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, h, z); // ������ı��ε�����
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + w, h, z);// ������ı��ε�����
	glEnd();
	glPopMatrix();
}

void CGround::tree1(float x, float z, float h,float h_add,float size)
{
	/////////��������������������������������������///////////
	float p = size;
	glPushMatrix();
	glTranslatef(x,h_add, z);
	glBindTexture(GL_TEXTURE_2D, m_texture[5]); //��������8��1M��
	glBegin(GL_QUADS);									//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.09f, h * p*0.0f, p*0.03f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.09f, h * p*0.0f, -p*0.03f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*0.09f, h * p*1.0f, -p*0.03f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*0.09f, h * p*1.0f, p*0.03f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.09f, h * p*0.0f, -p*0.03f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.03f, h * p*0.0f, -p*0.09f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*0.03f, h * p*1.0f, -p*0.09f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*0.09f, h * p*1.0f, -p*0.03f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.03f, h * p*0.0f, -p*0.09f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.03f, h * p*0.0f, -p*0.09f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*0.03f, h * p*1.0f, -p*0.09f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*0.03f, h * p*1.0f, -p*0.09f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.03f, h * p*0.0f, -p*0.09f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.09f, h * p*0.0f, -p*0.03f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*0.09f, h * p*1.0f, -p*0.03f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*0.03f, h * p*1.0f, -p*0.09f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.09f, h * p*0.0f, -p*0.03f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.09f, h * p*0.0f, p*0.03f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*0.09f, h * p*1.0f, p*0.03f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*0.09f, h * p*1.0f, -p*0.03f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.09f, h * p*0.0f, p*0.03f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.03f, h * p*0.0f, p*0.09f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*0.03f, h * p*1.0f, p*0.09f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*0.09f, h * p*1.0f, p*0.03f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.03f, h * p*0.0f, p*0.09f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.03f, h * p*0.0f, p*0.09f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*0.03f, h * p*1.0f, p*0.09f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*0.03f, h * p*1.0f, p*0.09f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.03f, h * p*0.0f, p*0.09f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.09f, h * p*0.0f, p*0.03f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*0.09f, h * p*1.0f, p*0.03f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*0.03f, h * p*1.0f, p*0.09f);
	glEnd();
	/////---------��ҳ�²�-----------------------
	glBindTexture(GL_TEXTURE_2D, m_texture[6]); //������ҳ8��0.4M��
	glBegin(GL_QUADS);									//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.36f, h * p*0.5f, p* 0.12f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.36f, h * p*0.5f, -p* 0.12f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p* 0.9f, h * p*1.4f, -p* 0.3f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p* 0.9f, h * p*1.4f, p* 0.3f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.36f, h * p*0.5f, -p*0.12f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.12f, h * p*0.5f, -p*0.36f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*00.3f, h * p*1.4f, -p*00.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.9f, h * p*1.4f, -p*00.3f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.12f, h * p*0.5f, -p*0.36f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.12f, h * p*0.5f, -p*0.36f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.3f, h * p*1.4f, -p*00.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.3f, h * p*1.4f, -p*00.9f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.12f, h * p*0.5f, -p*0.36f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.36f, h * p*0.5f, -p*0.12f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.9f, h * p*1.4f, -p*00.3f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*00.3f, h * p*1.4f, -p*00.9f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.36f, h * p*0.5f, -p*0.12f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.36f, h * p*0.5f, p*0.12f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.9f, h * p*1.4f, p*00.3f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*00.9f, h * p*1.4f, -p*00.3f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.36f, h * p*0.5f, p*0.12f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.12f, h * p*0.5f, p*0.36f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.3f, h * p*1.4f, p*00.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*00.9f, h * p*1.4f, p*00.3f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.12f, h * p*0.5f, p*0.36f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.12f, h * p*0.5f, p*0.36f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.3f, h * p*1.4f, p*00.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.3f, h * p*1.4f, p*00.9f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.12f, h * p*0.5f, p*0.36f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.36f, h * p*0.5f, p*0.12f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*00.9f, h * p*1.4f, p*00.3f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.3f, h * p*1.4f, p*00.9f);
	/////---------��ҳ�в�-----------------------
	glBindTexture(GL_TEXTURE_2D, m_texture[6]); //������ҳ8��1.0M��
	glBegin(GL_QUADS);									//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.9f, h * p*1.4f, p*0.3f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.9f, h * p*1.4f, -p*0.3f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*0.6f, h * p*3.4f, -p*0.2f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*0.6f, h * p*3.4f, p*0.2f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*00.9f, h * p*1.4f, -p*00.3f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*00.3f, h * p*1.4f, -p*00.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*00.2f, h * p*3.4f, -p*00.6f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.6f, h * p*3.4f, -p*00.2f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*00.3f, h * p*1.4f, -p*00.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*00.3f, h * p*1.4f, -p*00.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.2f, h * p*3.4f, -p*00.6f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.2f, h * p*3.4f, -p*00.6f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*00.3f, h * p*1.4f, -p*00.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*00.9f, h * p*1.4f, -p*00.3f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.6f, h * p*3.4f, -p*00.2f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*00.2f, h * p*3.4f, -p*00.6f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*00.9f, h * p*1.4f, -p*00.3f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*00.9f, h * p*1.4f, p*00.3f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.6f, h * p*3.4f, p*00.2f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*00.6f, h * p*3.4f, -p*00.2f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*00.9f, h * p*1.4f, p*00.3f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*00.3f, h * p*1.4f, p*00.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.2f, h * p*3.4f, p*00.6f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*00.6f, h * p*3.4f, p*00.2f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*00.3f, h * p*1.4f, p*00.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*00.3f, h * p*1.4f, p*00.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.2f, h * p*3.4f, p*00.6f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.2f, h * p*3.4f, p*00.6f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*00.3f, h * p*1.4f, p*00.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*00.9f, h * p*1.4f, p*00.3f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*00.6f, h * p*3.4f, p*00.2f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.2f, h * p*3.4f, p*00.6f);
	glEnd();
	/////---------��ҳ�ϲ�-----------------------
	glBindTexture(GL_TEXTURE_2D, m_texture[6]); //������ҳ8��0.6M��
	glBegin(GL_QUADS);									//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.09f, h * p*4.0f, p*0.03f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.09f, h * p*4.0f, -p*0.03f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*00.6f, h * p*3.4f, -p*00.2f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.6f, h *  p*3.4f, p*00.2f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.09f, h * p*4.0f, -p*0.03f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.03f, h *  p*4.0f, -p*0.09f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*00.2f, h * p*3.4f, -p*00.6f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.6f, h * p*3.4f, -p*00.2f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.03f, h * p*4.0f, -p*0.09f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.03f, h * p*4.0f, -p*0.09f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.2f, h * p*3.4f, -p*00.6f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.2f, h * p*3.4f, -p*00.6f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.03f, h *  p*4.0f, -p*0.09f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.09f, h *  p*4.0f, -p*0.03f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.6f, h *  p*3.4f, -p*00.2f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*00.2f, h *  p*3.4f, -p*00.6f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.09f, h *  p*4.0f, -p*0.03f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.09f, h * p*4.0f, p*0.03f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.6f, h *  p*3.4f, p*00.2f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*00.6f, h *  p*3.4f, -p*00.2f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.09f, h *  p*4.0f, p*0.03f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.03f, h *  p*4.0f, p*0.09f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.2f, h *  p*3.4f, p*00.6f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*00.6f, h *  p*3.4f, p*00.2f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.03f, h *  p*4.0f, p*0.09f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.03f, h *  p*4.0f, p*0.09f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.2f, h *  p*3.4f, p*00.6f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.2f, h *  p*3.4f, p*00.6f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.03f, h * p*4.0f, p*0.09f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.09f, h *  p*4.0f, p*0.03f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*00.6f, h *  p*3.4f, p*00.2f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.2f, h *  p*3.4f, p*00.6f);
	glEnd();
	glPopMatrix();
}



void CGround::tree2(float x, float z, float h, float h_add, float size)
{
	float p = size;
	glPushMatrix();
	glTranslatef(x, h_add, z);
	glBindTexture(GL_TEXTURE_2D, m_texture[5]); //��������8��1M��
	glBegin(GL_QUADS);									//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.09f, h * p*0.0f, p*0.03f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.09f, h * p*0.0f, -p*0.03f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*0.09f, h * p*1.0f, -p*0.03f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*0.09f, h * p*1.0f, p*0.03f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.09f, h * p*0.0f, -p*0.03f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.03f, h * p*0.0f, -p*0.09f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*0.03f, h * p*1.0f, -p*0.09f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*0.09f, h * p*1.0f, -p*0.03f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.03f, h * p*0.0f, -p*0.09f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.03f, h *  p*0.0f, -p*0.09f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*0.03f, h *  p*1.0f, -p*0.09f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*0.03f, h * p*1.0f, -p*0.09f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.03f, h *  p*0.0f, -p*0.09f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.09f, h *  p*0.0f, -p*0.03f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*0.09f, h *  p*1.0f, -p*0.03f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*0.03f, h *  p*1.0f, -p*0.09f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.09f, h *  p*0.0f, -p*0.03f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.09f, h * p*0.0f, p*0.03f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*0.09f, h * p*1.0f, p*0.03f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*0.09f, h * p*1.0f, -p*0.03f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.09f, h *  p*0.0f, p*0.03f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.03f, h *  p*0.0f, p*0.09f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*0.03f, h *  p*1.0f, p*0.09f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*0.09f, h *  p*1.0f, p*0.03f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.03f, h *  p*0.0f, p*0.09f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.03f, h *  p*0.0f, p*0.09f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*0.03f, h *  p*1.0f, p*0.09f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*0.03f, h *  p*1.0f, p*0.09f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.03f, h *  p*0.0f, p*0.09f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.09f, h *  p*0.0f, p*0.03f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*0.09f, h *  p*1.0f, p*0.03f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*0.03f, h *  p*1.0f, p*0.09f);
	glEnd();
	/////---------��ҳ�²�-----------------------
	glBindTexture(GL_TEXTURE_2D, m_texture[7]); //������ҳ8��0.4M��
	glBegin(GL_QUADS);									//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.36f, h *  p*1.4f, p* 0.12f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.36f, h * p*1.4f, -p* 0.12f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p* 0.9f, h *  p*0.5f, -p* 0.3f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p* 0.9f, h *  p*0.5f, p* 0.3f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.36f, h * p*1.4f, -p*0.12f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.12f, h * p*1.4f, -p*0.36f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*00.3f, h * p*0.4f, -p*00.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.9f, h * p*0.4f, -p*00.3f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.12f, h * p*1.4f, -p*0.36f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.12f, h *  p*1.4f, -p*0.36f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.3f, h *  p*0.5f, -p*00.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.3f, h * p*0.5f, -p*00.9f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.12f, h *  p*1.4f, -p*0.36f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.36f, h *  p*1.4f, -p*0.12f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.9f, h *  p*0.4f, -p*00.3f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*00.3f, h *  p*0.4f, -p*00.9f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.36f, h *  p*1.4f, -p*0.12f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.36f, h *  p*1.4f, p*0.12f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.9f, h *  p*0.5f, p*00.3f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*00.9f, h *  p*0.5f, -p*00.3f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.36f, h *  p*1.4f, p*0.12f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.12f, h *  p*1.4f, p*0.36f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.3f, h *  p*0.4f, p*00.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*00.9f, h *  p*0.4f, p*00.3f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.12f, h * p*1.4f, p*0.36f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.12f, h *  p*1.4f, p*0.36f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.3f, h *  p*0.5f, p*00.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.3f, h *  p*0.5f, p*00.9f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.12f, h * p*1.4f, p*0.36f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.36f, h *  p*1.4f, p*0.12f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*00.9f, h *  p*0.4f, p*00.3f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.3f, h *  p*0.4f, p*00.9f);
	glEnd();
	/////---------��ҳ�в�-----------------------
	glBindTexture(GL_TEXTURE_2D, m_texture[7]); //������ҳ8��1.0M��
	glBegin(GL_QUADS);									//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.36f, h *  p*2.4f, p* 0.12f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.36f, h * p*2.4f, -p* 0.12f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p* 0.9f, h *  p*1.2f, -p* 0.3f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p* 0.9f, h *  p*1.2f, p* 0.3f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.36f, h *  p*2.4f, -p*0.12f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.12f, h *  p*2.4f, -p*0.36f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*00.3f, h *  p*1.3f, -p*00.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.9f, h *  p*1.3f, -p*00.3f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.12f, h *  p*2.4f, -p*0.36f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.12f, h *  p*2.4f, -p*0.36f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.3f, h *  p*1.2f, -p*00.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.3f, h *  p*1.2f, -p*00.9f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.12f, h *  p*2.4f, -p*0.36f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.36f, h * p*2.4f, -p*0.12f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.9f, h *  p*1.3f, -p*00.3f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*00.3f, h *  p*1.3f, -p*00.9f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.36f, h * p*2.4f, -p*0.12f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.36f, h * p*2.4f, p*0.12f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.9f, h *  p*1.2f, p*00.3f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*00.9f, h *  p*1.2f, -p*00.3f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.36f, h *  p*2.4f, p*0.12f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.12f, h *  p*2.4f, p*0.36f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.3f, h * p*1.3f, p*00.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*00.9f, h *  p*1.3f, p*00.3f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.12f, h * p*2.4f, p*0.36f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.12f, h * p*2.4f, p*0.36f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.3f, h * p*1.2f, p*00.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.3f, h *  p*1.2f, p*00.9f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.12f, h *  p*2.4f, p*0.36f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.36f, h *  p*2.4f, p*0.12f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*00.9f, h *  p*1.3f, p*00.3f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.3f, h *  p*1.3f, p*00.9f);
	glEnd();
	/////---------��ҳ�ϲ�-----------------------
	glBindTexture(GL_TEXTURE_2D, m_texture[7]); //������ҳ8��0.6M��
	glBegin(GL_QUADS);									//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.09f, h * p*3.3f, p*0.03f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.09f, h * p*3.3f, -p*0.03f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*00.6f, h *  p*2.4f, -p*00.2f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.6f, h *  p*2.4f, p*00.2f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.09f, h *  p*3.3f, -p*0.03f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.03f, h *  p*3.3f, -p*0.09f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*00.2f, h *  p*2.3f, -p*00.6f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.6f, h *  p*2.3f, -p*00.2f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.03f, h * p*3.3f, -p*0.09f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.03f, h *  p*3.3f, -p*0.09f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.2f, h *  p*2.4f, -p*00.6f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.2f, h * p*2.4f, -p*00.6f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.03f, h *  p*3.3f, -p*0.09f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.09f, h *  p*3.3f, -p*0.03f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.6f, h *  p*2.3f, -p*00.2f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*00.2f, h *  p*2.3f, -p*00.6f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.09f, h *  p*3.3f, -p*0.03f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.09f, h *  p*3.3f, p*0.03f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.6f, h *  p*2.4f, p*00.2f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*00.6f, h *  p*2.4f, -p*00.2f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.09f, h * p*3.3f, p*0.03f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.03f, h * p*3.3f, p*0.09f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.2f, h * p*2.3f, p*00.6f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*00.6f, h * p*2.3f, p*00.2f);
	//��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.03f, h * p*3.3f, p*0.09f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-p*0.03f, h *  p*3.3f, p*0.09f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-p*00.2f, h *  p*2.4f, p*00.6f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.2f, h *  p*2.4f, p*00.6f);
	//��-��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*0.03f, h * p*3.3f, p*0.09f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.09f, h * p*3.3f, p*0.03f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*00.6f, h * p*2.3f, p*00.2f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*00.2f, h * p*2.3f, p*00.6f);
	glEnd();
	glPopMatrix();
}


void CGround::streetlight()
{
	float p = 1;

	if (open_light)
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, m_texture[8]);	//·�ƿ���
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.1f, -p*0.1f, p*0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.1f, -p*0.1f, p*0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(p*0.1f, p*0.1f, p*0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*0.1f, p*0.1f, p*0.0f);
		glEnd();
	}
	else
	{
		glColor3f(0.8f, 0.8f, 0.8f);
		glBindTexture(GL_TEXTURE_2D, m_texture[2]);	//·�ƿ���
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-p*0.1f, -p*0.1f, p*0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(p*0.1f, -p*0.1f, p*0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(p*0.1f, p*0.1f, p*0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-p*0.1f, p*0.1f, p*0.0f);
		glEnd();
	}
	glRotatef(-90, 1.0, 0.0, 0.0);//��ס+ 
	glColor3f(0.5f, 0.5f, 1.0f);
	gluCylinder(gluNewQuadric(), p*0.1, p*0.1, p*0.8, 12, 1);	//����Բ��
	glTranslatef(p*0.0f, p*0.0f, p*0.8f);			//0.8z
	gluCylinder(gluNewQuadric(), p*0.1, p*0.05, p*0.2, 12, 1);	//����Բ��

	glColor3f(0.3f, 1.0f, 1.0f);
	gluCylinder(gluNewQuadric(), p*0.05, p*0.05, p*4.2, 12, 1);	//����Բ��
	glTranslatef(p*0.0f, p*0.0f, p*4.2f);			//4.2z
	gluCylinder(gluNewQuadric(), p*0.05, p*0.1, p*0.2, 12, 1);	//����Բ��
	glTranslatef(p*0.0f, p*0.0f, p*0.2f);			//0.2z
	gluCylinder(gluNewQuadric(), p*0.1, p*0.0, p*0.6, 12, 1);	//����Բ��
	glTranslatef(p*0.0f, p*0.0f, -p*0.3f);		//-0.3z
	glColor3f(0.0f, 1.0f, 1.0f);
	gluDisk(gluNewQuadric(), p*0.45, p*0.55, 12, 12);			//����Բ��

	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(p*0.0f, p*0.5f, p*0.0f);			//0.5y
	gluSphere(gluNewQuadric(), p*0.15, 12, 12);	//��������
	glTranslatef(p*0.0f, -p*1.0f, p*0.0f);		//-1.0y
	gluSphere(gluNewQuadric(), p*0.15, 12, 12);	//��������
	glTranslatef(p*0.5f, p*0.5f, p*0.0f);			//0.5x/0.5y
	gluSphere(gluNewQuadric(), p*0.15, 12, 12);	//��������
	glTranslatef(-p*1.0f, p*0.0f, p*0.0f);		//-1.0x
	gluSphere(gluNewQuadric(), p*0.15, 12, 12);	//��������
	glTranslatef(p*0.5f, p*0.0f, p*0.0f);			//0.5x

	glRotatef(90, 1.0, 0.0, 0.0);   //��ס-
	glRotatef(45, 0.0, 1.0, 0.0);  //��4��+
	glTranslatef(p*0.5f, p*0.0f, p*0.0f);			//0.5x
	gluSphere(gluNewQuadric(), p*0.15, 12, 12);	//��������
	glTranslatef(-p*1.0f, p*0.0f, p*0.0f);		//-1.0x
	gluSphere(gluNewQuadric(), p*0.15, 12, 12);	//��������
	glTranslatef(p*0.5f, p*0.0f, p*0.5f);			//0.5x/0.5z
	gluSphere(gluNewQuadric(), p*0.15, 12, 12);	//��������
	glTranslatef(p*0.0f, p*0.0f, -p*1.0f);		//-1.0z
	gluSphere(gluNewQuadric(), p*0.15, 12, 12);	//��������
}

void CGround::football()
{
	glPushMatrix();
	glTranslatef(1080.0f, 0, -855.f); 
	glBindTexture(GL_TEXTURE_2D, m_texture[3]); //�������򳡵��� ��
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0f, 0.1f, -35.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0f, 0.1f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(25.0f, 0.1f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(25.0f, 0.1f, -35.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, m_texture[4]); //�������򳡵��� ��
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 25.0f, 0.1f, -35.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 25.0f, 0.1f,  0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(45.0f, 0.1f,  0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(45.0f, 0.1f, -35.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, m_texture[3]); //�������򳡵��� ��
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(65.0f, 0.1f,  0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(65.0f, 0.1f, -35.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(45.0f, 0.1f, -35.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(45.0f, 0.1f,  0.0f);
	glEnd();
	glPopMatrix();
}

void CGround::basketball()
{	/////////���������������򳡡�����������������������///////////
	float p=1;
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, m_texture[10]); //��������ذ�
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p* 0.0f, p*0.1f, p* 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p* 0.0f, p*0.1f, p*30.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*15.0f, p*0.1f, p*30.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*15.0f, p*0.1f, p* 0.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, m_texture[12]); //���������
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*7.3f, p*0.1f, p*31.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*7.3f, p*0.1f, p*31.4f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*7.3f, p*3.1f, p*31.4f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*7.3f, p*3.1f, p*31.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*7.7f, p*0.1f, p*31.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*7.7f, p*0.1f, p*31.4f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*7.7f, p*3.1f, p*31.4f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*7.7f, p*3.1f, p*31.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*7.3f, p*3.5f, p*29.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*7.3f, p*2.5f, p*32.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*7.7f, p*2.5f, p*32.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*7.7f, p*3.5f, p*29.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*7.3f, p*3.7f, p*29.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*7.3f, p*2.7f, p*32.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*7.7f, p*2.7f, p*32.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*7.7f, p*3.7f, p*29.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, m_texture[12]); //���������
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*7.3f, p*0.1f, -p*1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*7.3f, p*0.1f, -p*1.4f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*7.3f, p*3.1f, -p*1.4f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*7.3f, p*3.1f, -p*1.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*7.7f, p*0.1f, -p*1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*7.7f, p*0.1f, -p*1.4f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*7.7f, p*3.1f, -p*1.4f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*7.7f, p*3.1f, -p*1.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*7.3f, p*3.5f, p*1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*7.3f, p*2.5f, -p*2.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*7.7f, p*2.5f, -p*2.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*7.7f, p*3.5f, p*1.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*7.3f, p*3.7f, p*1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*7.3f, p*2.7f, -p*2.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*7.7f, p*2.7f, -p*2.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*7.7f, p*3.7f, p*1.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, m_texture[11]); //���������
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*8.4f, p*3.0f, p*29.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*6.6f, p*3.0f, p*29.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*6.6f, p*4.0f, p*29.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*8.4f, p*4.0f, p*29.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(p*6.6f, p*3.0f, p*1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(p*8.4f, p*3.0f, p*1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(p*8.4f, p*4.0f, p*1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(p*6.6f, p*4.0f, p*1.0f);
	glEnd();
	glPopMatrix();
}
