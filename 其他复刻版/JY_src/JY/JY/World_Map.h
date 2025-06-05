//-----------------------------------------------------------------------------
// �����ͼ����
//-----------------------------------------------------------------------------

#ifndef WORLD_MAP
#define WORLD_MAP

// INCLUDES ///////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <assert.h>

#include "Game_Public.h"
#include "RLE.h"

// DEFINES ////////////////////////////////////////////////////////////////////
// MACROS /////////////////////////////////////////////////////////////////////
// TYPES //////////////////////////////////////////////////////////////////////
// CLASS //////////////////////////////////////////////////////////////////////

// �����ͼ����
class CWorldMap
	{
	public:
		CWorldMap();
		~CWorldMap();

		int Init();
		// ����Ļ����ʾ�����ͼ
		int Show(LOC *plocCenter);

		// ��ȡ�������ֵ
		const LOC *locMax() {return &m_locMax;}

	private:
		// ����Ҫ���Ƶ�һ��ͼ������ͼ����յ�ͼ���λ��
		bool __GetDrawPos(LOC *plocBegin, LOC *plocEnd);


		Mem_RLE  m_mrMmap;						// �����ͼͼ����Ϣ
		Mem_File m_mfCol;						// ��ɫ����Ϣ
		Mem_File m_mfEarth;						// �����ͼ����ļ���Ϣ
		Mem_File m_mfSurface;
		Mem_File m_mfBuilding;
		Mem_File m_mfBuildx;
		Mem_File m_mfBuildy;

		LOC m_locMax;							// �������ֵ

		LOC m_locTopRight;						// ��������ƫ��ֵ
		LOC m_locTopLeft;						// ��������ƫ��ֵ
		LOC m_locBottomRight;					// ��������ƫ��ֵ
		LOC m_locBottomLeft;					// ��������ƫ��ֵ
		LOC m_locRightTop;						// ��������ƫ��ֵ
		LOC m_locRightBottom;					// ��������ƫ��ֵ
		LOC m_locLeftTop;						// ��������ƫ��ֵ
		LOC m_locLeftBottom;					// ��������ƫ��ֵ

		int m_nGetDrawPosStep;					// ��ȡ����λ�õĲ�����
	};

// PROTOTYPES /////////////////////////////////////////////////////////////////
// EXTERNALS //////////////////////////////////////////////////////////////////
// GLOBALS ////////////////////////////////////////////////////////////////////
// FUNCTIONS //////////////////////////////////////////////////////////////////

#endif // WORLD_MAP