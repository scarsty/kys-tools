
// INCLUDES ///////////////////////////////////////////////////////////////////

#include "World_Map.h"
#include "Game_Draw.h"

// DEFINES ////////////////////////////////////////////////////////////////////
// MACROS /////////////////////////////////////////////////////////////////////
// TYPES //////////////////////////////////////////////////////////////////////
// CLASS //////////////////////////////////////////////////////////////////////
// PROTOTYPES /////////////////////////////////////////////////////////////////
// EXTERNALS //////////////////////////////////////////////////////////////////
// GLOBALS ////////////////////////////////////////////////////////////////////

// ��ͼ����ͼ����
const WORD nTILE_WIDTH = 36;
const WORD nTILE_HEIGHT = 18;

// FUNCTIONS //////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: CWorldMap()
// Desc: ���캯��
//-----------------------------------------------------------------------------
CWorldMap::CWorldMap()
	{
	m_nGetDrawPosStep = 0;

	///////////////////////////////////////////////////////////
	// ��������������Ҫ��ʾ��ͼ�������μ�World_Map.txt��

	DWORD r = g_Cfg.m_nFrameWidth / (nTILE_WIDTH *2);
	DWORD p = g_Cfg.m_nFrameWidth % (nTILE_WIDTH *2);
	int n;							// ������Ҫ��ʾ��ͼ����
	bool b_stagger_h = false;		// ˮƽ�������ͼ�齻���μ�World_Map.txt��
	
	if (p > nTILE_WIDTH || p == 0)
		n = r + (p>0 ? 1 : 0);
	else
		{
		n = r + (p>0 ? 1 : 0) -1;
		b_stagger_h = true;
		}

	DWORD s = g_Cfg.m_nFrameHeight / (nTILE_HEIGHT *2);
	DWORD q = g_Cfg.m_nFrameHeight % (nTILE_HEIGHT *2);
	int m;							// ������Ҫ��ʾ��ͼ��������m����m-1��
	bool b_stagger_v = false;		// ��ֱ�������ͼ�齻��

	if (q > nTILE_HEIGHT || q == 0)
		{
		m = s + (q>0 ? 1 : 0);
		b_stagger_v = true;
		}
	else
		m = s + (q>0 ? 1 : 0);

	///////////////////////////////////////////////////////////
	// �����ĸ��ǵ�ͼ����Ի���ͼ��ĵ�ͼ����ƫ��ֵ(dx,dy)

	// ˮƽ����ֱ������ʱ
	if (!b_stagger_h && !b_stagger_v)
		{
		m_locTopRight.x		= -m + n;
		m_locTopRight.y		= -m - n;
		m_locTopLeft.x		= -m - n;
		m_locTopLeft.y		= -m + n;
		m_locBottomRight.x	=  m + n -1;
		m_locBottomRight.y	=  m - n -1;
		m_locBottomLeft.x	=  m - n -1;
		m_locBottomLeft.y	=  m + n -1;
		m_locRightTop.x		= -m + n;
		m_locRightTop.y		= -m - n;
		m_locRightBottom.x	=  m + n -1;
		m_locRightBottom.y	=  m - n -1;
		m_locLeftTop.x		= -m - n;
		m_locLeftTop.y		= -m + n;
		m_locLeftBottom.x	=  m - n -1;
		m_locLeftBottom.y	=  m + n -1;
		}
	// ˮƽ����ֱ����ʱ
	else if (b_stagger_h && b_stagger_v)
		{
		m_locTopRight.x		= -m + n;
		m_locTopRight.y		= -m - n -1;
		m_locTopLeft.x		= -m - n -1;
		m_locTopLeft.y		= -m + n;
		m_locBottomRight.x	=  m + n;
		m_locBottomRight.y	=  m - n -1;
		m_locBottomLeft.x	=  m - n -1;
		m_locBottomLeft.y	=  m + n;
		m_locRightTop.x		= -m + n;
		m_locRightTop.y		= -m - n -1;
		m_locRightBottom.x	=  m + n;
		m_locRightBottom.y	=  m - n -1;
		m_locLeftTop.x		= -m - n -1;
		m_locLeftTop.y		= -m + n;
		m_locLeftBottom.x	=  m - n -1;
		m_locLeftBottom.y	=  m + n;
		}
	// ˮƽ������ֱ����ʱ
	else if (b_stagger_h && !b_stagger_v)
		{
		m_locTopRight.x		= -m + n;
		m_locTopRight.y		= -m - n;
		m_locTopLeft.x		= -m - n;
		m_locTopLeft.y		= -m + n;
		m_locBottomRight.x	=  m + n -1;
		m_locBottomRight.y	=  m - n -1;
		m_locBottomLeft.x	=  m - n -1;
		m_locBottomLeft.y	=  m + n -1;
		m_locRightTop.x		= -m + n +1;
		m_locRightTop.y		= -m - n;
		m_locRightBottom.x	=  m + n -1;
		m_locRightBottom.y	=  m - n -2;
		m_locLeftTop.x		= -m - n;
		m_locLeftTop.y		= -m + n +1;
		m_locLeftBottom.x	=  m - n -2;
		m_locLeftBottom.y	=  m + n -1;
		}
	// ˮƽ��������ֱ��ʱ
	else
		{
		m_locTopRight.x		= -m + n -1;
		m_locTopRight.y		= -m - n;
		m_locTopLeft.x		= -m - n;
		m_locTopLeft.y		= -m + n -1;
		m_locBottomRight.x	=  m + n -1;
		m_locBottomRight.y	=  m - n;
		m_locBottomLeft.x	=  m - n;
		m_locBottomLeft.y	=  m + n -1;
		m_locRightTop.x		= -m + n;
		m_locRightTop.y		= -m - n;
		m_locRightBottom.x	=  m + n -1;
		m_locRightBottom.y	=  m - n -1;
		m_locLeftTop.x		= -m - n;
		m_locLeftTop.y		= -m + n;
		m_locLeftBottom.x	=  m - n -1;
		m_locLeftBottom.y	=  m + n -1;
		}
	} // end CWorldMap



//-----------------------------------------------------------------------------
// Name: ~CWorldMap()
// Desc: ��������
//-----------------------------------------------------------------------------
CWorldMap::~CWorldMap()
	{
	// ж�������ͼ�������
	Unload_RLE(&m_mrMmap);
	Unload_COL(&m_mfCol);
	UnloadFile(&m_mfEarth);
	UnloadFile(&m_mfSurface);
	UnloadFile(&m_mfBuilding);
	UnloadFile(&m_mfBuildx);
	UnloadFile(&m_mfBuildy);
	}



//-----------------------------------------------------------------------------
// Name: Init()
// Desc: ��ʼ�������ͼ����
//-----------------------------------------------------------------------------
int CWorldMap::Init()
	{
	///////////////////////////////////////////////////////////
	// ���������ͼ����

	// ����RLEͼ���ļ�����
	int result = Load_RLE(&m_mrMmap, g_Cfg.m_fnMmapGrp, g_Cfg.m_fnMmapIdx);
	if (result != 0)
		{
		g_Log.Write("����GRP��IDX�ļ�ʱ����\r\n");
		return -1;
		}

	// �����ɫ���ļ�����
	result = Load_COL(&m_mfCol, g_Cfg.m_fnMmapCol);
	if (result != 0)
		{
		g_Log.Write("����COL�ļ�ʱ����\r\n");
		return -1;
		}

	// �ѵ�ɫ����Ϣ���ӵ�ͼ��������Ϣ��
	Attach_COL(&m_mrMmap, &m_mfCol);

	// ����͸��ɫ
	Set_Trans_Color(COLOR_KEY);

	///////////////////////////////////////////
	// ����ͼ����������

	result = LoadFile(g_Cfg.m_fnEarth, &m_mfEarth, 0);
	if (result != 0)
		{
		g_Log.Write("����Earth�ļ�ʱ����\r\n");
		return -1;
		}

	result = LoadFile(g_Cfg.m_fnSurface, &m_mfSurface, 0);
	if (result != 0)
		{
		g_Log.Write("����Surface�ļ�ʱ����\r\n");
		return -1;
		}

	result = LoadFile(g_Cfg.m_fnBuilding, &m_mfBuilding, 0);
	if (result != 0)
		{
		g_Log.Write("����Building�ļ�ʱ����\r\n");
		return -1;
		}

	result = LoadFile(g_Cfg.m_fnBuildx, &m_mfBuildx, 0);
	if (result != 0)
		{
		g_Log.Write("����BuildX�ļ�ʱ����\r\n");
		return -1;
		}

	result = LoadFile(g_Cfg.m_fnBuildy, &m_mfBuildy, 0);
	if (result != 0)
		{
		g_Log.Write("����BuildY�ļ�ʱ����\r\n");
		return -1;
		}

	///////////////////////////////////////////////////////////
	// ��ͼ�������ֵ����ʱ�����㣬ֱ�Ӹ�����

	m_locMax.x = 479;
	m_locMax.y = 479;

	return 0;
	} // end Init



//-----------------------------------------------------------------------------
// Name: Show()
// Desc: ����Ļ����ʾ�����ͼ
//       ����ͼ�������������
//       ���Ʒ����μ�World_Map.txt
//-----------------------------------------------------------------------------
int CWorldMap::Show(LOC * const plocCenter)
	{
	assert(plocCenter);

	///////////////////////////////////////////////////////////
	// ���Ƶ�ͼͼ��
	// ����ͼ��������Ҵ��ϵ��»���

	// ����ͬһY���ͼ��ʱ����ʼ�ͽ������꣨������������ƫ��ֵ��
	LOC begin_loc, end_loc;

	///////////////////////////////////////////
	// �Ե�ͼ�������������

	// ��һ����Ƶ���ͼ��
	while ( __GetDrawPos(&begin_loc, &end_loc) )
		{
		// ����һ��ͼ�飨ͬY�ᣩ
		for (LOC draw_loc = begin_loc; draw_loc.x <= end_loc.x; ++draw_loc.x)
			{
			// ���ݵ�ͼ����ƫ��ֵ�������ֵ��Ȼ�����ͼ������ֵ
			LOC abs_loc;
			abs_loc.x = plocCenter->x + draw_loc.x;
			abs_loc.y = plocCenter->y + draw_loc.y;
			
			// ����ͼ������
			WORD index;
			// �����������ֵ��ʵ�ʷ�Χ�⣬��ͼ������Ϊ0
			if (abs_loc.x > m_locMax.x || abs_loc.x < 0 || abs_loc.y > m_locMax.y || abs_loc.y < 0)
				index = 0;
			else
				index = *(WORD *)( m_mfEarth.pBuffer + abs_loc.x*2 + abs_loc.y*2*(m_locMax.x +1) ) /2;

			// �ѵ�ͼ����ƫ��ֵת��Ϊ��Ļ���꣨�μ�World_Map.txt��
			LOC screen_loc;
			screen_loc.x = g_Cfg.m_nFrameWidth/2 + (draw_loc.x - draw_loc.y)*(nTILE_WIDTH/2);
			screen_loc.y = g_Cfg.m_nFrameHeight/2 -1 + (draw_loc.x + draw_loc.y +2)*(nTILE_HEIGHT/2);

			// ����ָ�������ŵĵ���ͼ�鵽ָ����Ļ����
			int result = Draw_RLE_Tile(&m_mrMmap, index, &screen_loc);
			if (result != 0)
				{
				g_Log.Write("���������ͼ(%d, %d)���ĵ���ͼ��%u����Ļ����(%d, %d)ʧ��\r\n",
							draw_loc.x, draw_loc.y, index, screen_loc.x, screen_loc.y);
				return -1;
				}
			}
		}

	// �ڶ�����Ʊ���ͼ��
	while ( __GetDrawPos(&begin_loc, &end_loc) )
		{
		// ����һ��ͼ�飨ͬY�ᣩ
		for (LOC draw_loc = begin_loc; draw_loc.x <= end_loc.x; ++draw_loc.x)
			{
			// ���ݵ�ͼ����ƫ��ֵ�������ֵ��Ȼ�����ͼ������ֵ
			LOC abs_loc;
			abs_loc.x = plocCenter->x + draw_loc.x;
			abs_loc.y = plocCenter->y + draw_loc.y;
			
			// ����ͼ������
			WORD index;
			// �����������ֵ��ʵ�ʷ�Χ�⣬��ͼ������Ϊ0
			if (abs_loc.x > m_locMax.x || abs_loc.x < 0 || abs_loc.y > m_locMax.y || abs_loc.y < 0)
				index = 0;
			else
				index = *(WORD *)( m_mfSurface.pBuffer + abs_loc.x*2 + abs_loc.y*2*(m_locMax.x +1) ) /2;

			// �ѵ�ͼ����ƫ��ֵת��Ϊ��Ļ���꣨�μ�World_Map.txt��
			LOC screen_loc;
			screen_loc.x = g_Cfg.m_nFrameWidth/2 + (draw_loc.x - draw_loc.y)*(nTILE_WIDTH/2);
			screen_loc.y = g_Cfg.m_nFrameHeight/2 -1 + (draw_loc.x + draw_loc.y +2)*(nTILE_HEIGHT/2);

			// ����ָ�������ŵı���ͼ�鵽ָ����Ļ����
			int result = Draw_RLE_Tile(&m_mrMmap, index, &screen_loc);
			if (result != 0)
				{
				g_Log.Write("���������ͼ(%d, %d)���ı���ͼ��%u����Ļ����(%d, %d)ʧ��\r\n",
							draw_loc.x, draw_loc.y, index, screen_loc.x, screen_loc.y);
				return -1;
				}
			}
		}

	// ��������ƽ���ͼ��
	while ( __GetDrawPos(&begin_loc, &end_loc) )
		{
		// ����һ��ͼ�飨ͬY�ᣩ
		for (LOC draw_loc = begin_loc; draw_loc.x <= end_loc.x; ++draw_loc.x)
			{
			// ���ݵ�ͼ����ƫ��ֵ�������ֵ��Ȼ�����ͼ������ֵ
			LOC abs_loc;
			abs_loc.x = plocCenter->x + draw_loc.x;
			abs_loc.y = plocCenter->y + draw_loc.y;
			
			// ����ͼ������
			WORD index;
			// �����������ֵ��ʵ�ʷ�Χ�⣬��ͼ������Ϊ0
			if (abs_loc.x > m_locMax.x || abs_loc.x < 0 || abs_loc.y > m_locMax.y || abs_loc.y < 0)
				index = 0;
			else
				index = *(WORD *)( m_mfBuilding.pBuffer + abs_loc.x*2 + abs_loc.y*2*(m_locMax.x +1) ) /2;

			// �ѵ�ͼ����ƫ��ֵת��Ϊ��Ļ���꣨�μ�World_Map.txt��
			LOC screen_loc;
			screen_loc.x = g_Cfg.m_nFrameWidth/2 + (draw_loc.x - draw_loc.y)*(nTILE_WIDTH/2);
			screen_loc.y = g_Cfg.m_nFrameHeight/2 -1 + (draw_loc.x + draw_loc.y +2)*(nTILE_HEIGHT/2);

			// ����ָ�������ŵĽ���ͼ�鵽ָ����Ļ����
			int result = Draw_RLE_Tile(&m_mrMmap, index, &screen_loc);
			if (result != 0)
				{
				g_Log.Write("���������ͼ(%d, %d)���Ľ���ͼ��%u����Ļ����(%d, %d)ʧ��\r\n",
							draw_loc.x, draw_loc.y, index, screen_loc.x, screen_loc.y);
				return -1;
				}
			}
		}

	return 0;
	} // end Show



////-----------------------------------------------------------------------------
//// Name: Show()
//// Desc: ����Ļ����ʾ�����ͼ
////       ����Ļ�ϵ���������
////       ���Ʒ����μ�World_Map.txt
////-----------------------------------------------------------------------------
//int CWorldMap::Show(LOC * const plocCenter)
//	{
//	assert(plocCenter);
//
//	///////////////////////////////////////////////////////////
//	// ���Ƶ�ͼͼ��
//	// ����Ļ����ϵ�Ĵ����Ҵ��ϵ��»���
//
//	// ����һˮƽ��ʱ����ʼ�ͽ������꣨������������ƫ��ֵ��
//	LOC begin_loc, end_loc;
//
//	///////////////////////////////////////////
//	// ����Ļ�ϵ���������
//
//	// ��һ����Ƶ���ͼ��
//	while ( __GetDrawPos(&begin_loc, &end_loc) )
//		{
//		// ����һ��ͼ�飨��Ļ�ϴ����ң���ͼ����x+1,y-1��
//		for (LOC draw_loc = begin_loc; draw_loc.x <= end_loc.x; ++draw_loc.x, --draw_loc.y)
//			{
//			// ���ݵ�ͼ����ƫ��ֵ�������ֵ��Ȼ�����ͼ������ֵ
//			LOC abs_loc;
//			abs_loc.x = plocCenter->x + draw_loc.x;
//			abs_loc.y = plocCenter->y + draw_loc.y;
//			
//			// ����ͼ������
//			WORD index;
//			// �����������ֵ��ʵ�ʷ�Χ�⣬��ͼ������Ϊ0
//			if (abs_loc.x > m_locMax.x || abs_loc.x < 0 || abs_loc.y > m_locMax.y || abs_loc.y < 0)
//				index = 0;
//			else
//				index = *(WORD *)( m_mfEarth.pBuffer + abs_loc.x*2 + abs_loc.y*2*(m_locMax.x +1) ) /2;
//
//			// �ѵ�ͼ����ƫ��ֵת��Ϊ��Ļ���꣨�μ�World_Map.txt��
//			LOC screen_loc;
//			screen_loc.x = g_Cfg.m_nFrameWidth/2 + (draw_loc.x - draw_loc.y)*(nTILE_WIDTH/2);
//			screen_loc.y = g_Cfg.m_nFrameHeight/2 -1 + (draw_loc.x + draw_loc.y +2)*(nTILE_HEIGHT/2);
//
//			// ����ָ�������ŵĵ���ͼ�鵽ָ����Ļ����
//			int result = Draw_RLE_Tile(&m_mrMmap, index, &screen_loc);
//			if (result != 0)
//				{
//				g_Log.Write("���������ͼ(%d, %d)���ĵ���ͼ��%u����Ļ����(%d, %d)ʧ��\r\n",
//							draw_loc.x, draw_loc.y, index, screen_loc.x, screen_loc.y);
//				return -1;
//				}
//			}
//		}
//
//	// �ڶ�����Ʊ���ͼ��
//	while ( __GetDrawPos(&begin_loc, &end_loc) )
//		{
//		// ����һ��ͼ�飨��Ļ�ϴ����ң���ͼ����x+1,y-1��
//		for (LOC draw_loc = begin_loc; draw_loc.x <= end_loc.x; ++draw_loc.x, --draw_loc.y)
//			{
//			// ���ݵ�ͼ����ƫ��ֵ�������ֵ��Ȼ�����ͼ������ֵ
//			LOC abs_loc;
//			abs_loc.x = plocCenter->x + draw_loc.x;
//			abs_loc.y = plocCenter->y + draw_loc.y;
//			
//			// ����ͼ������
//			WORD index;
//			// �����������ֵ��ʵ�ʷ�Χ�⣬��ͼ������Ϊ0
//			if (abs_loc.x > m_locMax.x || abs_loc.x < 0 || abs_loc.y > m_locMax.y || abs_loc.y < 0)
//				index = 0;
//			else
//				index = *(WORD *)( m_mfSurface.pBuffer + abs_loc.x*2 + abs_loc.y*2*(m_locMax.x +1) ) /2;
//
//			// �ѵ�ͼ����ƫ��ֵת��Ϊ��Ļ���꣨�μ�World_Map.txt��
//			LOC screen_loc;
//			screen_loc.x = g_Cfg.m_nFrameWidth/2 + (draw_loc.x - draw_loc.y)*(nTILE_WIDTH/2);
//			screen_loc.y = g_Cfg.m_nFrameHeight/2 -1 + (draw_loc.x + draw_loc.y +2)*(nTILE_HEIGHT/2);
//
//			// ����ָ�������ŵı���ͼ�鵽ָ����Ļ����
//			int result = Draw_RLE_Tile(&m_mrMmap, index, &screen_loc);
//			if (result != 0)
//				{
//				g_Log.Write("���������ͼ(%d, %d)���ı���ͼ��%u����Ļ����(%d, %d)ʧ��\r\n",
//							draw_loc.x, draw_loc.y, index, screen_loc.x, screen_loc.y);
//				return -1;
//				}
//			}
//		}
//
//	// ��������ƽ���ͼ��
//	while ( __GetDrawPos(&begin_loc, &end_loc) )
//		{
//		// ����һ��ͼ�飨��Ļ�ϴ����ң���ͼ����x+1,y-1��
//		for (LOC draw_loc = begin_loc; draw_loc.x <= end_loc.x; ++draw_loc.x, --draw_loc.y)
//			{
//			// ���ݵ�ͼ����ƫ��ֵ�������ֵ��Ȼ�����ͼ������ֵ
//			LOC abs_loc;
//			abs_loc.x = plocCenter->x + draw_loc.x;
//			abs_loc.y = plocCenter->y + draw_loc.y;
//			
//			// ����ͼ������
//			WORD index;
//			// �����������ֵ��ʵ�ʷ�Χ�⣬��ͼ������Ϊ0
//			if (abs_loc.x > m_locMax.x || abs_loc.x < 0 || abs_loc.y > m_locMax.y || abs_loc.y < 0)
//				index = 0;
//			else
//				index = *(WORD *)( m_mfBuilding.pBuffer + abs_loc.x*2 + abs_loc.y*2*(m_locMax.x +1) ) /2;
//
//			// �ѵ�ͼ����ƫ��ֵת��Ϊ��Ļ���꣨�μ�World_Map.txt��
//			LOC screen_loc;
//			screen_loc.x = g_Cfg.m_nFrameWidth/2 + (draw_loc.x - draw_loc.y)*(nTILE_WIDTH/2);
//			screen_loc.y = g_Cfg.m_nFrameHeight/2 -1 + (draw_loc.x + draw_loc.y +2)*(nTILE_HEIGHT/2);
//
//			// ����ָ�������ŵĽ���ͼ�鵽ָ����Ļ����
//			int result = Draw_RLE_Tile(&m_mrMmap, index, &screen_loc);
//			if (result != 0)
//				{
//				g_Log.Write("���������ͼ(%d, %d)���Ľ���ͼ��%u����Ļ����(%d, %d)ʧ��\r\n",
//							draw_loc.x, draw_loc.y, index, screen_loc.x, screen_loc.y);
//				return -1;
//				}
//			}
//		}
//
//	return 0;
//	} // end Show



//-----------------------------------------------------------------------------
// Name: __GetDrawPos()
// Desc: ����Ҫ���Ƶ�һ��ͼ������ͼ����յ�ͼ���λ�ã�������������ƫ��ֵ���������Ƿ���Ҫ��������
//       ����ͼ���꣨��������Ļ���꣩�������ң����ϵ��»���
//-----------------------------------------------------------------------------
bool CWorldMap::__GetDrawPos(LOC * const plocBegin, LOC * const plocEnd)
	{
	assert(plocBegin && plocEnd);

	// ��ǰ��������ҽǣ��յ������Ͻ�
	if (m_nGetDrawPosStep == 0)
		{
		*plocBegin = m_locTopRight;
		*plocEnd = m_locRightTop;

		// �ı��´�������յ��λ��ָʾ
		m_nGetDrawPosStep = 1;		// �� - ��
		}
	// ��ǰ������ϣ��յ�����
	else if (m_nGetDrawPosStep == 1)
		{
		// �����㵽������ǣ�����ɵ��������������
		if (plocBegin->x == m_locTopLeft.x && plocBegin->y == m_locTopLeft.y)
			{
			m_nGetDrawPosStep = 2;		// �� - ��

			// ���һ����һ�������������ϵ�y����Ҫ��1
			if (plocBegin->y < m_locLeftTop.y)
				plocBegin->y = m_locLeftTop.y;
			// �������û�б仯������Ҫ���������������һ��
			else
				{
				++plocBegin->x;
				++plocBegin->y;
				}
			}
		// ��������ƽ��һ��λ��
		else
			{
			--plocBegin->x;
			++plocBegin->y;
			}

		// ����յ㵽�����½ǣ�����ɵ����±���������
		if (plocEnd->x == m_locRightBottom.x && plocEnd->y == m_locRightBottom.y)
			{
			m_nGetDrawPosStep = 3;		// �� - ��

			// ���һ����һ�������������ҵ�y����Ҫ��1
			if (plocEnd->y < m_locBottomRight.y)
				plocEnd->y = m_locBottomRight.y;
			// �������û�б仯������Ҫ���������±�����һ��
			else
				{
				--plocEnd->x;
				++plocEnd->y;
				}
			}
		// ��������ƽ��һ��λ��
		else
			{
			++plocEnd->x;
			++plocEnd->y;
			}
		}
	// ��ǰ������ϣ��յ�����
	else if (m_nGetDrawPosStep == 3)
		{
		// �����㵽������ǣ�����ɵ��������������
		if (plocBegin->x == m_locTopLeft.x && plocBegin->y == m_locTopLeft.y)
			{
			m_nGetDrawPosStep = 4;		// �� - ��

			// ���һ����һ�������������ϵ�y����Ҫ��1
			if (plocBegin->y < m_locLeftTop.y)
				plocBegin->y = m_locLeftTop.y;
			// �������û�б仯������Ҫ���������������һ��
			else
				{
				++plocBegin->x;
				++plocBegin->y;
				}
			}
		// ��������ƽ��һ��λ��
		else
			{
			--plocBegin->x;
			++plocBegin->y;
			}

		// �յ�����ƽ��һ��λ��
		--plocEnd->x;
		++plocEnd->y;
		}
	// ��ǰ��������յ�����
	else if (m_nGetDrawPosStep == 2)
		{
		// �������ƽ��һ��λ��
		++plocBegin->x;
		++plocBegin->y;

		// ����յ㵽�����½ǣ�����ɵ����±���������
		if (plocEnd->x == m_locRightBottom.x && plocEnd->y == m_locRightBottom.y)
			{
			m_nGetDrawPosStep = 4;		// �� - ��

			// ���һ����һ�������������ҵ�y����Ҫ��1
			if (plocEnd->y < m_locBottomRight.y)
				plocEnd->y = m_locBottomRight.y;
			// �������û�б仯������Ҫ���������±�����һ��
			else
				{
				--plocEnd->x;
				++plocEnd->y;
				}
			}
		// ��������ƽ��һ��λ��
		else
			{
			++plocEnd->x;
			++plocEnd->y;
			}
		}
	// ��ǰ��������յ�����
	else
		{
		// �����㵽�����½ǣ���˵���������
		// ��λ��ָʾ��ԭ��Ȼ�󷵻�false
		if (plocBegin->x == m_locLeftBottom.x && plocBegin->y == m_locLeftBottom.y)
			{
			m_nGetDrawPosStep = 0;		// ���� - ����
			return false;
			}
		// �����������ƽ��һ��λ�ã��յ�����ƽ��һ��λ��
		else
			{
			++plocBegin->x;
			++plocBegin->y;
			--plocEnd->x;
			++plocEnd->y;
			}
		}

	return true;
	} // end __GetDrawPos


////-----------------------------------------------------------------------------
//// Name: __GetDrawPos()
//// Desc: ����Ҫ���Ƶ�һ��ͼ������ͼ����յ�ͼ���λ�ã�������������ƫ��ֵ���������Ƿ���Ҫ��������
////       ����Ļ����ϵ�������ң����ϵ��»���
////       �ʼ���������ǡ��յ������ҽǣ�������������ǡ��յ������ҽ�
////       ��䣬����յ�ı仯����Z�֣�ֻ�ж˵��Ƿ��غϻ�Ӱ�쵽��ʼ��Z�ֵķ���
////-----------------------------------------------------------------------------
//bool CWorldMap::__GetDrawPos(LOC * const plocBegin, LOC * const plocEnd)
//	{
//	assert(plocBegin && plocEnd);
//
//	switch (m_nGetDrawPosStep)
//		{
//		// ��һ�λ��ƣ�����������
//		case 0:
//			{
//			*plocBegin = m_locTopLeft;
//			*plocEnd = m_locTopRight;
//
//			// ��������ϲ�ͬ������������Z�֣����y+1���յ�x+1��
//			if (m_locTopLeft.y != m_locLeftTop.y)
//				m_nGetDrawPosStep = 1;
//			// ��ͬ������������Z�֣����x+1���յ�y+1��
//			else
//				m_nGetDrawPosStep = 2;
//			}
//			break;
//		// ������Z�֣����y+1���յ�x+1��
//		case 1:
//			{
//			// ��㵽������ǣ�˵���������
//			// �������㣬�����ѻ������
//			if (plocBegin->x == m_locBottomLeft.x && plocBegin->y == m_locBottomLeft.y)
//				{
//				m_nGetDrawPosStep = 0;
//				return false;
//				}
//
//			++plocBegin->y;
//			++plocEnd->x;
//
//			// �´�������Z��
//			m_nGetDrawPosStep = 2;
//			}
//			break;
//		// ������Z�֣����x+1���յ�y+1��
//		case 2:
//			{
//			// ��㵽������ǣ�˵���������
//			// �������㣬�����ѻ������
//			if (plocBegin->x == m_locBottomLeft.x && plocBegin->y == m_locBottomLeft.y)
//				{
//				m_nGetDrawPosStep = 0;
//				return false;
//				}
//
//			++plocBegin->x;
//			++plocEnd->y;
//
//			// �´�������Z��
//			m_nGetDrawPosStep = 1;
//			}
//			break;
//		}
//
//	return true;
//	} // end __GetDrawPos
