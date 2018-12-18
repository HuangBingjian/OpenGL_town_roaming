//#include "CBullet.h";
#include "Camera.h"
#include "GL_Base.h"
extern bool	   keys[256];	

class CMDLOBJ  
{
public:
	CMDLOBJ();
	virtual ~CMDLOBJ();
	unsigned int Animation[10];
	TMDLModel	MdlModel[30];
	int		Type;
	bool	Anim_Fin[10];

	float gman_z;//���İ����˵�λ��
	bool  gman_turn;//���İ����˵ķ���
	float holo_z;//��ɫŮ�˵�λ��
	bool  holo_turn;//��ɫŮ�˵ķ���
	float player_z;//��ɫ���˵�λ��
	bool  player_turn;//��ɫ���˵ķ���
	float scientist_z;//��ѧ�ҵ�λ��
	bool  scientist_turn;//��ѧ�ҵķ���
	float apache_x;//ֱ������λ��
	bool  apache_turn;//ֱ�����ķ���
	float osprey_x;//ֱ����2��λ��
	bool  osprey_turn;//ֱ����2�ķ���

	float z_15,x_17, z_18, z_19, x_21,z_22;//λ��
	bool  turn_15, turn_17,turn_18, turn_19, turn_21,turn_22;//����

	float car_z1,car_z2,car_z3,car_z4,car_z5,car_z6,
		  car_z7,car_z8,car_z9,car_z10,car_z11,car_z12;//����λ��
	float car1_move,car2_move,car3_move,car4_move,car5_move,car6_move,
		  car7_move,car8_move,car9_move,car10_move,car11_move,car12_move;//���Ƿ��ƶ�

	Camera  *m_Camera ;	//�������
	 
	int		InitGL(int a,char* na);  //��ʼ������MDLģ��

	bool	init(Camera *); //��ʼ�����ģ�ͣ��������������ָ��

	bool	man_render(int  m_turn,float m_x, float m_z, float m_degree, int m_action);//���ǵ�����x,z,�Ƕȣ��Լ��ĸ�����
	bool	render();
	void	move(int num_car);
	float   get_car_pos(int num_car);//��ȡ�ڼ�������λ��

	void	DrawModels(float x,float y,float z,int a,float size, float rz);
	 //����ָ��ģ�� ��� ���� ģ�������±� ���ű��� ˮƽ�� ����
	void	action(int a,int b); //����
};

