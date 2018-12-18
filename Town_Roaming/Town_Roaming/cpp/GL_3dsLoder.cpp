// *************************************************************
// ��Ҫ������룬������cpp����ע��д������������
// ��3DS�ļ���ص��ڶຯ�������Ŀ���..���÷����У���������
// ��ٶȣ��࿴CSDN����
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
	
	if(!strFileName)									// ����޴��ļ�����ֱ�ӷ���
		return;

	pBitmap = auxDIBImageLoad(strFileName);				// װ��λͼ������������
	
	if(pBitmap == NULL)									// ���װ��λͼʧ�ܣ����˳�
		exit(0);

	// ��������
	glGenTextures(1, &textureArray[textureID]);

	// �������ض����ʽ
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	if (pBitmap)										// �ͷ�λͼռ�õ���Դ
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
	g_Load3ds.Import3DS(g_3DModel,filename );	// ��3ds�ļ�װ�뵽ģ�ͽṹ����
//	printf("numOfMaterials=%d ",g_3DModel->numOfMaterials);
	// �������еĲ���
	for(long k = 0; k < g_3DModel->numOfMaterials; k++)
	{	//printf("strFile=data\\%s ",g_3DModel->vctMaterials[k].strFile);
	// �ж��Ƿ���һ���ļ���
	//	if(strlen(g_3DModel.vctMaterials[i].strFile) > 0)
	{
		//  ʹ�������ļ�������װ��λͼ
		g_Load3ds.CreateTexture(g_3DModel->g_Texture,g_3DModel->vctMaterials[k].strFile ,k);
//		g_Load3ds.Load_Texture(g_3DModel->g_Texture,g_3DModel->vctMaterials[k].strFile ,k);			
		
	}

	// ���ò��ʵ�����ID
	g_3DModel->vctMaterials[k].texureId = k;
	}
	
	

	for(int i = 0; i < g_3DModel->numOfObjects; i++)
	{
		// �������Ĵ�СС��0�����˳�
		if(g_3DModel->vctObject.size() <= 0) break;

		// ��õ�ǰ��ʾ�Ķ���
		t3DObject *pObject = &g_3DModel->vctObject[i];

		// �жϸö����Ƿ�������ӳ��
		if(pObject->bHasTexture) {

			// ������ӳ��
			glEnable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);//������ɲ�����������������ɫ��ֵ***************************************
			glBindTexture(GL_TEXTURE_2D,g_3DModel->g_Texture[pObject->materialID]);
		} else {

			// �ر�����ӳ��
			glDisable(GL_TEXTURE_2D);
			//glColor3ub(255, 255, 255);
		}
		// ��ʼ��g_ViewModeģʽ����
		glBegin(GL_TRIANGLES);					
		// �������е���
		for(int j = 0; j < pObject->numOfFaces; j++)
		{
			// ���������ε����е�
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				// ������ÿ���������
				int index = pObject->pFaces[j].vertIndex[whichVertex];

				// ����������
				glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);

				// ��������������
				if(pObject->bHasTexture) {

					// ȷ���Ƿ���UVW��������
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

		glEnd();								// ���ƽ���

	}
	glEndList();
	return true;
}
GLuint CLoad3DS::Show3DSModel(t3DModel *g_3DModel)
{	
	// ����ģ�������еĶ���
	g_3DModel->showList = glGenLists(1);

	
	return g_3DModel->showList ;
}
bool CLoad3DS::Import3DS(t3DModel *pModel, char *strFileName)//********************************************************************
{
	char strMessage[255] = {0};
	tChunk currentChunk={0};
	int i=0;
	strFileName = StringCat(MODELSPATH,strFileName);
	// ��һ��3ds�ļ�
	m_FilePointer = fopen(strFileName, "rb");

	// ȷ������õ��ļ�ָ��Ϸ�
	if(!m_FilePointer) 
	{
		sprintf(strMessage, "Unable to find the file: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}

	// ���ļ���֮������Ӧ�ý��ļ��ʼ�����ݿ�������ж��Ƿ���һ��3ds�ļ�
	// �����3ds�ļ��Ļ�����һ����IDӦ����PRIMARY

	// ���ļ��ĵ�һ��������ж��Ƿ���3ds�ļ�
	ReadChunk(&currentChunk);//�������id�Ϳ��size

	// ȷ����3ds�ļ�
	if (currentChunk.ID != PRIMARY)//��ͷ�Ķ��ǻ����飬���������
	{
		sprintf(strMessage, "Unable to load PRIMARY chuck from file: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}

	// ���ڿ�ʼ�������ݣ�ProcessNextChunk()��һ���ݹ麯��

	// ͨ����������ĵݹ麯�������������
	ProcessNextChunk(pModel, &currentChunk);

	// �ڶ�������3ds�ļ�֮�󣬼��㶥��ķ���
	ComputeNormals(pModel);

	// �ͷ��ڴ�ռ�
	CleanUp();

	return true;
}


//  ����ĺ����ͷ����е��ڴ�ռ䣬���ر��ļ�
void CLoad3DS::CleanUp()//**************************************************************************
{
if(m_FilePointer)
{	fclose(m_FilePointer);						// �رյ�ǰ���ļ�ָ��
	m_FilePointer=NULL;						
}					
}



//  ����ĺ�������3ds�ļ�����Ҫ����
void CLoad3DS::ProcessNextChunk(t3DModel *pModel, tChunk *pPreviousChunk)//***************************************************************
{
	t3DObject newObject = {0};					// ������ӵ���������
	tMaterialInfo newTexture = {0};				// ������ӵ���������
	tChunk currentChunk={0};                    // ������ӵ���ǰ������
	tChunk tempChunk={0};                       // ������ӵ���ʱ������
//	unsigned int version = 0;					// �����ļ��汾
	int gBuffer[50000] = {0};					// ������������Ҫ������

//	m_CurrentChunk = new tChunk;				// Ϊ�µĿ����ռ�		

	//  ����ÿ��һ���¿飬��Ҫ�ж�һ�¿��ID������ÿ�����Ҫ�Ķ���ģ����������
	//  ����ǲ���Ҫ����Ŀ飬���Թ�

	// ���������ӿ飬ֱ���ﵽԤ���ĳ���
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// ������һ����
		ReadChunk(&currentChunk);

		// �жϿ��ID��
		switch (currentChunk.ID)
		{
		case VERSION:							// �ļ��汾��
			
			// �ڸÿ�����һ���޷��Ŷ��������������ļ��İ汾
			
			// �����ļ��İ汾�ţ������ֽ�����ӵ�bytesRead������
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);

			// ����ļ��汾�Ŵ���3������һ��������Ϣ
			if ((currentChunk.length - currentChunk.bytesRead==4)&&(gBuffer[0]> 0x03))
				MessageBox(NULL, "This 3DS file is over version 3 so it may load incorrectly", "Warning", MB_OK);
			break;

		case OBJECTINFO:						// ����汾��Ϣ
			
			// ������һ����
			ReadChunk(&tempChunk);

			// �������İ汾��
			tempChunk.bytesRead += fread(gBuffer, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer);

			// ���Ӷ�����ֽ���
			currentChunk.bytesRead += tempChunk.bytesRead;

			// ������һ����
			ProcessNextChunk(pModel, &currentChunk);
			break;

		case MATERIAL:							// ������Ϣ

			// ���ʵ���Ŀ����
			pModel->numOfMaterials++;

			// ���������������һ���հ�����ṹ
			pModel->vctMaterials.push_back(newTexture);

			// �������װ�뺯��
			ProcessNextMaterialChunk(pModel, &currentChunk);
			break;

		case OBJECT:							// ���������
				
			// �ÿ��Ƕ�����Ϣ���ͷ���������˶���������

			// ����������
			pModel->numOfObjects++;
		
			// ���һ���µ�tObject�ڵ㵽����������
			pModel->vctObject.push_back(newObject);
			
			// ��ʼ������������������ݳ�Ա
			memset(&(pModel->vctObject[pModel->numOfObjects - 1]), 0, sizeof(t3DObject));

			// ��ò������������ƣ�Ȼ�����Ӷ�����ֽ���
			currentChunk.bytesRead += GetString(pModel->vctObject[pModel->numOfObjects - 1].strName);
			
			// ��������Ķ�����Ϣ�Ķ���
			ProcessNextObjectChunk(pModel, &(pModel->vctObject[pModel->numOfObjects - 1]), &currentChunk);
			break;

		case EDITKEYFRAME:

			// �����ؼ�֡��Ķ��룬������Ҫ������ֽ���
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;

		default: 
			
			//  �������к��ԵĿ�����ݵĶ��룬������Ҫ������ֽ���
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		}

		// ���Ӵ����������ֽ���
		pPreviousChunk->bytesRead += currentChunk.bytesRead;
	}

	// �ͷŵ�ǰ����ڴ�ռ�
//	delete m_CurrentChunk;
//	m_CurrentChunk = pPreviousChunk;
}


//  ����ĺ����������е��ļ��ж������Ϣ
void CLoad3DS::ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)//********************************************
{
	int gBuffer[50000] = {0};					// ���ڶ��벻��Ҫ������
	tChunk currentChunk={0};
	// ���µĿ����洢�ռ�
//	m_CurrentChunk = new tChunk;
	vector<tMatREF*> vctMatIDS;

	// ��������������ֱ�����ӿ����
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// ������һ����
		ReadChunk(&currentChunk);

		// ������������ֿ�
		switch (currentChunk.ID)
		{
		case OBJECT_MESH:					// ���������һ���¿�
		
			// ʹ�õݹ麯�����ã�������¿�
			ProcessNextObjectChunk(pModel, pObject, &currentChunk);
			break;

		case OBJECT_VERTICES:				// �����Ƕ��󶥵�
			ReadVertices(pObject, &currentChunk);
			break;

		case OBJECT_FACES:					// ������Ƕ������
			ReadVertexIndices(pObject, &currentChunk);
			break;

		case OBJECT_MATERIAL:				// ������Ƕ���Ĳ�������
			
			// �ÿ鱣���˶�����ʵ����ƣ�������һ����ɫ��Ҳ������һ������ӳ�䡣ͬʱ�ڸÿ���Ҳ������
			// ����������������

			// ����������Ĳ�������
			ReadObjectMaterial(pModel, pObject, &currentChunk,&vctMatIDS);			
			break;

		case OBJECT_UV:						// ��������UV��������

			// ��������UV��������
			ReadUVCoordinates(pObject, &currentChunk);
			break;

		default:  

			// �Թ�����Ҫ����Ŀ�
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		}

		// ��Ӵ������ж�����ֽ�����ǰ��Ķ�����ֽ���
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
	// �ͷŵ�ǰ����ڴ�ռ䣬���ѵ�ǰ������Ϊǰ���
//	delete m_CurrentChunk;
//	m_CurrentChunk = pPreviousChunk;
}

//  ����ĺ����������еĲ�����Ϣ
void CLoad3DS::ProcessNextMaterialChunk(t3DModel *pModel, tChunk *pPreviousChunk) //*****************************************************
{
	int gBuffer[50000] = {0};					// ���ڶ��벻��Ҫ������
	char *temp = NULL;
	// ����ǰ�����洢�ռ�
//	m_CurrentChunk = new tChunk;
	tChunk currentChunk={0};
	// ����������Щ�飬ֱ�����ӿ����
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// ������һ��
		ReadChunk(&currentChunk);

		// �ж϶������ʲô��
		switch (currentChunk.ID)
		{
		case MATNAME:							// ���ʵ�����
			
			// ������ʵ�����
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


		case MATDIFFUSE:						// �����R G B��ɫ
			ReadColorChunk(&(pModel->vctMaterials[pModel->numOfMaterials - 1]), &currentChunk,MATDIFFUSE);
			break;
		
		case MATMAP:							// ������Ϣ��ͷ��
			
			// ������һ�����ʿ���Ϣ
			ProcessNextMaterialChunk(pModel, &currentChunk);
			break;

		case MATMAPFILE:						// �����ļ�������
			
			//g_Buffer = g_TexturesPath+g_TexturesName;
			//temp = StringCat(temp,g_TexturesPath);
			// ������ʵ��ļ���
			currentChunk.bytesRead += fread(pModel->vctMaterials[pModel->numOfMaterials - 1].strFile, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			//pModel->vctMaterials[pModel->numOfMaterials - 1].strFile = StringCat(TEXTURESPATH,pModel->vctMaterials[pModel->numOfMaterials - 1].strFile);
			temp = StringCat(TEXTURESPATH,pModel->vctMaterials[pModel->numOfMaterials - 1].strFile);
			strcpy(pModel->vctMaterials[pModel->numOfMaterials - 1].strFile,temp);
			break;
		
		default:  

			// �ӹ�����Ҫ����Ŀ�
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;
		}

		// ��Ӵ������ж�����ֽ���
		pPreviousChunk->bytesRead += currentChunk.bytesRead;
	}

	// ɾ����ǰ�飬������ǰ������Ϊǰ��Ŀ�
	//delete m_CurrentChunk;
	//m_CurrentChunk = pPreviousChunk;
}

//  ���溯��������ID�ź������ֽڳ���
void CLoad3DS::ReadChunk(tChunk *pChunk)                       //****************************************************
{
	// ������ID�ţ�ռ����2���ֽڡ����ID����OBJECT��MATERIALһ����˵�����ڿ���������������
	pChunk->bytesRead = fread(&pChunk->ID, 1, 2, m_FilePointer);

	// Ȼ������ռ�õĳ��ȣ��������ĸ��ֽ�
	pChunk->bytesRead += fread(&pChunk->length, 1, 4, m_FilePointer);
}

//  ����ĺ�������һ���ַ���
int CLoad3DS::GetString(char *pBuffer)//**************************************************************
{
	int index = 0;

	// ����һ���ֽڵ�����
	fread(pBuffer, 1, 1, m_FilePointer);

	// ֱ������
	while (*(pBuffer + index++) != 0) {

		// ����һ���ַ�ֱ��NULL
		fread(pBuffer + index, 1, 1, m_FilePointer);
	}

	// �����ַ����ĳ���
	return strlen(pBuffer) + 1;
}


//  ����ĺ�������RGB��ɫ
void CLoad3DS::ReadColorChunk(tMaterialInfo *pMaterial, tChunk *pChunk,USHORT typeFlag) //*********************************************
{
	// ������ɫ����Ϣ
	

	tChunk tempChunk={0};
 BYTE btmp[3];  
 ReadChunk(&tempChunk);// ������ɫ����Ϣ
 switch(typeFlag) 
 { 
 case MAT_AMBIENT:      //���ʵĻ�������ɫ 
	 tempChunk.bytesRead += fread(btmp, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer); 
	 Bytes2Floats(btmp,pMaterial->ambient,3,1.0f/256.0f); 
	 pMaterial->ambient[3]=1.0f;
	 break; 
 case MAT_SPECULAR:      //���ʵľ������ɫ 
	 tempChunk.bytesRead += fread(btmp, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer); 
	 Bytes2Floats(btmp,pMaterial->specular,3,1.0f/256.0f); 
	 pMaterial->specular[3]=1.0f;
	 break;
	  case MAT_EMISSIVE:      //���ʵĳ������ɫ 
	 tempChunk.bytesRead += fread(btmp, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer); 
	 memset(btmp,0,3);
	 Bytes2Floats(btmp,pMaterial->emissive,3,1.0f/256.0f); 
	 pMaterial->emissive[3]=1.0f;
	 break;
case MATDIFFUSE:      //���ʵ����������ɫ 
default:
	 tempChunk.bytesRead += fread(pMaterial->color, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer);
	 Bytes2Floats(pMaterial->color,pMaterial->diffuse,3,1.0f/256.0f); 
	 pMaterial->diffuse[3]=1.0f;
	 break;
 }
	

	// ���Ӷ�����ֽ���
	pChunk->bytesRead += tempChunk.bytesRead;
}

//  ����ĺ������붥������
void CLoad3DS::ReadVertexIndices(t3DObject *pObject, tChunk *pPreviousChunk)//**************************************************
{
	unsigned short index = 0;					// ���ڶ��뵱ǰ�������

	// ����ö����������Ŀ
	pPreviousChunk->bytesRead += fread(&pObject->numOfFaces, 1, 2, m_FilePointer);

	// ����������Ĵ洢�ռ䣬����ʼ���ṹ
	pObject->pFaces = new tFace [pObject->numOfFaces];
	memset(pObject->pFaces, 0, sizeof(tFace) * pObject->numOfFaces);

	// �������������е���
	for(int i = 0; i < pObject->numOfFaces; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			// ���뵱ǰ��ĵ�һ���� 
			pPreviousChunk->bytesRead += fread(&index, 1, sizeof(index), m_FilePointer);

			if(j < 3)
			{
				// ��������������Ľṹ��
				pObject->pFaces[i].vertIndex[j] = index;
			}
		}
	}
}


//  ����ĺ�����������UV����
void CLoad3DS::ReadUVCoordinates(t3DObject *pObject, tChunk *pPreviousChunk)//**************************************************
{
	// Ϊ�˶�������UV���꣬������Ҫ����UV�����������Ȼ��Ŷ�����������

	// ����UV���������
	pPreviousChunk->bytesRead += fread(&pObject->numTexVertex, 1, 2, m_FilePointer);

	// ���䱣��UV������ڴ�ռ�
	pObject->pTexVerts = new CVector2 [pObject->numTexVertex];

	// ������������
	pPreviousChunk->bytesRead += fread(pObject->pTexVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}

//  �������Ķ���
void CLoad3DS::ReadVertices(t3DObject *pObject, tChunk *pPreviousChunk)//******************************************
{
	// �ڶ���ʵ�ʵĶ���֮ǰ�����ȱ���ȷ����Ҫ������ٸ����㡣
	
	// ���붥�����Ŀ
	pPreviousChunk->bytesRead += fread(&(pObject->numOfVerts), 1, 2, m_FilePointer);

	// ���䶥��Ĵ洢�ռ䣬Ȼ���ʼ���ṹ��
	pObject->pVerts = new CVector3 [pObject->numOfVerts];
	memset(pObject->pVerts, 0, sizeof(CVector3) * pObject->numOfVerts);

	// ���붥������
	pPreviousChunk->bytesRead += fread(pObject->pVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);

//8888888888888888888888888888888888888888888888888888888888888888888888
	pObject->m_minX=999999;pObject->m_maxX=-999999;
	pObject->m_minY=999999;pObject->m_maxX=-999999;
	pObject->m_minZ=999999;pObject->m_maxZ=-999999;
	
	
	
	
	// �����Ѿ����������еĶ��㡣
	// ��Ϊ3D Studio Max��ģ�͵�Z����ָ���ϵģ������Ҫ��y���z�ᷭת������
	// ����������ǽ�Y���Z�ύ����Ȼ��Z�ᷴ��

	// �������еĶ���
	for(int i = 0; i < pObject->numOfVerts; i++)
	{
		// ����Y���ֵ
		float fTempY = pObject->pVerts[i].y;

		// ����Y���ֵ����Z���ֵ
		pObject->pVerts[i].y = pObject->pVerts[i].z;

		// ����Z���ֵ����-Y���ֵ 
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


//  ����ĺ����������Ĳ�������
void CLoad3DS::ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk, vector<tMatREF*>*pvmatids)//***************************************************
{
	char strMaterial[255] = {0};			// �����������Ĳ�������
	int gBuffer[50000] = {0};				// �������벻��Ҫ������
    bool bmaterror=true;
	tMatREF *pMatref;
    pMatref=new tMatREF;
	// ���ʻ�������ɫ�������Ƕ��������Ҳ���ܱ������������ȡ�����ȵ���Ϣ��

	// ������븳�赱ǰ����Ĳ�������
	pPreviousChunk->bytesRead += GetString(strMaterial);

	// �������е�����
	for(int i = 0; i < pModel->numOfMaterials; i++)
	{
		//�������������뵱ǰ����������ƥ��
		if(strcmp(strMaterial, pModel->vctMaterials[i].strName) == 0)
		{
			// ���ò���ID
			pObject->materialID = i;
			pMatref->nMaterialID=i;

			// �ж��Ƿ�������ӳ�䣬���strFile��һ�����ȴ���1���ַ�������������
			if(strlen(pModel->vctMaterials[i].strFile) > 0) {

				// ���ö��������ӳ���־
				pObject->bHasTexture = true;
                pMatref->bHasTexture = true;
			}	
			bmaterror=false;
			break;
		}
		else
		{
			// ����ö���û�в��ʣ�������IDΪ-1
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



//  �������Щ������Ҫ�������㶥��ķ�����������ķ�������Ҫ�����������
// ����ĺ궨�����һ��ʸ���ĳ���
#define Mag(Normal) (sqrt(Normal.x*Normal.x + Normal.y*Normal.y + Normal.z*Normal.z))

// ����ĺ��������������ʸ��
CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2)//*******************************************************************************
{
	CVector3 vVector;							

	vVector.x = vPoint1.x - vPoint2.x;			
	vVector.y = vPoint1.y - vPoint2.y;			
	vVector.z = vPoint1.z - vPoint2.z;			

	return vVector;								
}

// ����ĺ�������ʸ�����
CVector3 AddVector(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vResult;							
	
	vResult.x = vVector2.x + vVector1.x;		
	vResult.y = vVector2.y + vVector1.y;		
	vResult.z = vVector2.z + vVector1.z;		

	return vResult;								
}

// ����ĺ�������ʸ��������
CVector3 DivideVectorByScaler(CVector3 vVector1, float Scaler)
{
	CVector3 vResult;							
	
	vResult.x = vVector1.x / Scaler;			
	vResult.y = vVector1.y / Scaler;			
	vResult.z = vVector1.z / Scaler;			

	return vResult;								
}

// ����ĺ�����������ʸ���Ĳ��
CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vCross;								
												
	vCross.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
												
	vCross.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
												
	vCross.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	return vCross;								
}

// ����ĺ����淶��ʸ��
CVector3 Normalize(CVector3 vNormal)
{
	double Magnitude;							

	Magnitude = Mag(vNormal);					// ���ʸ���ĳ���

	vNormal.x /= (float)Magnitude;				
	vNormal.y /= (float)Magnitude;				
	vNormal.z /= (float)Magnitude;				

	return vNormal;								
}

//  ����ĺ������ڼ������ķ�����
void CLoad3DS::ComputeNormals(t3DModel *pModel)//************************************************************************
{
	CVector3 vVector1, vVector2, vNormal, vPoly[3];
	int i,j;
	// ���ģ����û�ж����򷵻�
	if(pModel->numOfObjects <= 0)
		return;

	// ����ģ�������еĶ���
	for(int index = 0; index < pModel->numOfObjects; index++)
	{
		// ��õ�ǰ�Ķ���
		t3DObject *pObject = &(pModel->vctObject[index]);

		// ������Ҫ�Ĵ洢�ռ�
		CVector3 *pNormals		= new CVector3 [pObject->numOfFaces];
		CVector3 *pTempNormals	= new CVector3 [pObject->numOfFaces];
		pObject->pNormals		= new CVector3 [pObject->numOfVerts];

		// ���������������
		for( i=0; i < pObject->numOfFaces; i++)
		{												
			vPoly[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
			vPoly[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
			vPoly[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];

			// ������ķ�����

			vVector1 = Vector(vPoly[0], vPoly[2]);		// ��ö���ε�ʸ��
			vVector2 = Vector(vPoly[2], vPoly[1]);		// ��ö���εĵڶ���ʸ��

			vNormal  = Cross(vVector1, vVector2);		// �������ʸ���Ĳ��
			pTempNormals[i] = vNormal;					// ����ǹ淶��������
			vNormal  = Normalize(vNormal);				// �淶����õĲ��

			pNormals[i] = vNormal;						// ����������ӵ��������б���
		}

		//  �����󶥵㷨����
		CVector3 vSum = {0.0, 0.0, 0.0};
		CVector3 vZero = vSum;
		int shared=0;
		// �������еĶ���
		for ( i = 0; i < pObject->numOfVerts; i++)			
		{
			for ( j = 0; j < pObject->numOfFaces; j++)	// �������е���������
			{												// �жϸõ��Ƿ����������湲��
				if (pObject->pFaces[j].vertIndex[0] == i || 
					pObject->pFaces[j].vertIndex[1] == i || 
					pObject->pFaces[j].vertIndex[2] == i)
				{
					vSum = AddVector(vSum, pTempNormals[j]);
					shared++;								
				}
			}      
			
			pObject->pNormals[i] = DivideVectorByScaler(vSum, float(-shared));

			// �淶�����Ķ��㷨��
			pObject->pNormals[i] = Normalize(pObject->pNormals[i]);	

			vSum = vZero;								
			shared = 0;										
		}
	
		// �ͷŴ洢�ռ䣬��ʼ��һ������
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
