
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

#include "Game.h"
#include "Image_Manager.h"

// DEFINES ////////////////////////////////////////////////////////////////////
// MACROS /////////////////////////////////////////////////////////////////////
// TYPES //////////////////////////////////////////////////////////////////////
// CLASS //////////////////////////////////////////////////////////////////////
// PROTOTYPES /////////////////////////////////////////////////////////////////
// EXTERNALS //////////////////////////////////////////////////////////////////

extern HINSTANCE g_hInstance;					// ����ʵ�����

// GLOBALS ////////////////////////////////////////////////////////////////////
// FUNCTIONS //////////////////////////////////////////////////////////////////

#include "Process_Input.h"
#include "Display.h"

//-----------------------------------------------------------------------------
// Name: CGame
// Desc: ������Ϸ����
//-----------------------------------------------------------------------------
CGame::CGame(CGameWindow * const piGameWnd) 
	: m_nState(0), m_piGameWnd(piGameWnd), m_nPressLength(0)
	{
#ifdef DEBUG
	m_nCpuCount = 0;
	m_nCpuCountSum = 0;
	m_nCpuCountLastCalc = 0;
#endif // DEBUG
	}



//-----------------------------------------------------------------------------
// Name: ~CGame
// Desc: ������Ϸ����
//-----------------------------------------------------------------------------
CGame::~CGame()
	{
	///////////////////////////////////////////////////////////
	// ������Ϸ����

	Save();

	///////////////////////////////////////////////////////////
	// �ͷ�DirectX����

	// DirectSound
	DSound_Shutdown();
	// DirectInput
	DInput_Shutdown();
	// DirectDraw
	DDraw_Shutdown();

	///////////////////////////////////////////////////////////
	}



//-----------------------------------------------------------------------------
// Name: Init()
// Desc: ��ʼ����Ϸ
//-----------------------------------------------------------------------------
int CGame::Init()
	{
	///////////////////////////////////////////////////////////
	// ��ʼ�������ͼ����

	int result = m_WorldMap.Init();
	if (result != 0)
		{
		g_Log.Write("��ʼ�������ͼ����ʧ��\r\n");
		return -1;
		}

	///////////////////////////////////////////////////////////
	// ��ʼ��DirectDraw

	if (DDraw_Init(m_piGameWnd->hWnd(), g_Cfg.m_nFrameWidth, g_Cfg.m_nFrameHeight, 
				   g_Cfg.m_nFrameBpp, !g_Cfg.m_bFullScreen) != 1)
		{
		g_Log.Write("DDraw_Init() failed.\r\n");
		return -1;
		}

	///////////////////////////////////////////////////////////
	// ��ʼ��DirectInput

	if (DInput_Init(g_hInstance) != 1)
		{
		g_Log.Write("DInput_Init() failed.\r\n");
		return -1;
		}

	if (DInput_Init_Keyboard(m_piGameWnd->hWnd()) != 1)
		{
		g_Log.Write("DInput_Init_Keyboard() failed.\r\n");
		return -1;
		}

	if (DInput_Init_Mouse(m_piGameWnd->hWnd()) != 1)
		{
		g_Log.Write("DInput_Init_Mouse() failed.\r\n");
		return -1;
		}

	///////////////////////////////////////////////////////////
	// ��ʼ��DirectSound

	if (DSound_Init(m_piGameWnd->hWnd()) != 1)
		{
		g_Log.Write("DSound_Init() failed.\r\n");
		return -1;
		}

	///////////////////////////////////////////////////////////
	// ��ʼ����Ϸʱ��

	if (m_Time.Init(g_Cfg.m_nFrameRate) != 0)
		{
		g_Log.Write("��Ϸʱ���ʼ��ʧ��\r\n");
		return -1;
		}

	return 0;
	} // end Init



//-----------------------------------------------------------------------------
// Name: Main()
// Desc: ��Ϸ��ģ�飬ÿִ֡��һ��
//-----------------------------------------------------------------------------
int CGame::Main()
	{
	///////////////////////////////////////////////////////////
	// ��������

	if (__ProcessInput() != 0)
		{
		g_Log.Write("process_input() failed.\r\n");
		return -1;
		}

	///////////////////////////////////////////////////////////
	// ����ͼ��

	// �����Ҫ��֡���򲻻���ͼ��
	if (m_Time.ShouldDropThisFrame())
		m_Time.AnnounceDropThisFrame();
	else
		{
		if (__Display() != 0)
			{
			g_Log.Write("display() failed.\r\n");
			return -1;
			}
		}

	///////////////////////////////////////////////////////////
	// ʱ�䴦��

	// ��Ϸ�ڹ涨ʱ��ת����һ֡
	if (m_Time.Pass() != 0)
		{
		g_Log.Write("��Ϸ�ڹ涨ʱ��ת����һ֡ʧ��\r\n");
		return -1;
		}

	return 0;
	} // end Main



//-----------------------------------------------------------------------------
// Name: Pause()
// Desc: ��Ϸ��ͣ��������С��ʱ��ϷҲ������ͣ״̬��
//-----------------------------------------------------------------------------
int CGame::Pause()
	{
	// ��ͣ��������
	DSound_Pause_All_Sounds();

	// ������Ϸ״̬Ϊ��ͣ
	SET_BIT(m_nState, GM_PAUSED);

	return 0;
	} // end Pause



//-----------------------------------------------------------------------------
// Name: Unpause()
// Desc: ��Ϸ����ͣ״̬�ָ�
//-----------------------------------------------------------------------------
int CGame::Unpause()
	{
	// ȡ����ͣ��������
	DSound_Unpause_All_Sounds();

	// �ָ���Ϸʱ��
	m_Time.Restore();

	// �����Ϸ����ͣ״̬
	RESET_BIT(m_nState, GM_PAUSED);

	return 0;
	} // end Unpause

