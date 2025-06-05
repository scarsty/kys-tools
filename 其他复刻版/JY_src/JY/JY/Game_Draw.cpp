
// INCLUDES ///////////////////////////////////////////////////////////////////

#include "Game_Draw.h"
#include "Image_Manager.h"

// DEFINES ////////////////////////////////////////////////////////////////////
// MACROS /////////////////////////////////////////////////////////////////////
// TYPES //////////////////////////////////////////////////////////////////////
// CLASS //////////////////////////////////////////////////////////////////////
// PROTOTYPES /////////////////////////////////////////////////////////////////
// EXTERNALS //////////////////////////////////////////////////////////////////
// GLOBALS ////////////////////////////////////////////////////////////////////

extern const DWORD COLOR_KEY = BGR(0,255,0);	// ɫ�ʹؼ��֣�͸��ɫ��

// FUNCTIONS //////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: Draw_RLE_Tile()
// Desc: ����ָ�������ŵ�RLEͼ�鵽ָ����Ļ����
//       ���������ŵ���Ч�ԣ��Ա�֤֮�����Ҫ�����ŵĺ�������������
//       �Զ������ڴ����Դ��ʹ�ã���������ʹ���ڴ����Դ�Ĵ�С
//-----------------------------------------------------------------------------
int Draw_RLE_Tile(const Mem_RLE * const p_rle, WORD index, const LOC * const p_loc)
	{
	assert(p_rle && p_loc);

	// ����������Ƿ���Ч
	if (index >= p_rle->nImages)
		{
		//g_Log.Write("ͼ��������%u����.idx�ļ��涨�ķ�Χ\r\n", index);
		//return -1;

		// �������Ÿ�Ϊ0
		index = 0;
		}

	///////////////////////////////////////////////////////////
	// ��ȡͼ�������Ϣ

	// ���������ţ���ȡͼ�������Ϣ��ָ��
	SURFACE *pSurface;
	if (CImageManager::GetSurfaceInfo(p_rle, index, &pSurface) != 0)
		{
		g_Log.Write("��ȡָ�������ŵı�����Ϣʧ��\r\n");
		return -1;
		}

	// ������صı���ָ��Ϊ�գ���˵��ͼ��Ϊ�գ�������ֱ�ӷ���
	if (pSurface == NULL)
		return 0;

	///////////////////////////////////////////////////////////
	// ��ͼ���ͼ�����任���󱸻������
	// ��������ͼ�������Ϣ����Ч���ѱ���֤

	// ͼ������
	RECT source_rect;
	source_rect.left	= 0;
	source_rect.top		= 0;
	source_rect.right	= pSurface->Image.nWidth;
	source_rect.bottom	= pSurface->Image.nHeight;

	// ��Ļ��������
	RECT dest_rect;
	dest_rect.left		= (LONG)(p_loc->x - pSurface->Image.nXCorrection);
	dest_rect.top		= (LONG)(p_loc->y - pSurface->Image.nYCorrection);
	dest_rect.right		= (LONG)(dest_rect.left + pSurface->Image.nWidth);
	dest_rect.bottom	= (LONG)(dest_rect.top + pSurface->Image.nHeight);

	// ��ͼ��任���󱸻��廭�棨ʹ��Դ�����ɫ�ʹؼ��֣�
	if (FAILED(lpddsback->Blt(&dest_rect, pSurface->pDDS, &source_rect,
							  (DDBLT_WAIT | DDBLT_KEYSRC), NULL)))
		{
		g_Log.Write("��%u��ͼ��任���󱸻��廭��(%d, %d, %d, %d)ʱʧ��\r\n",
			index, dest_rect.left, dest_rect.top, dest_rect.right, dest_rect.bottom);
		return 0;
		}

	// �Ѹ�ͼ����漤��
	CImageManager::SurfaceActivate(pSurface);

	///////////////////////////////////////////////////////////

	return 0;
	} // end Draw_RLE_Tile



//-----------------------------------------------------------------------------
// Name: Create_RLE_Surface()
// Desc: ���ݵ�ǰͼ���Ŵ���ͼ�����
//       Ҫ������߱�֤�����ŵ���Ч��
//-----------------------------------------------------------------------------
int Create_RLE_Surface(LPDIRECTDRAWSURFACE7 * const p_lpdds, 
					   const Mem_RLE * const p_rle, RLE_Image * const p_image, WORD const index)
	{
	assert(p_lpdds && p_rle && p_image);

	///////////////////////////////////////////////////////////
	// ��ȡͼ����Ϣ

	// ����ͼ����.grp�ļ��е�ƫ�Ƶ�ַ
	// ͼ��������Ϊ0ʱ���ܸ���û��.idx�ļ�
	DWORD addr_begin;
	if (index == 0)
		addr_begin = 0;
	else
		addr_begin = *((DWORD *)p_rle->Idx.pBuffer + index -1);

	// ��ȡͼ���ߡ�XY����
	p_image->nWidth = *(short *)(p_rle->Grp.pBuffer + addr_begin);
	p_image->nHeight = *(short *)(p_rle->Grp.pBuffer + addr_begin + 2);
	p_image->nXCorrection = *(short *)(p_rle->Grp.pBuffer + addr_begin + 4);
	p_image->nYCorrection = *(short *)(p_rle->Grp.pBuffer + addr_begin + 6);

	if (p_image->nWidth <= 0 || p_image->nHeight <= 0)
		{
		g_Log.Write(".grp�е�ͼ����Ϣ���� (width:%u, height:%u)\r\n", p_image->nWidth, p_image->nHeight);
		return -1;
		}

	///////////////////////////////////////////////////////////
	// ��������������ͼ��

	// �ͷ�֮ǰ����������
	if (*p_lpdds)
		{
		(*p_lpdds)->Release();
		*p_lpdds = NULL;
		}

	// ����ͼ���߽�����������
	*p_lpdds = DDraw_Create_Surface(p_image->nWidth, p_image->nHeight, DDSCAPS_VIDEOMEMORY, COLOR_KEY);
	if (*p_lpdds == NULL)
		{
		g_Log.Write("���������������\r\n");
		return -1;
		}

	// ��������
	DDRAW_INIT_STRUCT(ddsd);
	if (FAILED((*p_lpdds)->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL)))
		{
		g_Log.Write("�����������\r\n");
		return -1;
		}

	DWORD lpitch32 = (DWORD)(ddsd.lPitch / 4);
	DWORD *sur_buffer = (DWORD *)ddsd.lpSurface;
	BYTE *rle_buffer = p_rle->Grp.pBuffer + addr_begin + 8;

	// ���а�����д�����
	for (int i = 0; i< p_image->nHeight; ++i)
		{
		// ��һ������д�����
		int result = RleWriteLine32(sur_buffer, rle_buffer, p_image->nWidth, p_rle->Col.pBuffer);
		if (result < 0)
			{
			g_Log.Write("д��һ������ʱ�����к�%d��������%d��\r\n", i, result);
			return -1;
			}
		sur_buffer += lpitch32;
		rle_buffer += *rle_buffer + 1;		// rle�����Ƶ���һ�п�ʼ
		}

	// ��������
	if (FAILED((*p_lpdds)->Unlock(NULL)))
		{
		g_Log.Write("�����������\r\n");
		return -1;
		}

	return 0;
	} // end Create_RLE_Surface
