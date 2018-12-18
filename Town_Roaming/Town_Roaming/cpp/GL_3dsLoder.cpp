// *************************************************************
// 先要梳理代码，尽量把cpp功能注释写清楚，自行理解
// 跟3DS文件相关的众多函数，耐心看吧..懂用法就行，不用深入
// 多百度，多看CSDN博客
// *************************************************************


#define GLUT_DISABLE_ATEXIT_HACK

#include "..\header\stdafx.h"
#include  <cstring>
#include "..\header\GL_3dsLoder.h"
#include "..\header\C3DSModel.h"
#include <atlstr.h>


void CLoad3DS::CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID)//*****************************************************
{
	AUX_RGBImageRec *pBitmap = NULL;
	
	if(!strFileName)									// 如果无此文件，则直接返回
		return;

	pBitmap = auxDIBImageLoad(strFileName);				// 装入位图，并保存数据
	
	if(pBitmap == NULL)									// 如果装入位图失败，则退出
		exit(0);

	// 生成纹理
	glGenTextures(1, &textureArray[textureID]);

	// 设置像素对齐格式
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	if (pBitmap)										// 释放位图占用的资源
	{
		if (pBitmap->data)						
		{
			free(pBitmap->data);				
		}

		free(pBitmap);					
	}
} 

void CLoad3DS::Load_Texture(UINT textureArray[], LPSTR strFileName, int textureID)
{
	if (!strFileName)  return;

	tImage *pImage = NULL;
	// Load TGA Image
	if (strstr(strFileName, ".tga"))
	{
		pImage = LoadTGA(strFileName);
	}
	// Load BMP Image
	else if (strstr(strFileName, ".bmp"))
	{
		pImage = LoadBMP(strFileName);
	}
	else	return ;

	if (pImage == NULL)	return ;

	glGenTextures(1, &textureArray[textureID]);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

	int textureType = GL_RGB;

	if (pImage->channels == 4)
		textureType = GL_RGBA;

	gluBuild2DMipmaps(GL_TEXTURE_2D, pImage->channels, pImage->sizeX,
		pImage->sizeY, textureType, GL_UNSIGNED_BYTE, pImage->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	if (pImage)
	{
		if (pImage->data)	free(pImage->data);
		free(pImage);
	}

}

bool CLoad3DS::init(t3DModel *g_3DModel,char *filename)
{	
	CLoad3DS g_Load3ds;	
	g_3DModel->showList = glGenLists(1);
	glNewList(g_3DModel->showList,GL_COMPILE);
	glEnable(GL_TEXTURE_2D);     
	g_Load3ds.Import3DS(g_3DModel,filename );	// 将3ds文件装入到模型结构体中
//	printf("numOfMaterials=%d ",g_3DModel->numOfMaterials);
	// 遍历所有的材质
	for(long k = 0; k < g_3DModel->numOfMaterials; k++)
	{	//printf("strFile=data\\%s ",g_3DModel->vctMaterials[k].strFile);
	// 判断是否是一个文件名
	//	if(strlen(g_3DModel.vctMaterials[i].strFile) > 0)
	{
		//  使用纹理文件名称来装入位图
		g_Load3ds.CreateTexture(g_3DModel->g_Texture,g_3DModel->vctMaterials[k].strFile ,k);
//		g_Load3ds.Load_Texture(g_3DModel->g_Texture,g_3DModel->vctMaterials[k].strFile ,k);			
		
	}

	// 设置材质的纹理ID
	g_3DModel->vctMaterials[k].texureId = k;
	}
	
	

	for(int i = 0; i < g_3DModel->numOfObjects; i++)
	{
		// 如果对象的大小小于0，则退出
		if(g_3DModel->vctObject.size() <= 0) break;

		// 获得当前显示的对象
		t3DObject *pObject = &g_3DModel->vctObject[i];

		// 判断该对象是否有纹理映射
		if(pObject->bHasTexture) {

			// 打开纹理映射
			glEnable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);//我在想可不可以在这里设置颜色的值***************************************
			glBindTexture(GL_TEXTURE_2D,g_3DModel->g_Texture[pObject->materialID]);
		} else {

			// 关闭纹理映射
			glDisable(GL_TEXTURE_2D);
			//glColor3ub(255, 255, 255);
		}
		// 开始以g_ViewMode模式绘制
		glBegin(GL_TRIANGLES);					
		// 遍历所有的面
		for(int j = 0; j < pObject->numOfFaces; j++)
		{
			// 遍历三角形的所有点
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				// 获得面对每个点的索引
				int index = pObject->pFaces[j].vertIndex[whichVertex];

				// 给出法向量
				glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);

				// 如果对象具有纹理
				if(pObject->bHasTexture) {

					// 确定是否有UVW纹理坐标
					if(pObject->pTexVerts) {
						glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
					}
				} else {

					if(g_3DModel->vctMaterials.size() && pObject->materialID >= 0) 
					{
						BYTE *pColor = g_3DModel->vctMaterials[pObject->materialID].color;
						glColor3ub(pColor[0], pColor[1], pColor[2]);
						//	printf("1=%uc,2=%uc,3=%uc",pColor[0],pColor[1],pColor[2]);
					}
				}
				glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
			}
		}

		glEnd();								// 绘制结束

	}
	glEndList();
	return true;
}
GLuint CLoad3DS::Show3DSModel(t3DModel *g_3DModel)
{	
	// 遍历模型中所有的对象
	g_3DModel->showList = glGenLists(1);

	
	return g_3DModel->showList ;
}
bool CLoad3DS::Import3DS(t3DModel *pModel, char *strFileName)//********************************************************************
{
	char strMessage[255] = {0};
	tChunk currentChunk={0};
	int i=0;
	strFileName = StringCat(MODELSPATH,strFileName);
	// 打开一个3ds文件
	m_FilePointer = fopen(strFileName, "rb");

	// 确保所获得的文件指针合法
	if(!m_FilePointer) 
	{
		sprintf(strMessage, "Unable to find the file: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}

	// 当文件打开之后，首先应该将文件最开始的数据块读出以判断是否是一个3ds文件
	// 如果是3ds文件的话，第一个块ID应该是PRIMARY

	// 将文件的第一块读出并判断是否是3ds文件
	ReadChunk(&currentChunk);//读出块的id和块的size

	// 确保是3ds文件
	if (currentChunk.ID != PRIMARY)//打头的都是基本块，这个是区别
	{
		sprintf(strMessage, "Unable to load PRIMARY chuck from file: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}

	// 现在开始读入数据，ProcessNextChunk()是一个递归函数

	// 通过调用下面的递归函数，将对象读出
	ProcessNextChunk(pModel, &currentChunk);

	// 在读完整个3ds文件之后，计算顶点的法线
	ComputeNormals(pModel);

	// 释放内存空间
	CleanUp();

	return true;
}


//  下面的函数释放所有的内存空间，并关闭文件
void CLoad3DS::CleanUp()//**************************************************************************
{
if(m_FilePointer)
{	fclose(m_FilePointer);						// 关闭当前的文件指针
	m_FilePointer=NULL;						
}					
}



//  下面的函数读出3ds文件的主要部分
void CLoad3DS::ProcessNextChunk(t3DModel *pModel, tChunk *pPreviousChunk)//***************************************************************
{
	t3DObject newObject = {0};					// 用来添加到对象链表
	tMaterialInfo newTexture = {0};				// 用来添加到材质链表
	tChunk currentChunk={0};                    // 用来添加到当前块链表
	tChunk tempChunk={0};                       // 用来添加到临时块链表
//	unsigned int version = 0;					// 保存文件版本
	int gBuffer[50000] = {0};					// 用来跳过不需要的数据

//	m_CurrentChunk = new tChunk;				// 为新的块分配空间		

	//  下面每读一个新块，都要判断一下块的ID，如果该块是需要的读入的，则继续进行
	//  如果是不需要读入的块，则略过

	// 继续读入子块，直到达到预定的长度
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// 读入下一个块
		ReadChunk(&currentChunk);

		// 判断块的ID号
		switch (currentChunk.ID)
		{
		case VERSION:							// 文件版本号
			
			// 在该块中有一个无符号短整型数保存了文件的版本
			
			// 读入文件的版本号，并将字节数添加到bytesRead变量中
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);

			// 如果文件版本号大于3，给出一个警告信息
			if ((currentChunk.length - currentChunk.bytesRead==4)&&(gBuffer[0]> 0x03))
				MessageBox(NULL, "This 3DS file is over version 3 so it may load incorrectly", "Warning", MB_OK);
			break;

		case OBJECTINFO:						// 网格版本信息
			
			// 读入下一个块
			ReadChunk(&tempChunk);

			// 获得网格的版本号
			tempChunk.bytesRead += fread(gBuffer, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer);

			// 增加读入的字节数
			currentChunk.bytesRead += tempChunk.bytesRead;

			// 进入下一个块
			ProcessNextChunk(pModel, &currentChunk);
			break;

		case MATERIAL:							// 材质信息

			// 材质的数目递增
			pModel->numOfMaterials++;

			// 在纹理链表中添加一个空白纹理结构
			pModel->vctMaterials.push_back(newTexture);

			// 进入材质装入函数
			ProcessNextMaterialChunk(pModel, &currentChunk);
			break;

		case OBJECT:							// 对象的名称
				
			// 该块是对象信息块的头部，保存了对象了名称

			// 对象数递增
			pModel->numOfObjects++;
		
			// 添加一个新的tObject节点到对象链表中
			pModel->vctObject.push_back(newObject);
			
			// 初始化对象和它的所有数据成员
			memset(&(pModel->vctObject[pModel->numOfObjects - 1]), 0, sizeof(t3DObject));

			// 获得并保存对象的名称，然后增加读入的字节数
			currentChunk.bytesRead += GetString(pModel->vctObject[pModel->numOfObjects - 1].strName);
			
			// 进入其余的对象信息的读入
			ProcessNextObjectChunk(pModel, &(pModel->vctObject[pModel->numOfObjects - 1]), &currentChunk);
			break;

		case EDITKEYFRAME:

			// 跳过关键帧块的读入，增加需要读入的字节数
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;

		default: 
			
			//  跳过所有忽略的块的内容的读入，增加需要读入的字节数
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		}

		// 增加从最后块读入的字节数
		pPreviousChunk->bytesRead += currentChunk.bytesRead;
	}

	// 释放当前块的内存空间
//	delete m_CurrentChunk;
//	m_CurrentChunk = pPreviousChunk;
}


//  下面的函数处理所有的文件中对象的信息
void CLoad3DS::ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)//********************************************
{
	int gBuffer[50000] = {0};					// 用于读入不需要的数据
	tChunk currentChunk={0};
	// 对新的块分配存储空间
//	m_CurrentChunk = new tChunk;
	vector<tMatREF*> vctMatIDS;

	// 继续读入块的内容直至本子块结束
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// 读入下一个块
		ReadChunk(&currentChunk);

		// 区别读入是哪种块
		switch (currentChunk.ID)
		{
		case OBJECT_MESH:					// 正读入的是一个新块
		
			// 使用递归函数调用，处理该新块
			ProcessNextObjectChunk(pModel, pObject, &currentChunk);
			break;

		case OBJECT_VERTICES:				// 读入是对象顶点
			ReadVertices(pObject, &currentChunk);
			break;

		case OBJECT_FACES:					// 读入的是对象的面
			ReadVertexIndices(pObject, &currentChunk);
			break;

		case OBJECT_MATERIAL:				// 读入的是对象的材质名称
			
			// 该块保存了对象材质的名称，可能是一个颜色，也可能是一个纹理映射。同时在该块中也保存了
			// 纹理对象所赋予的面

			// 下面读入对象的材质名称
			ReadObjectMaterial(pModel, pObject, &currentChunk,&vctMatIDS);			
			break;

		case OBJECT_UV:						// 读入对象的UV纹理坐标

			// 读入对象的UV纹理坐标
			ReadUVCoordinates(pObject, &currentChunk);
			break;

		default:  

			// 略过不需要读入的块
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		}

		// 添加从最后块中读入的字节数到前面的读入的字节中
		pPreviousChunk->bytesRead += currentChunk.bytesRead;
	}
//88888888888888888888888888888888888888888888888
/*	if(pPreviousChunk->ID!=OBJECT_MESH)return;
	int size=vctMatIDS.size();
	if(size)
	{
		pModel->numOfMaterials=size;
		pObject->pMaterialREFS=new tMatREF[size];
        for(int i=0;i<size;i++)
		{pObject->pMaterialREFS[i]=*(vctMatIDS[i]);}
		vctMatIDS.clear();
	}*/
	// 释放当前块的内存空间，并把当前块设置为前面块
//	delete m_CurrentChunk;
//	m_CurrentChunk = pPreviousChunk;
}

//  下面的函数处理所有的材质信息
void CLoad3DS::ProcessNextMaterialChunk(t3DModel *pModel, tChunk *pPreviousChunk) //*****************************************************
{
	int gBuffer[50000] = {0};					// 用于读入不需要的数据
	char *temp = NULL;
	// 给当前块分配存储空间
//	m_CurrentChunk = new tChunk;
	tChunk currentChunk={0};
	// 继续读入这些块，直到该子块结束
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// 读入下一块
		ReadChunk(&currentChunk);

		// 判断读入的是什么块
		switch (currentChunk.ID)
		{
		case MATNAME:							// 材质的名称
			
			// 读入材质的名称
			currentChunk.bytesRead += fread(pModel->vctMaterials[pModel->numOfMaterials - 1].strName, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
  //8888888888888888888888888888888888888888888888888888888888888
		case MAT_AMBIENT:						
			ReadColorChunk(&(pModel->vctMaterials[pModel->numOfMaterials - 1]), &currentChunk,MAT_AMBIENT);
			break;
case MAT_SPECULAR:						
			ReadColorChunk(&(pModel->vctMaterials[pModel->numOfMaterials - 1]), &currentChunk,MAT_SPECULAR);
			break;
case MAT_EMISSIVE:						
			ReadColorChunk(&(pModel->vctMaterials[pModel->numOfMaterials - 1]), &currentChunk,MAT_EMISSIVE);
			break;


		case MATDIFFUSE:						// 对象的R G B颜色
			ReadColorChunk(&(pModel->vctMaterials[pModel->numOfMaterials - 1]), &currentChunk,MATDIFFUSE);
			break;
		
		case MATMAP:							// 纹理信息的头部
			
			// 进入下一个材质块信息
			ProcessNextMaterialChunk(pModel, &currentChunk);
			break;

		case MATMAPFILE:						// 材质文件的名称
			
			//g_Buffer = g_TexturesPath+g_TexturesName;
			//temp = StringCat(temp,g_TexturesPath);
			// 读入材质的文件名
			currentChunk.bytesRead += fread(pModel->vctMaterials[pModel->numOfMaterials - 1].strFile, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			//pModel->vctMaterials[pModel->numOfMaterials - 1].strFile = StringCat(TEXTURESPATH,pModel->vctMaterials[pModel->numOfMaterials - 1].strFile);
			temp = StringCat(TEXTURESPATH,pModel->vctMaterials[pModel->numOfMaterials - 1].strFile);
			strcpy(pModel->vctMaterials[pModel->numOfMaterials - 1].strFile,temp);
			break;
		
		default:  

			// 掠过不需要读入的块
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		}

		// 添加从最后块中读入的字节数
		pPreviousChunk->bytesRead += currentChunk.bytesRead;
	}

	// 删除当前块，并将当前块设置为前面的块
	//delete m_CurrentChunk;
	//m_CurrentChunk = pPreviousChunk;
}

//  下面函数读入块的ID号和它的字节长度
void CLoad3DS::ReadChunk(tChunk *pChunk)                       //****************************************************
{
	// 读入块的ID号，占用了2个字节。块的ID号象OBJECT或MATERIAL一样，说明了在块中所包含的内容
	pChunk->bytesRead = fread(&pChunk->ID, 1, 2, m_FilePointer);

	// 然后读入块占用的长度，包含了四个字节
	pChunk->bytesRead += fread(&pChunk->length, 1, 4, m_FilePointer);
}

//  下面的函数读入一个字符串
int CLoad3DS::GetString(char *pBuffer)//**************************************************************
{
	int index = 0;

	// 读入一个字节的数据
	fread(pBuffer, 1, 1, m_FilePointer);

	// 直到结束
	while (*(pBuffer + index++) != 0) {

		// 读入一个字符直到NULL
		fread(pBuffer + index, 1, 1, m_FilePointer);
	}

	// 返回字符串的长度
	return strlen(pBuffer) + 1;
}


//  下面的函数读入RGB颜色
void CLoad3DS::ReadColorChunk(tMaterialInfo *pMaterial, tChunk *pChunk,USHORT typeFlag) //*********************************************
{
	// 读入颜色块信息
	

	tChunk tempChunk={0};
 BYTE btmp[3];  
 ReadChunk(&tempChunk);// 读入颜色块信息
 switch(typeFlag) 
 { 
 case MAT_AMBIENT:      //材质的环境光颜色 
	 tempChunk.bytesRead += fread(btmp, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer); 
	 Bytes2Floats(btmp,pMaterial->ambient,3,1.0f/256.0f); 
	 pMaterial->ambient[3]=1.0f;
	 break; 
 case MAT_SPECULAR:      //材质的镜面光颜色 
	 tempChunk.bytesRead += fread(btmp, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer); 
	 Bytes2Floats(btmp,pMaterial->specular,3,1.0f/256.0f); 
	 pMaterial->specular[3]=1.0f;
	 break;
	  case MAT_EMISSIVE:      //材质的出射光颜色 
	 tempChunk.bytesRead += fread(btmp, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer); 
	 memset(btmp,0,3);
	 Bytes2Floats(btmp,pMaterial->emissive,3,1.0f/256.0f); 
	 pMaterial->emissive[3]=1.0f;
	 break;
case MATDIFFUSE:      //材质的漫反射光颜色 
default:
	 tempChunk.bytesRead += fread(pMaterial->color, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer);
	 Bytes2Floats(pMaterial->color,pMaterial->diffuse,3,1.0f/256.0f); 
	 pMaterial->diffuse[3]=1.0f;
	 break;
 }
	

	// 增加读入的字节数
	pChunk->bytesRead += tempChunk.bytesRead;
}

//  下面的函数读入顶点索引
void CLoad3DS::ReadVertexIndices(t3DObject *pObject, tChunk *pPreviousChunk)//**************************************************
{
	unsigned short index = 0;					// 用于读入当前面的索引

	// 读入该对象中面的数目
	pPreviousChunk->bytesRead += fread(&pObject->numOfFaces, 1, 2, m_FilePointer);

	// 分配所有面的存储空间，并初始化结构
	pObject->pFaces = new tFace [pObject->numOfFaces];
	memset(pObject->pFaces, 0, sizeof(tFace) * pObject->numOfFaces);

	// 遍历对象中所有的面
	for(int i = 0; i < pObject->numOfFaces; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			// 读入当前面的第一个点 
			pPreviousChunk->bytesRead += fread(&index, 1, sizeof(index), m_FilePointer);

			if(j < 3)
			{
				// 将索引保存在面的结构中
				pObject->pFaces[i].vertIndex[j] = index;
			}
		}
	}
}


//  下面的函数读入对象的UV坐标
void CLoad3DS::ReadUVCoordinates(t3DObject *pObject, tChunk *pPreviousChunk)//**************************************************
{
	// 为了读入对象的UV坐标，首先需要读入UV坐标的数量，然后才读入具体的数据

	// 读入UV坐标的数量
	pPreviousChunk->bytesRead += fread(&pObject->numTexVertex, 1, 2, m_FilePointer);

	// 分配保存UV坐标的内存空间
	pObject->pTexVerts = new CVector2 [pObject->numTexVertex];

	// 读入纹理坐标
	pPreviousChunk->bytesRead += fread(pObject->pTexVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}

//  读入对象的顶点
void CLoad3DS::ReadVertices(t3DObject *pObject, tChunk *pPreviousChunk)//******************************************
{
	// 在读入实际的顶点之前，首先必须确定需要读入多少个顶点。
	
	// 读入顶点的数目
	pPreviousChunk->bytesRead += fread(&(pObject->numOfVerts), 1, 2, m_FilePointer);

	// 分配顶点的存储空间，然后初始化结构体
	pObject->pVerts = new CVector3 [pObject->numOfVerts];
	memset(pObject->pVerts, 0, sizeof(CVector3) * pObject->numOfVerts);

	// 读入顶点序列
	pPreviousChunk->bytesRead += fread(pObject->pVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);

//8888888888888888888888888888888888888888888888888888888888888888888888
	pObject->m_minX=999999;pObject->m_maxX=-999999;
	pObject->m_minY=999999;pObject->m_maxX=-999999;
	pObject->m_minZ=999999;pObject->m_maxZ=-999999;
	
	
	
	
	// 现在已经读入了所有的顶点。
	// 因为3D Studio Max的模型的Z轴是指向上的，因此需要将y轴和z轴翻转过来。
	// 具体的做法是将Y轴和Z轴交换，然后将Z轴反向。

	// 遍历所有的顶点
	for(int i = 0; i < pObject->numOfVerts; i++)
	{
		// 保存Y轴的值
		float fTempY = pObject->pVerts[i].y;

		// 设置Y轴的值等于Z轴的值
		pObject->pVerts[i].y = pObject->pVerts[i].z;

		// 设置Z轴的值等于-Y轴的值 
		pObject->pVerts[i].z = -fTempY;
         if(pObject->m_minX>pObject->pVerts[i].x)  pObject->m_minX=pObject->pVerts[i].x;
         if(pObject->m_maxX<pObject->pVerts[i].x)  pObject->m_maxX=pObject->pVerts[i].x;
		 if(pObject->m_minY>pObject->pVerts[i].y)  pObject->m_minY=pObject->pVerts[i].y;
		 if(pObject->m_maxY<pObject->pVerts[i].y)  pObject->m_maxY=pObject->pVerts[i].y;
		 if(pObject->m_minZ>pObject->pVerts[i].z)  pObject->m_minZ=pObject->pVerts[i].z;
		 if(pObject->m_maxZ<pObject->pVerts[i].z)  pObject->m_maxZ=pObject->pVerts[i].z;
	
/*pObject->m_minX=pObject->m_minX/100;pObject->m_maxX=pObject->m_maxX/100;
pObject->m_minY=pObject->m_minY/100;pObject->m_maxY=pObject->m_maxY/100;
pObject->m_minZ=pObject->m_minZ/100;pObject->m_maxZ=pObject->m_maxZ/100;
*/
pObject->pVerts[i].x=(pObject->pVerts[i].x)/20;
pObject->pVerts[i].y=(pObject->pVerts[i].y)/20;
pObject->pVerts[i].z=(pObject->pVerts[i].z)/20;}

}


//  下面的函数读入对象的材质名称
void CLoad3DS::ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk, vector<tMatREF*>*pvmatids)//***************************************************
{
	char strMaterial[255] = {0};			// 用来保存对象的材质名称
	int gBuffer[50000] = {0};				// 用来读入不需要的数据
    bool bmaterror=true;
	tMatREF *pMatref;
    pMatref=new tMatREF;
	// 材质或者是颜色，或者是对象的纹理，也可能保存了象明亮度、发光度等信息。

	// 下面读入赋予当前对象的材质名称
	pPreviousChunk->bytesRead += GetString(strMaterial);

	// 遍历所有的纹理
	for(int i = 0; i < pModel->numOfMaterials; i++)
	{
		//如果读入的纹理与当前的纹理名称匹配
		if(strcmp(strMaterial, pModel->vctMaterials[i].strName) == 0)
		{
			// 设置材质ID
			pObject->materialID = i;
			pMatref->nMaterialID=i;

			// 判断是否是纹理映射，如果strFile是一个长度大于1的字符串，则是纹理
			if(strlen(pModel->vctMaterials[i].strFile) > 0) {

				// 设置对象的纹理映射标志
				pObject->bHasTexture = true;
                pMatref->bHasTexture = true;
			}	
			bmaterror=false;
			break;
		}
		else
		{
			// 如果该对象没有材质，则设置ID为-1
			pObject->materialID = -1;
			pMatref->nMaterialID=-1;
			bmaterror=true;
		}
	}

	pPreviousChunk->bytesRead += fread(gBuffer, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
     if(!bmaterror)
	 {pMatref->nFaceNum=gBuffer[0]&0x0000FFFF;
	 pMatref->pFaceIndexs=new USHORT[pMatref->nFaceNum];
	 memcpy(pMatref->pFaceIndexs,2+(BYTE*)gBuffer,pMatref->nFaceNum*sizeof(USHORT));
	 pvmatids->push_back(pMatref);
	 
}



}			



//  下面的这些函数主要用来计算顶点的法向量，顶点的法向量主要用来计算光照
// 下面的宏定义计算一个矢量的长度
#define Mag(Normal) (sqrt(Normal.x*Normal.x + Normal.y*Normal.y + Normal.z*Normal.z))

// 下面的函数求两点决定的矢量
CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2)//*******************************************************************************
{
	CVector3 vVector;							

	vVector.x = vPoint1.x - vPoint2.x;			
	vVector.y = vPoint1.y - vPoint2.y;			
	vVector.z = vPoint1.z - vPoint2.z;			

	return vVector;								
}

// 下面的函数两个矢量相加
CVector3 AddVector(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vResult;							
	
	vResult.x = vVector2.x + vVector1.x;		
	vResult.y = vVector2.y + vVector1.y;		
	vResult.z = vVector2.z + vVector1.z;		

	return vResult;								
}

// 下面的函数处理矢量的缩放
CVector3 DivideVectorByScaler(CVector3 vVector1, float Scaler)
{
	CVector3 vResult;							
	
	vResult.x = vVector1.x / Scaler;			
	vResult.y = vVector1.y / Scaler;			
	vResult.z = vVector1.z / Scaler;			

	return vResult;								
}

// 下面的函数返回两个矢量的叉积
CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vCross;								
												
	vCross.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
												
	vCross.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
												
	vCross.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	return vCross;								
}

// 下面的函数规范化矢量
CVector3 Normalize(CVector3 vNormal)
{
	double Magnitude;							

	Magnitude = Mag(vNormal);					// 获得矢量的长度

	vNormal.x /= (float)Magnitude;				
	vNormal.y /= (float)Magnitude;				
	vNormal.z /= (float)Magnitude;				

	return vNormal;								
}

//  下面的函数用于计算对象的法向量
void CLoad3DS::ComputeNormals(t3DModel *pModel)//************************************************************************
{
	CVector3 vVector1, vVector2, vNormal, vPoly[3];
	int i,j;
	// 如果模型中没有对象，则返回
	if(pModel->numOfObjects <= 0)
		return;

	// 遍历模型中所有的对象
	for(int index = 0; index < pModel->numOfObjects; index++)
	{
		// 获得当前的对象
		t3DObject *pObject = &(pModel->vctObject[index]);

		// 分配需要的存储空间
		CVector3 *pNormals		= new CVector3 [pObject->numOfFaces];
		CVector3 *pTempNormals	= new CVector3 [pObject->numOfFaces];
		pObject->pNormals		= new CVector3 [pObject->numOfVerts];

		// 遍历对象的所有面
		for( i=0; i < pObject->numOfFaces; i++)
		{												
			vPoly[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
			vPoly[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
			vPoly[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];

			// 计算面的法向量

			vVector1 = Vector(vPoly[0], vPoly[2]);		// 获得多边形的矢量
			vVector2 = Vector(vPoly[2], vPoly[1]);		// 获得多边形的第二个矢量

			vNormal  = Cross(vVector1, vVector2);		// 获得两个矢量的叉积
			pTempNormals[i] = vNormal;					// 保存非规范化法向量
			vNormal  = Normalize(vNormal);				// 规范化获得的叉积

			pNormals[i] = vNormal;						// 将法向量添加到法向量列表中
		}

		//  下面求顶点法向量
		CVector3 vSum = {0.0, 0.0, 0.0};
		CVector3 vZero = vSum;
		int shared=0;
		// 遍历所有的顶点
		for ( i = 0; i < pObject->numOfVerts; i++)			
		{
			for ( j = 0; j < pObject->numOfFaces; j++)	// 遍历所有的三角形面
			{												// 判断该点是否与其它的面共享
				if (pObject->pFaces[j].vertIndex[0] == i || 
					pObject->pFaces[j].vertIndex[1] == i || 
					pObject->pFaces[j].vertIndex[2] == i)
				{
					vSum = AddVector(vSum, pTempNormals[j]);
					shared++;								
				}
			}      
			
			pObject->pNormals[i] = DivideVectorByScaler(vSum, float(-shared));

			// 规范化最后的顶点法向
			pObject->pNormals[i] = Normalize(pObject->pNormals[i]);	

			vSum = vZero;								
			shared = 0;										
		}
	
		// 释放存储空间，开始下一个对象
	//	delete [] pTempNormals;
	//	delete [] pNormals;
	}
}


void CLoad3DS::Bytes2Floats(BYTE* pbs,float* pfs,int num,float fsk)
{if(num==0||num>100) return;
 for(int i=0;i<num;i++)
 {
    pfs[i]=float(pbs[i])*fsk;
 }
}

char * CLoad3DS::StringCat(char *s1,char *s2){
	int i=strlen(s1);//StringLength(s1);
	int j=strlen(s2);//StringLength(s2);
	char *pc=(char *)malloc(i+j+1);
	char *pc1;
	pc1=pc;
	while((*pc++=*s1++)!='\0');
	pc--;
	while((*pc++=*s2++)!='\0');
	return pc1;
}




tImage * CLoad3DS::LoadBMP(const char *strFileName)
{
	AUX_RGBImageRec *pBitmap = NULL;
	FILE *pFile = NULL;

	if ((pFile = fopen(strFileName, "rb")) == NULL)
	{
		MessageBox(NULL, "Can't load BMP image!", "ERROR", MB_OK | MB_ICONINFORMATION);
		return NULL;
	}

	pBitmap = auxDIBImageLoad(strFileName);

	tImage *pImage = (tImage *)malloc(sizeof(tImage));

	pImage->channels = 3;
	pImage->sizeX = pBitmap->sizeX;
	pImage->sizeY = pBitmap->sizeY;
	pImage->data = pBitmap->data;

	free(pBitmap);

	return pImage;
}


tImage * CLoad3DS::LoadTGA(const char *strFileName)
{
	tImage *pImageData = NULL;
	WORD width = 0, height = 0;
	byte length = 0;
	byte imageType = 0;
	byte bits = 0;
	FILE *pFile = NULL;
	int channels = 0;
	int stride = 0;
	int i = 0;

	if ((pFile = fopen(strFileName, "rb")) == NULL)
	{
		MessageBox(NULL, "Can't load TGA image!", "ERROR", MB_OK | MB_ICONINFORMATION);
		return NULL;
	}

	pImageData = (tImage*)malloc(sizeof(tImage));

	fread(&length, sizeof(byte), 1, pFile);

	fseek(pFile, 1, SEEK_CUR);

	fread(&imageType, sizeof(byte), 1, pFile);

	fseek(pFile, 9, SEEK_CUR);

	fread(&width, sizeof(WORD), 1, pFile);
	fread(&height, sizeof(WORD), 1, pFile);
	fread(&bits, sizeof(byte), 1, pFile);

	fseek(pFile, length + 1, SEEK_CUR);

	if (imageType != TGA_RLE)
	{
		if (bits == 24 || bits == 32)
		{
			channels = bits / 8;
			stride = channels * width;
			pImageData->data = ((unsigned char*)malloc(sizeof(unsigned char)*stride*height));

			for (int y = 0; y < height; y++)
			{
				unsigned char *pLine = &(pImageData->data[stride * y]);

				fread(pLine, stride, 1, pFile);

				for (i = 0; i < stride; i += channels)
				{
					int temp = pLine[i];
					pLine[i] = pLine[i + 2];
					pLine[i + 2] = temp;
				}
			}
		}
		else if (bits == 16)
		{
			unsigned short pixels = 0;
			int r = 0, g = 0, b = 0;

			channels = 3;
			stride = channels * width;
			pImageData->data = ((unsigned char*)malloc(sizeof(unsigned char)*stride*height));

			for (int i = 0; i < width*height; i++)
			{
				fread(&pixels, sizeof(unsigned short), 1, pFile);

				b = (pixels & 0x1f) << 3;
				g = ((pixels >> 5) & 0x1f) << 3;
				r = ((pixels >> 10) & 0x1f) << 3;

				pImageData->data[i * 3 + 0] = r;
				pImageData->data[i * 3 + 1] = g;
				pImageData->data[i * 3 + 2] = b;
			}
		}
		else
			return NULL;
	}
	else
	{
		byte rleID = 0;
		int colorsRead = 0;
		channels = bits / 8;
		stride = channels * width;

		pImageData->data = ((unsigned char*)malloc(sizeof(unsigned char)*stride*height));
		byte *pColors = ((byte*)malloc(sizeof(byte)*channels));

		while (i < width*height)
		{
			fread(&rleID, sizeof(byte), 1, pFile);

			if (rleID < 128)
			{
				rleID++;

				while (rleID)
				{
					fread(pColors, sizeof(byte) * channels, 1, pFile);

					pImageData->data[colorsRead + 0] = pColors[2];
					pImageData->data[colorsRead + 1] = pColors[1];
					pImageData->data[colorsRead + 2] = pColors[0];

					if (bits == 32)
						pImageData->data[colorsRead + 3] = pColors[3];

					i++;
					rleID--;
					colorsRead += channels;
				}
			}
			else
			{
				rleID -= 127;

				fread(pColors, sizeof(byte) * channels, 1, pFile);

				while (rleID)
				{
					pImageData->data[colorsRead + 0] = pColors[2];
					pImageData->data[colorsRead + 1] = pColors[1];
					pImageData->data[colorsRead + 2] = pColors[0];

					if (bits == 32)
						pImageData->data[colorsRead + 3] = pColors[3];

					i++;
					rleID--;
					colorsRead += channels;
				}
			}
		}
	}

	fclose(pFile);

	pImageData->channels = channels;
	pImageData->sizeX = width;
	pImageData->sizeY = height;

	return pImageData;
}
