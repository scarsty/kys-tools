//-----------------------------------------------------------------------------
// ͼ�������
//-----------------------------------------------------------------------------

#ifndef IMAGE_MANAGER
#define IMAGE_MANAGER

// INCLUDES ///////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#define INITGUID										// DirectX

#include <windows.h>
#include <stdio.h>
#include <assert.h>

#include <mmsystem.h>									// DirectX
#include <objbase.h>
#define DIRECTINPUT_VERSION 0x0800
#include <ddraw.h>
#include <dinput.h>
#include <dsound.h>
//#include <dmksctrl.h>
//#include <dmusici.h>
//#include <dmusicc.h>
//#include <dmusicf.h>

#include "dxlib_dd.h"									// DirectX User Lib
#include "dxlib_di.h"
#include "dxlib_ds.h"

#include "Game_Public.h"
#include "RLE.h"

// DEFINES ////////////////////////////////////////////////////////////////////
// MACROS /////////////////////////////////////////////////////////////////////
// TYPES //////////////////////////////////////////////////////////////////////
// CLASS //////////////////////////////////////////////////////////////////////

// ͼ�������Ϣ
typedef struct _SURFACE
	{
	LPDIRECTDRAWSURFACE7 pDDS;	// ͼ�����ָ��
	WORD nActive;				// ͼ����ԣ�0��ʾ�޻��ԣ�������Ļ�ڣ���Ϊ2��ʹ����Ļ�ڵ�ͼ�񱣳ֻ���
	WORD nIndex;				// ͼ������
	RLE_Image Image;			// ͼ����Ϣ
	} SURFACE;

// ͼ���ڴ������
// ��ʱ�޷�ʵ�ֶ��̹߳���һ��������
// ��Ϊ��δʵ���߳�ͬ���������߳̿���ͬʱ��ͬһ�ڴ�д�벻ͬ������*********
class CImageManager
	{
	public:
		CImageManager();
		~CImageManager();

		// ����ͼ������ֵ��ȡͼ�������Ϣ
		static int GetSurfaceInfo(const Mem_RLE *p_rle, WORD index, SURFACE **ppSurface);
		// �Ѹ�ͼ�����ڱ��漤��
		static void SurfaceActivate(SURFACE *pSurface, WORD nActive = 2);
		// ����ͼ�����ڱ������
		static void SurfaceDeactivate(SURFACE *pSurface);
		// ����ȫ������Ļ���
		static void SurfaceDeactivateAll();

		// ��ȡ�����ɱ�����
		static WORD MaxSurface() {return sm_nMaxSurface;}
		// ��ȡ�Ѵ���������
		static WORD SurfaceUsed() {return sm_nSurfaceUsed;}

	private:
		// ����ͼ�������ҵ������ŵı��
		// ������0��ʾ��ͼ��û�б��棬������1��Ӧ0�ű���
		// �����ŷ�Χ0x00-0xFFFF��0x01-0xFFFFΪ��Ч�����ţ��ܹ����԰���0xFFFF�����棨2�ֽڵ����ޣ�
		static WORD sm_gSurfaceIndex[0x10000];

		// �����ݱ������ҵ�������Ϣ�ı�񡱵��׸�������Ϣ�ĵ�ַ
		// �ϱ��еı�����n��Ӧ�ñ���е�n-1�ű�����Ϣ�����������0xFFFF�ű�����Ϣ
		static WORD sm_nMaxSurface;
		static SURFACE * const sm_pSurfaces;

		// �Ѵ����ı�����������������������õ�ǰ�������������ֵ�ǲ����С�ģ�
		static WORD sm_nSurfaceUsed;
	};

// PROTOTYPES /////////////////////////////////////////////////////////////////
// EXTERNALS //////////////////////////////////////////////////////////////////
// GLOBALS ////////////////////////////////////////////////////////////////////
// FUNCTIONS //////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: SurfaceActivate()
// Desc: ������Ļ����ʾ��ͼ��󣬣��Ѹ�ͼ�����ڱ��漤��
//       Ĭ��ֻά����Ļ�ڵ�ͼ��Ļ���
//       ��������£�ÿһ֡��Ļ�ڵ�ͼ��ȫ����ͬ����ô������Ļ�ڵı��������һ֡��
//       ���������Ҫ��С��������Ļ��ͼ������ͼ�񶼲�ͬ���ı�������
//       ������������µ�ʵ��һ�㶼����������б��棬����������һ����Ļ��ͼ������ͼ�񶼲�ͬ�������ˡ�
//-----------------------------------------------------------------------------
inline void CImageManager::SurfaceActivate(SURFACE * const pSurface, WORD const nActive)
	{
	pSurface->nActive = nActive;
	}



//-----------------------------------------------------------------------------
// Name: SurfaceDeactivate()
// Desc: ����ͼ�����ڱ�����ԣ�ÿ�μ�1
//-----------------------------------------------------------------------------
inline void CImageManager::SurfaceDeactivate(SURFACE * const pSurface)
	{
	if (pSurface->nActive > 0)
		--pSurface->nActive;
	}



//-----------------------------------------------------------------------------
// Name: SurfaceDeactivateAll()
// Desc: ��һ֡��ʼ��ʾͼ��ǰ��������ȫ������Ļ���
//-----------------------------------------------------------------------------
inline void CImageManager::SurfaceDeactivateAll()
	{
	for (int i = 0; i < sm_nSurfaceUsed; ++i)
		SurfaceDeactivate(&sm_pSurfaces[i]);
	}

#endif // IMAGE_MANAGER