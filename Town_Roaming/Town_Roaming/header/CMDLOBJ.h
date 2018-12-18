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

	float gman_z;//公文包男人的位置
	bool  gman_turn;//公文包男人的方向
	float holo_z;//黄色女人的位置
	bool  holo_turn;//黄色女人的方向
	float player_z;//黄色男人的位置
	bool  player_turn;//黄色男人的方向
	float scientist_z;//科学家的位置
	bool  scientist_turn;//科学家的方向
	float apache_x;//直升机的位置
	bool  apache_turn;//直升机的方向
	float osprey_x;//直升机2的位置
	bool  osprey_turn;//直升机2的方向

	float z_15,x_17, z_18, z_19, x_21,z_22;//位置
	bool  turn_15, turn_17,turn_18, turn_19, turn_21,turn_22;//方向

	float car_z1,car_z2,car_z3,car_z4,car_z5,car_z6,
		  car_z7,car_z8,car_z9,car_z10,car_z11,car_z12;//车的位置
	float car1_move,car2_move,car3_move,car4_move,car5_move,car6_move,
		  car7_move,car8_move,car9_move,car10_move,car11_move,car12_move;//车是否移动

	Camera  *m_Camera ;	//摄像机类
	 
	int		InitGL(int a,char* na);  //初始化单个MDL模型

	bool	init(Camera *); //初始化多个模型，并加入摄像机的指针

	bool	man_render(int  m_turn,float m_x, float m_z, float m_degree, int m_action);//主角的坐标x,z,角度，以及哪个动作
	bool	render();
	void	move(int num_car);
	float   get_car_pos(int num_car);//获取第几辆车的位置

	void	DrawModels(float x,float y,float z,int a,float size, float rz);
	 //绘制指定模型 入参 坐标 模型数组下标 缩放倍数 水平角 仰角
	void	action(int a,int b); //动作
};

