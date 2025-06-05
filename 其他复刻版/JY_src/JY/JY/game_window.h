//-----------------------------------------------------------------------------
// ��Ϸ���ڶ���
//-----------------------------------------------------------------------------

#ifndef GAME_WINDOW
#define GAME_WINDOW

// INCLUDES ///////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <assert.h>

#include "FH_Bit.h"

// DEFINES ////////////////////////////////////////////////////////////////////

// ����״̬
#define GW_ACTIVATED		(1 << 0)		// ���ڱ�����
#define GW_MINIMIZED		(1 << 1)		// ���ڱ���С����ȫ��ʱ�л������棩
#define GW_CLOSING			(1 << 2)		// �������ڱ��ر�

// MACROS /////////////////////////////////////////////////////////////////////
// TYPES //////////////////////////////////////////////////////////////////////
// CLASS //////////////////////////////////////////////////////////////////////

class CGameWindow
	{
	public:
		CGameWindow();
		~CGameWindow();

		int Init();
		// ���ڵ��¼�ѭ��
		static WPARAM EventLoop();

		// ��鴰��״̬
		DWORD Check(DWORD bits) {return CHECK_BIT(m_nState, bits);}
		// ��ȡ���ھ��
		HWND hWnd() {return m_hWnd;}
		// ��ȡ�ͻ���λ��
		const RECT *rectClient() {return &m_rectClient;}
		// ��ȡ�ͻ������
		int ClientWidth()  {return m_nClientWidth;}
		// ��ȡ�ͻ����߶�
		int ClientHeight() {return m_nClientHeight;}

	private:
		// ע�ᴰ����
		static int __RegWndClass();
		// ��������
		int __CreateWnd();
		// ��ʼ�����������Ϣ
		int __InitWindowInfo();
		// ������Ϸ�߳�
		int __CreateGameThread();
		// ��Ϸ�̴߳�����
		static DWORD WINAPI __GameThreadProc(LPVOID lpParameter);
		// ������Ϣ������
		static LRESULT CALLBACK __WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		// ��ȡ��ǰ���ڹ�����ʵ����ַ��������Ϣ�������У�
		static CGameWindow *__piGameWnd(HWND hWnd);


		HWND	m_hWnd;							// ���ھ��
		DWORD	m_nState;						// ����״̬��λ�������
		RECT	m_rectClient;					// ����ģʽ�µĿͻ���λ��
		int		m_nClientWidth;					// ����ģʽ�µĿͻ�����ߣ�ֻ�����޸�����ʱ�䶯
		int		m_nClientHeight;
		RECT	m_rectAdjusted;					// �����ı����û����ıߵĲ�ֵ������Ϊ�������ʹ�����ʽ�й�
		HANDLE	m_hGameThread;					// ��Ϸ�߳̾��
	};

// PROTOTYPES /////////////////////////////////////////////////////////////////
// EXTERNALS //////////////////////////////////////////////////////////////////
// GLOBALS ////////////////////////////////////////////////////////////////////
// FUNCTIONS //////////////////////////////////////////////////////////////////

#endif // GAME_WINDOW