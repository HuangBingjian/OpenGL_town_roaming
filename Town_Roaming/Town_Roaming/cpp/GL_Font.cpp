// *************************************************************
// ������ʾ���壬��Ʒ���Ͻ���ʾ���������ּ��ǡ�
// *************************************************************

#include "..\header\Font.h"

GLFont::GLFont()
{
}
/** �������� */
GLFont::~GLFont()
{
	if(m_hFont)
		DeleteObject(m_hFont); /**< ɾ�������� */
}
/** ��ʼ������ */
bool GLFont::InitFont()
{
 
  /**< �������� */
   m_hFont = CreateFont(-16,					/**< ����߶� */
						0,						/**< ������ */
						0,						/**< �������ת�Ƕ� Angle Of Escapement */
						0,						/**< ������ߵ���ת�Ƕ�Orientation Angle */
						FW_BOLD,				/**< ��������� */
						FALSE,					/**< �Ƿ�ʹ��б�� */
						FALSE,					/**< �Ƿ�ʹ���»��� */
						FALSE,					/**< �Ƿ�ʹ��ɾ���� */
						GB2312_CHARSET,			/**< �����ַ��� */
						OUT_TT_PRECIS,			/**< ������� */
						CLIP_DEFAULT_PRECIS,	/**< �ü����� */
						ANTIALIASED_QUALITY,	/**< ������� */
				FF_DONTCARE|DEFAULT_PITCH,		/**< Family And Pitch */
						"����");				/**< �������� */
 if(!m_hFont)
		return false;  /**< ��������ʧ���򷵻� */
  /** ��ʼ���ɹ��򷵻�true */
   return true;
}

/** ��ָ��λ������ַ��� */
void GLFont::PrintText(char *string, float x, float y)
{
	HBITMAP hBitmap,hOldBmp; /**< ��������λͼ��� */
	BITMAP bm;               /**< λͼ�ṹ���� */
	SIZE size;               /**< λͼ�ߴ� */
	GLboolean lp,tp;
	HDC hDC = ::CreateCompatibleDC(0); /**< �ݴ��豸���� */
	glGetBooleanv(GL_LIGHTING,&lp);  /**< �鿴�������Ƿ��й��� */
	glGetBooleanv(GL_TEXTURE_2D,&tp);/**< �鿴�������Ƿ��������� */
	/** ���������һЩ���� */
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0,0,-10.0f);
	glDisable(GL_LIGHTING);    /**< �رչ��� */
	glDisable(GL_TEXTURE_2D);  /**< �ر����� */
	glDisable(GL_DEPTH_TEST);  /**< �ر���Ȳ��� */
	SelectObject(hDC, m_hFont); /**< ѡ������ */
	::GetTextExtentPoint32(hDC, string, strlen(string), &size);/**< ��ȡ�ַ�λͼ��С */
	hBitmap = CreateBitmap(size.cx, size.cy,1, 1, NULL); /**< ������hDC��ص�ɫλͼ */
	hOldBmp = (HBITMAP)SelectObject(hDC,hBitmap); /**< ѡ��λͼ */
  
	SetBkColor(hDC, RGB(0, 0, 0));              /**< ����ɫΪ��ɫ */
	SetTextColor(hDC, RGB(255, 255, 255));      /**< ������ɫ��ɫ */
	SetBkMode(hDC, OPAQUE);                     /**< �õ�ǰ�ı�����ɫ��䱳�� */
	TextOut(hDC, 0, 0, string, strlen(string)); /**< ������ֵ��ݴ�hDC */
	/** ������λͼ���ݽṹ */
	GetObject(hBitmap, sizeof(bm), &bm);
	size.cx = (bm.bmWidth + 31) & (~31); /**< ��Ե���� */
	size.cy = bm.bmHeight;
	int bufsize = size.cx * size.cy/8;  /**< ͼ�����ݳ��� */
    /**�����嵥ɫλͼ�ṹ */  
	struct {
	BITMAPINFOHEADER bih;
	RGBQUAD col[2];
	}bic;
    /** ��ȡ��ɫλͼ�ṹ��Ϣ */
	BITMAPINFO *binf = (BITMAPINFO *)&bic;
	binf->bmiHeader.biSize = sizeof(binf->bmiHeader); /**< �޸Ľṹ��Ϣ */
	binf->bmiHeader.biWidth = bm.bmWidth;
	binf->bmiHeader.biHeight = bm.bmHeight;
	binf->bmiHeader.biPlanes = 1;
	binf->bmiHeader.biBitCount = 1;       /**< ��ɫ */
	binf->bmiHeader.biCompression = BI_RGB;  /**< ��ɫ��ʽ */
	binf->bmiHeader.biSizeImage = bufsize;
	binf->bmiHeader.biXPelsPerMeter = 1;
	binf->bmiHeader.biYPelsPerMeter = 1;
	binf->bmiHeader.biClrUsed = 0;
	binf->bmiHeader.biClrImportant = 0;
	/** ����ͼ�����ݿ� */ 
	UCHAR* pBmpBits = new UCHAR[bufsize];	
	memset(pBmpBits, 0, sizeof(UCHAR)*bufsize);
	/** ���豸�޹����ݱ�����pBmpBitsָ������ݿ��� */
	::GetDIBits(hDC, hBitmap, 0, bm.bmHeight, pBmpBits, binf,DIB_RGB_COLORS);
	 
	/** ��ʾ�ַ��� */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); /**< ָ�����ش洢ģʽ */
	glRasterPos2f(x,y);                  /**< ��λ */
	glBitmap(size.cx, size.cy, 0.0, 0.0, 0.0, 0.0, pBmpBits); /**< λͼ��ʾ */
	delete pBmpBits;                       /**< ɾ��ָ�� */
	SelectObject(hDC, hOldBmp);           /**< �ָ�ԭ��λͼ��Ϣ */
	DeleteObject(hBitmap);
	::DeleteDC(hDC);
	/** �ָ�һЩ���� */
	if(lp)
         glEnable(GL_LIGHTING);       /**< ���ù��� */
	if(tp)
		glEnable(GL_TEXTURE_2D);      /**< �������� */
	glEnable(GL_DEPTH_TEST);          /**< ������Ȳ��� */
	glPopMatrix();
}


