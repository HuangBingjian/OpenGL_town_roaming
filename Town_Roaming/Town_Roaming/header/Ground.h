#ifndef __GROUND_H__
#define __GROUND_H__

#include "stdafx.h"
#include "CBMPLoader.h"
#include "Vector.h"
#include "Camera.h"

#define GL_CLAMP_TO_EDGE	0x812F
#define NUM_TEXTURE	123	//纹理数量
#define width_map 800		//地图内宽度
#define length_map 800		//地图内长度

/** 地面类 */
class CGround
{
public:
	/** 构造函数 */
	CGround();
	~CGround();

	/** 初始化 */
	bool init();


	int	open_light;		//路灯开关

	void setlight();
	void openlight();
	void closelight();
	GLuint		m_texture[NUM_TEXTURE];   /**< 地面纹理   */


	/** 渲染 */
	void render();
	/** 建筑渲染 */
	void building_render();
	void building(int tex1, int tex2, int tex3, int tex4, int tex5, float x, float z, float w, float l, float h, float h_add);
	/** 树木渲染 */
	void tree_render();
	void tree1(float x, float z, float h, float h_add, float size);
	void tree2(float x, float z, float h, float h_add, float size);
	/** 路灯渲染 */
	void streetlight_render();
	void streetlight();
	/** 运动场渲染 */
	void playground_render();
	void football();
	void basketball();

private:
	AUX_RGBImageRec *TextureImage[NUM_TEXTURE];						//为纹理创建存储空间
	float       length;         /**< 长度 */
	float       width;          /**< 宽度 */
//	float       height;         /**< 高度 */
	float		height_base;	//微增高度
	float		height_tree;	//树木高度

};


#endif ///__GROUND_H__