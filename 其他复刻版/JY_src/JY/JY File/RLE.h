//-----------------------------------------------------------------------------
// ��ӹȺ����ʹ�õ�RLE���г̳���ѹ�����룩ͼ���ļ���ز���
// ������8λ���ز���
//-----------------------------------------------------------------------------

#ifndef JY_RLE
#define JY_RLE

// INCLUDES ///////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <assert.h>

#include "FH_File.h"

// DEFINES ////////////////////////////////////////////////////////////////////
// MACROS /////////////////////////////////////////////////////////////////////
// TYPES //////////////////////////////////////////////////////////////////////
// CLASS //////////////////////////////////////////////////////////////////////

// �ڴ��е�RLEͼ���ļ���Ϣ
typedef struct _Mem_RLE
	{
	Mem_File Grp;	// ͼ�������ļ���Ϣ
	Mem_File Idx;	// ͼ�������ļ���Ϣ
	Mem_File Col;	// ��ɫ���ļ���Ϣ
	DWORD nImages;	// ͼ����Ŀ����Ϊ�õ���2�ֽڵ���������ͼ����Ŀ���ܳ���65536����
	}Mem_RLE;

// ����RLEͼ����Ϣ
typedef struct _RLE_Image
	{
	short nWidth;		// ͼ���ȣ����з���������Ϊ����ļ�������Ժ����׵ļ���������������������أ�
	short nHeight;		// ͼ��߶�
	short nXCorrection;	// ͼ��X����������������и���
	short nYCorrection;	// ͼ��Y����
	}RLE_Image;

// PROTOTYPES /////////////////////////////////////////////////////////////////

inline void Set_Trans_Color(COLORREF color);
inline void Attach_COL(Mem_RLE *p_rle, const Mem_File *p_col);
inline void Unload_RLE(Mem_RLE *p_rle);
inline void Unload_COL(Mem_File *p_col);
int Load_RLE(Mem_RLE *p_rle, const char *filename_grp, const char *filename_idx);
int Load_COL(Mem_File *p_col, const char *filename_col);
int RleWriteLine32(DWORD *dst, BYTE *src, short width, BYTE *p_col);
int RleWriteLine8(BYTE *dst, BYTE *src, short width);

// EXTERNALS //////////////////////////////////////////////////////////////////
// GLOBALS ////////////////////////////////////////////////////////////////////
// FUNCTIONS //////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: Set_Trans_Color()
// Desc: ����͸��ɫ
//-----------------------------------------------------------------------------
inline void Set_Trans_Color(COLORREF color)
	{
	extern COLORREF Trans_Color;		// ���ⲿ�����������Trans_Color��RLE.cpp��
	Trans_Color = color;
	}



//-----------------------------------------------------------------------------
// Name: Attach_COL()
// Desc: �ѵ�ɫ����Ϣ���ӵ�ͼ��������Ϣ��
//-----------------------------------------------------------------------------
inline void Attach_COL(Mem_RLE * const p_rle, const Mem_File * const p_col)
	{
	assert(p_rle && p_col);

	p_rle->Col = *p_col;
	}



//-----------------------------------------------------------------------------
// Name: Unload_RLE()
// Desc: ж��RLEͼ���ļ�����
//       �ͷ�.GRP, .IDX�ڴ棬��ɫ����Ϊ��ר�ŵ�ж�غ��������ﲻ�ù�
//-----------------------------------------------------------------------------
inline void Unload_RLE(Mem_RLE * const p_rle)
	{
	assert(p_rle);

	UnloadFile(&p_rle->Grp);
	UnloadFile(&p_rle->Idx);
	}



//-----------------------------------------------------------------------------
// Name: Unload_COL()
// Desc: ж��.COL��ɫ���ļ�����
//-----------------------------------------------------------------------------
inline void Unload_COL(Mem_File * const p_col)
	{
	UnloadFile(p_col);
	}


#endif // JY_RLE