// *************************************************************
// MDLģ�͵�Ӧ�ã���ʼ������Ⱦ�����ģ�͵ķ���
// *************************************************************


#include "..\header\stdafx.h"
#include "..\header\CMDLOBJ.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

//////////////////////////////////////////////////////////////////////////
CMDLOBJ::CMDLOBJ()
{
	Type = 0 ;
}
CMDLOBJ::~CMDLOBJ()
{
}
/////////////////////////////////////////////////////////////////////
//��ʼ��

int CMDLOBJ::InitGL(int a,char* na)	
{	
	MdlModel[a].Init(na);
	MdlModel[a].SetSequence(0);//���õ�ǰ����
	Animation[a]=0;
	Anim_Fin[a] = true;
	return TRUE;							// Initialization Went OK
}



//����ģ��
void CMDLOBJ::DrawModels(float x,float y,float z,int a,float size,float rz )
{
	glPushMatrix();
//	glEnable(GL_LIGHT0); 
	//glEnable(GL_LIGHTING);            
	glEnable(GL_TEXTURE_2D);     
	//�ƶ���ָ������
	glTranslatef(x, y, z);
	glScalef(size,size,size);	//����
	//��ת
	glRotated(-90,1.0f,0.0f,0.0f);
	//y
//	glRotated(-ry * 2 / 3 - 5, 0.0f, 1.0f, 0.0f);
	//z
	glRotated(rz+90,0.0f,0.0f,1.0f);//rz��man_degree

	//ˢ�µ�ǰ�������е�֡
	MdlModel[a].AdvanceFrame2();

	//����
	MdlModel[a].DrawModel();///////////////

	if(Animation[a]==1)
	{
		Animation[a]=0;
		MdlModel[a].SetSequence(0);
	}
	if(Animation[a]>0) 
		Animation[a]--;

	glPopMatrix();
}



void CMDLOBJ::action(int a,int b) //��һ��������ʾ�ڼ���ģ�ͣ��ڶ�����ʾת���ĸ�������
{
	MdlModel[a].SetSequence(b);
	tagMDLSeqDescription *aa=(tagMDLSeqDescription *)((BYTE *)MdlModel[a].Header
		+ MdlModel[a].Header->SequenceOffset)+b;

	//���������� Animation : 5 , NumFrames:29
	Animation[a]=aa->NumFrames;
}

bool CMDLOBJ::init(Camera  *m_Camera)
{
	this->m_Camera = m_Camera;
	//���������ʼ��
	InitGL(1, "data/mdl/barney.mdl");//����Ѳ��
	InitGL(2,"data/mdl/gman.mdl");//���İ�����
	InitGL(3,"data/mdl/holo.mdl");//��ɫŮ��
	InitGL(4,"data/mdl/player.mdl");//��ɫ����	
	InitGL(5,"data/mdl/scientist.mdl");	//��ѧ��
	InitGL(6,"data/mdl/apache.mdl");	//ֱ����
	InitGL(7,"data/mdl/osprey.mdl");	//ֱ����2
	InitGL(8,"data/mdl/forklift.mdl");	//�泵
	InitGL(9,"data/mdl/sat_globe.mdl");	//����ģ��
//	InitGL(10,"data/mdl/hassassin.mdl");	//Ů�̿�
//	InitGL(11,"data/mdl/hassault.mdl");	//�ػ�ǹ��
	InitGL(12,"data/mdl/hgrunt.mdl");	//������
	InitGL(13,"data/mdl/loader.mdl");	//������
//	InitGL(14,"data/mdl/arctic.mdl");	//�����Բ���
	InitGL(15,"data/mdl/gign.mdl");	//���ֱ�����
	InitGL(16,"data/mdl/gsg9.mdl");	//�����ֱ�����
	InitGL(17,"data/mdl/guerilla.mdl");	//��ͷ��
	InitGL(18,"data/mdl/leet.mdl");	//�۾�
	InitGL(19,"data/mdl/sas.mdl");	//ͷ��
//	InitGL(20,"data/mdl/terror.mdl");	//���ֲֿ�����
	InitGL(21,"data/mdl/urban.mdl");	//������
	InitGL(22,"data/mdl/vip.mdl");	//vip

	gman_z = -870.0f;	gman_turn = 1;
	holo_z = -1000.0f;	holo_turn = 0;
	player_z = -1000.0f;	player_turn = 1; 
	scientist_z = -960.0f;	scientist_turn = 1;
	apache_x = 900.0f;	apache_turn = 1;
	osprey_x = 1100.0f;	osprey_turn = 0;
	z_15 = -1300;	turn_15 = 1;	x_17 = 800;		turn_17 = 1;
	z_18 = -900;	turn_18 = 1;	z_19 = -1100;	turn_19 = 0;
	x_21 = 850;		turn_21 = 0;	z_22 = -1000;	turn_22 = 0;

	car_z1 = -880.0f;	car_z2 = -1100.0f;	car_z3 = -1000.0f;	car_z4 = -1180.0f;
	car_z5 = -920.0f;	car_z6 = -1020.0f;	car_z7 = -1080.0f;	car_z8 = -860.0f;
	car_z9 = -1160.0f;	car_z10= -960.0f;	car_z11= -1130.0f;	car_z12= -1060.0f;

	Type = 1 ;
	return TRUE ;
}

void  CMDLOBJ::move(int num_car)
{
	//���İ�����
	if (gman_z>-800.0 && gman_turn == 1  )	gman_turn = 0;
	else if (gman_z < -1300.0 && gman_turn == 0 )	gman_turn = 1;
	//��ɫŮ��
	if (holo_z>-800.0 && holo_turn == 1  )	holo_turn = 0;	
	else if (holo_z < -1300.0 && holo_turn == 0)		holo_turn = 1;	
	//��ɫ����
	if (player_z>-800.0 && player_turn == 1)	player_turn = 0;	
	else if (player_z < -1300.0 && player_turn == 0)		player_turn = 1;	
	//��ѧ��
	if (scientist_z>-800.0 && scientist_turn == 1) 	scientist_turn = 0; 
	else if (scientist_z < -1300.0 && scientist_turn == 0) 	scientist_turn = 1;
	//ֱ����
	if (apache_x>1300 && apache_turn == 1) apache_turn = 0; 
	else if (apache_x < 700 && apache_turn == 0) 		apache_turn = 1; 
	//ֱ����2
	if (osprey_x>1300 && osprey_turn == 1) 	osprey_turn = 0; 
	else if (osprey_x < 700 && osprey_turn == 0) 	osprey_turn = 1;
	//�泵
	car1_move = car2_move = car3_move = car4_move = car5_move = car6_move = 1;
	car7_move = car8_move = car9_move = car10_move = car11_move = car12_move = 1;

	if (num_car == 1)		car1_move = 0;	else if (num_car == 2)	car2_move = 0;
	else if (num_car == 3)	car3_move = 0;	else if (num_car == 4)	car4_move = 0;
	else if (num_car == 5)	car5_move = 0;	else if (num_car == 6)	car6_move = 0;
	else if (num_car == 7)	car7_move = 0;	else if (num_car == 8)	car8_move = 0;
	else if (num_car == 9)	car9_move = 0;	else if (num_car == 10)	car10_move = 0;
	else if (num_car == 11)	car11_move = 0;	else if (num_car == 12)	car12_move = 0;

	if (z_15 >-1160.0 && turn_15 == 1) 	turn_15 = 0;
	else if (z_15 < -1400.0 && turn_15 == 0) 	turn_15 = 1;
	if (x_17 >925.0 && turn_17 == 1) 	turn_17 = 0;
	else if (x_17 < 750.0 && turn_17 == 0) 	turn_17 = 1;
	if (z_18 >-800.0 && turn_18 == 1) 	turn_18 = 0;
	else if (z_18 < -1135.0 && turn_18 == 0) 	turn_18 = 1;
	if (z_19 >-1160.0 && turn_19 == 1) 	turn_19 = 0;
	else if (z_19 < -1400.0 && turn_19 == 0) 	turn_19 = 1;
	if (x_21 >925.0 && turn_21 == 1) 	turn_21 = 0;
	else if (x_21 < 750.0 && turn_21 == 0) 	turn_21 = 1;
	if (z_22 >-800.0 && turn_22 == 1) 	turn_22 = 0;
	else if (z_22 < -1135.0 && turn_22 == 0) 	turn_22 = 1;
}

float CMDLOBJ::get_car_pos(int num_car)//��ȡ�ڼ�������λ��
{
	if (num_car == 1)	return car_z1;	if (num_car == 2)	return car_z2;	if (num_car == 3)	return car_z3;	
	if (num_car == 4)	return car_z4;	if (num_car == 5)	return car_z5;	if (num_car == 6)	return car_z6;
	if (num_car == 7)	return car_z7;	if (num_car == 8)	return car_z8;	if (num_car == 9)	return car_z9;	
	if (num_car == 10)	return car_z10;	if (num_car == 11)	return car_z11;	if (num_car == 12)	return car_z12;
}

bool CMDLOBJ::render()
{  
	//���İ�����
	if (gman_turn == 1)	{		gman_z += 0.2f;		DrawModels(1012, 0.0, gman_z, 2, 0.035f, 180); }
	else				{		gman_z -= 0.2f;		DrawModels(1012, 0.0, gman_z, 2, 0.035f, 0);   	}
	if (Animation[2] == 0)		action(2, 6);//��·
	//��ɫŮ��
	if (holo_turn == 1)	{		holo_z += 0.2f;		DrawModels(1048, 1.2, holo_z, 3, 0.03f, 180);   }
	else{						holo_z -= 0.2f;		DrawModels(1048, 1.2, holo_z, 3, 0.03f, 0);   	}
	if (Animation[3] == 0)		action(3, 0);//��·
	//��ɫ����
	if (player_turn == 1)	{	player_z += 0.2f;	DrawModels(989, 1.2, player_z, 4, 0.03f, 180);   }
	else{						player_z -= 0.2f;	DrawModels(989, 1.2, player_z, 4, 0.03f, 0);   	}
	if (Animation[4] == 0)		action(4, 4);//��·
	//��ѧ��
	if (scientist_turn == 1) {	scientist_z += 0.2f;DrawModels(1072, 0.0, scientist_z, 5, 0.035f, 180); 
													DrawModels(1072, 0.0, scientist_z - 10, 13, 0.010f, 180);	}//�����˸��ſ�ѧ��
	else {						scientist_z -= 0.2f;DrawModels(1072, 0.0, scientist_z, 5, 0.035f, 0);
													DrawModels(1072, 0.0, scientist_z - 10, 13, 0.010f, 0);	}//�����˸��ſ�ѧ��
	if (Animation[5] == 0)		action(5, 0);	if (Animation[13] == 0)		action(13, 1);//������
	//ֱ����
	if (apache_turn == 1) {		apache_x += 0.6f;	DrawModels(apache_x, 40.0, -990, 6, 0.020f, 270); }
	else {						apache_x -= 0.6f;	DrawModels(apache_x, 40.0, -990, 6, 0.020f, 90); }
	//ֱ����2
	if (osprey_turn == 1) {		osprey_x += 0.6f;	DrawModels(osprey_x, 40.0, -1150, 7, 0.020f, 270); }
	else {						osprey_x -= 0.6f;	DrawModels(osprey_x, 40.0, -1150, 7, 0.020f, 90); }


	//�泵
	if (car1_move == 1) { car_z1 += 0.6f;	if (car_z1 >= -600)			car_z1 = -1400; }
	if (car2_move == 1) { car_z2 += 0.6f;	if (car_z2 >= -600)			car_z2 = -1400;  }
	if (car3_move == 1) { car_z3 -= 0.6f;	if (car_z3 <= -1400)		car_z3 = -600;	}
	if (car4_move == 1) { car_z4 -= 0.6f;	if (car_z4 <= -1400)		car_z4 = -600; }
	if (car5_move == 1) { car_z5 += 0.6f;	if (car_z5 >= -600)			car_z5 = -1400; }
	if (car6_move == 1) { car_z6 += 0.6f;	if (car_z6 >= -600)			car_z6 = -1400;}
	if (car7_move == 1) { car_z7 -= 0.6f;	if (car_z7 <= -1400)		car_z7 = -600;	}
	if (car8_move == 1) { car_z8 -= 0.6f;	if (car_z8 <= -1400)		car_z8 = -600; }
	if (car9_move == 1) { car_z9 += 0.6f;	if (car_z9 >= -600)			car_z9 = -1400; }
	if (car10_move == 1) { car_z10 += 0.6f;	if (car_z10 >= -600)		car_z10 = -1400; }
	if (car11_move == 1) { car_z11 -= 0.6f;	if (car_z11 <= -1400)		car_z11 = -600;	}
	if (car12_move == 1) { car_z12 -= 0.6f;	if (car_z12 <= -1400)		car_z12 = -600; }

	DrawModels(933, 0.2, car_z1, 8, 0.035f, 180);		DrawModels(937, 0.2, car_z2, 8, 0.035f, 180);
	DrawModels(943, 0.2, car_z3, 8, 0.035f, 0);			DrawModels(947, 0.2, car_z4, 8, 0.035f, 0);		
	DrawModels(993, 0.2, car_z5, 8, 0.035f, 180);		DrawModels(997, 0.2, car_z6, 8, 0.035f, 180);
	DrawModels(1003, 0.2, car_z7, 8, 0.035f, 0);		DrawModels(1007, 0.2, car_z8, 8, 0.035f, 0);	
	DrawModels(1053, 0.2, car_z9, 8, 0.035f, 180);		DrawModels(1057, 0.2, car_z10, 8, 0.035f, 180);	
	DrawModels(1063, 0.2, car_z11, 8, 0.035f, 0);		DrawModels(1067, 0.2, car_z12, 8, 0.035f, 0);
	if (Animation[8] == 0)		action(8, 1);//����
	
/*	DrawModels(940, 0.0, -1000, 10, 0.032f, 0); if (Animation[10] == 0)	action(10, 1);
	DrawModels(940, 0.0, -1005, 11, 0.032f, 0); if (Animation[11] == 0)	action(11, 1);	
	DrawModels(940, 1.3, -1020, 14, 0.036f, 0); if (Animation[14] == 0)	action(14, 4);
	DrawModels(940, 1.3, -1050, 20, 0.036f, 0); if (Animation[20] == 0)	action(20, 4);*/

	DrawModels(890, 3.0, -910, 9, 0.05f, 0);//����ģ��
	DrawModels(912, 0.0, -1010, 12, 0.032f, 220); if (Animation[12] == 0)	action(12, 11);//������	
	DrawModels(920, 1.3, -1020, 16, 0.036f, 240); if (Animation[16] == 0)	action(16, 1);//������
	//�Ƶ����Ѳ��
	if (turn_15 == 1) { z_15 += 0.2f;	DrawModels(952, 1.3, z_15, 15, 0.036f, 180); }
	else { z_15 -= 0.2f;		DrawModels(952, 1.3, z_15, 15, 0.036f, 0); }
	if (Animation[15] == 0)	action(15, 4);
	//�������ֵ�Ѳ��
	if (turn_17 == 1) { x_17 += 0.2f;	DrawModels(x_17, 1.3, -1002, 17, 0.036f, 270); }
	else { x_17 -= 0.2f;		DrawModels(x_17, 1.3, -1002, 17, 0.036f, 90); }
	if (Animation[17] == 0)	action(17, 4);	
	//���ж���Ѳ��
	if (turn_18 == 1) { z_18 += 0.2f;	DrawModels(952, 1.3, z_18, 18, 0.036f, 180); }
	else { z_18 -= 0.2f;		DrawModels(952, 1.3, z_18, 18, 0.036f, 0); }
	if (Animation[18] == 0)	action(18, 4);
	//�Ƶ�ֵ�Ѳ��
	if (turn_19 == 1) { z_19 += 0.2f;	DrawModels(928, 1.3, z_19, 19, 0.036f, 180); }
	else { z_19 -= 0.2f;		DrawModels(928, 1.3, z_19, 19, 0.036f, 0); }
	if (Animation[19] == 0)	action(19, 4);
	//��԰�ֵ�Ѳ��
	if (turn_21 == 1) { x_21 += 0.2f;	DrawModels(x_21, 1.3, -978, 21, 0.036f, 270); }
	else { x_21 -= 0.2f;		DrawModels(x_21, 1.3, -978, 21, 0.036f, 90); }
	if (Animation[21] == 0)	action(21, 4);	
	//�����ҽֵ�Ѳ��
	if (turn_22 == 1) { z_22 += 0.2f;	DrawModels(928, 1.3, z_22, 22, 0.036f, 180); }
	else { z_22 -= 0.2f;		DrawModels(928, 1.3, z_22, 22, 0.036f, 0); }	
	if (Animation[22] == 0)	action(22, 4);

	return true ;
	
}


bool CMDLOBJ::man_render(int  m_turn, float m_x, float m_z, float m_degree, int m_action)
{
	Type = m_action;
		if (m_turn == 0)
			DrawModels(m_x, 0.0, m_z - 10.0f, 1, 0.03f, m_degree);//ǰ
		else if (m_turn == 1)
			DrawModels(m_x - 10.0f, 0.0, m_z, 1, 0.03f, m_degree);//��
		else if (m_turn == 2)
			DrawModels(m_x, 0.0, m_z + 10.0f, 1, 0.03f, m_degree);//��
		else if (m_turn == 3)
			DrawModels(m_x + 10.0f, 0.0, m_z, 1, 0.03f, m_degree);//��
	
		if (m_action == 0)	{action(1, 65);		Animation[1] = 0;	}//����
	else if (m_action == 1) { if (Animation[1] == 0)action(1, 4); }//��			
	else if (m_action == 2)	{ if (Animation[1] == 0)action(1, 5);	}//��
	return true;

}