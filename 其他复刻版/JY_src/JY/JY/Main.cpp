//-----------------------------------------------------------------------------
// Win32��ǣ��������ڵ�
//-----------------------------------------------------------------------------

// INCLUDES ///////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <assert.h>

#include "FH_File.h"
#include "FH_Log.h"
#include "config.h"
#include "game_window.h"

// DEFINES ////////////////////////////////////////////////////////////////////
// MACROS /////////////////////////////////////////////////////////////////////
// TYPES //////////////////////////////////////////////////////////////////////
// CLASS //////////////////////////////////////////////////////////////////////
// PROTOTYPES /////////////////////////////////////////////////////////////////

int Init();
void Shutdown();
int RegMainWndClass();
int CreateMainWnd();
WPARAM MainWndEventLoop();
int CreateGameWndThread();
DWORD WINAPI GameWndThreadProc(LPVOID lpParameter);
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
void CloseGameWndThreadHandleByIndex(int nThreadIndex);

// EXTERNALS //////////////////////////////////////////////////////////////////
// GLOBALS ////////////////////////////////////////////////////////////////////

HINSTANCE g_hInstance = NULL;							// ����ʵ�����
static HWND sg_hMainWnd;								// �����ھ��

CConfig g_Cfg;											// �������ö���
CLog g_Log;												// ��־����

const char gcMAIN_WND_CLASS_NAME[] = "WIN_GAME_MAIN";	// ����������
const char gcMAIN_WND_TITLE[] = "JY_MAIN";				// �����ڱ���
const char gcMUTEX_NAME[] = "FH_JY_GAME";				// ����������������֤ͬһʱ��ֻ��һ��������ʵ��

const UINT nWM_CREATE_GAME_WND = WM_APP + 123;			// ������Ҫ����ġ������µ���Ϸ���̵���Ϣ
const WPARAM nWPARAM_CREATE_GAME_WND = 456;				// ����Ϣ�Ĳ���wparam��Ϊ��ֹ������Ϣ����Ϊ0��
const LPARAM nLPARAM_CREATE_GAME_WND = 789;				// ����Ϣ�Ĳ���lparam

const DWORD nMAX_GAME_WND_THREAD = 16;					// ����ͬʱ��������Ϸ�����߳���Ŀ���ޣ����32��
static DWORD sg_nGameWndThread = 0;								// �ѿ�������Ϸ�����߳���Ŀ
static HANDLE sg_ghGameWndThread[nMAX_GAME_WND_THREAD] = {0};	// ��Ϸ�����̵߳ľ���б�

// FUNCTIONS //////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: �������
//       ����Ϣѭ��֮ǰ�˳�WinMainҪ����0
//-----------------------------------------------------------------------------
int WINAPI WinMain(	HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
	{
	// �������ʵ�����
	g_hInstance = hinstance;

	///////////////////////////////////////////////////////////
	// �����ʼ��

	int nRet = Init();
	if (nRet == -1)
		{
		// ϵͳ���Ѵ��ڱ������ʵ�������Ҵ�ʱ��־�ļ���δ��
		return 0;
		}
	else if (nRet != 0)
		{
		g_Log.Write("�����ʼ��ʧ��\r\n");
		return 0;
		}

	///////////////////////////////////////////////////////////
	// ������ע����������

	if (RegMainWndClass() != 0)
		{
		g_Log.Write("ע����������ʧ��\r\n");
		return 0;
		}

	///////////////////////////////////////////////////////////
	// ����������

	if (CreateMainWnd() != 0)
		{
		g_Log.Write("����������ʧ��\r\n");
		return 0;
		}

	///////////////////////////////////////////////////////////
	// ��������Ϣѭ��

	WPARAM wParamQuit = MainWndEventLoop();

	///////////////////////////////////////////////////////////
	// ���򷴳�ʼ��

	Shutdown();

	///////////////////////////////////////////////////////////
	// ���ز���ϵͳ

	return static_cast<int>(wParamQuit);
	} // end WinMain



//-----------------------------------------------------------------------------
// Name: Init()
// Desc: �����ʼ��
//       ����-1��ʾ�Ѵ���ͬ��������
//-----------------------------------------------------------------------------
int Init()
	{
	///////////////////////////////////////////////////////////
	// ����������
	// ����Ѵ��ڸû����壬���򱾳������һ��ʵ���������ڷ��ʹ����µ���Ϸ���̵���Ϣ
	// ����ж���Ѵ���ʵ������������ʱ�����ܣ�����ֻ���һ���ҵ��ķ�����Ϣ
	// ������������þ���ڽ�����ֹ����Զ��رգ�������������ر�

	CreateMutex(NULL, false, gcMUTEX_NAME);

	// �Ѵ��ڸû�����
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
		// Ѱ����һ��ʵ����������
		HWND hWnd = FindWindow(gcMAIN_WND_CLASS_NAME, gcMAIN_WND_TITLE);   
		if (hWnd == NULL)
			return -2;		// ���ܺ�������ʧ�ܣ�Ҳ���ܸ�ʵ����û����������

		// ����һ��ʵ���������ڷ��ʹ����µ���Ϸ���̵���Ϣ
		if (PostMessage(hWnd, nWM_CREATE_GAME_WND, nWPARAM_CREATE_GAME_WND, nLPARAM_CREATE_GAME_WND) == 0)
			return -3;

		return -1;
		}

	///////////////////////////////////////////////////////////
	// �ѵ�ǰ·������Ϊ��ǰִ���ļ�·��

	if (Set_Process_Dir_to_Exe_Dir() != 0)
		return -4;

	///////////////////////////////////////////////////////////
	// ��ʼ���ļ���¼

	if (g_Log.Init_File() != 0)
		return -5;

	// ����Ĭ�ϼ�¼��ʽ
	g_Log.Set(TAR_FILE, PRE_TIME_MS);

	///////////////////////////////////////////////////////////
	// �����������

	if (g_Cfg.Load() != 0)
		{
		g_Log.Write("��������ʧ��\r\n");
		return -6;
		}

	return 0;
	} // end Init



//-----------------------------------------------------------------------------
// Name: Shutdown()
// Desc: ���򷴳�ʼ��
//-----------------------------------------------------------------------------
void Shutdown()
	{
	// �ȴ�����Ϸ�������߳��˳�
	;

	// �رշ���Ϸ�������߳̾��
	;
	} // end Shutdown



//-----------------------------------------------------------------------------
// Name: RegMainWndClass()
// Desc: ������ע����������
//-----------------------------------------------------------------------------
int RegMainWndClass()
	{
	// ��䴰����ṹ��
	WNDCLASSEX winclass;
	winclass.cbSize         = sizeof(WNDCLASSEX);
	winclass.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= MainWndProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= g_hInstance;
	winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL;
	winclass.lpszClassName	= gcMAIN_WND_CLASS_NAME;
	winclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

	// ע�ᴰ����
	if (!RegisterClassEx(&winclass))
		return -1;

	return 0;
	} // end RegMainWndClass



//-----------------------------------------------------------------------------
// Name: CreateMainWnd()
// Desc: ����������
//       �����������ε�
//-----------------------------------------------------------------------------
int CreateMainWnd()
	{
	sg_hMainWnd = CreateWindowEx(0,			// ������չ��ʽ
		gcMAIN_WND_CLASS_NAME,				// ��������
		gcMAIN_WND_TITLE,					// ������
		WS_POPUP,							// ������ʽ
		0, 0,								// �������Ͻ�λ��
		0, 0,								// ���ڿ��
		NULL,								// �����ھ��
		NULL,								// �˵����
		g_hInstance,						// ����ʵ�����
		NULL);								// ��ѡ����������WM_CREATE��Ϣ�н���
	if (sg_hMainWnd == NULL)
		return -1;

	return 0;
	} // end CreateMainWnd



//-----------------------------------------------------------------------------
// Name: MainWndEventLoop()
// Desc: ��������Ϣѭ��
//       ���������̴߳������߳̽����������ڻᱻ�ر�
//-----------------------------------------------------------------------------
WPARAM MainWndEventLoop()
	{
	while (true)
		{
		// ����Ϣ�ʹ���ֱ����ʱû����Ϣ����
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
            if (msg.message == WM_QUIT)
                return msg.wParam;
			DispatchMessage(&msg);
			}

		// �ȴ����߳��˳����߳���Ϣ����
		DWORD result = MsgWaitForMultipleObjects(
			sg_nGameWndThread, sg_ghGameWndThread, false, INFINITE, QS_ALLINPUT);

		//-------------------------------------------**********
		if (sg_nGameWndThread == 0)
			g_Log.Write("MsgWaitForMultipleObjects()�ȴ��Ķ���Ϊ0, ����0x%X\r\n", result);
		//-------------------------------------------**********

		// ���߳���Ϣ����
		if (result == WAIT_OBJECT_0 + sg_nGameWndThread)
			{}
		// ĳ����Ϸ�����߳��˳�
		else if (result >= WAIT_OBJECT_0 && result < WAIT_OBJECT_0 + sg_nGameWndThread)
			{
			// ����Ϸ�����̱߳��
			int nThreadIndex = result - WAIT_OBJECT_0;

			// �رո���Ϸ�����̵߳ľ�������������б�
			CloseGameWndThreadHandleByIndex(nThreadIndex);

			// ��Ϸ�����߳�����һ
			--sg_nGameWndThread;

			// ������Ϸ�����̶߳����˳�
			if (sg_nGameWndThread == 0)
				{
				// ֪ͨ�������̹߳ر�
				;

				// �ر�������
				PostMessage(sg_hMainWnd, WM_CLOSE, 0, 0);
				}
			}
		// �������������ĳ�������Ǳ������Ļ����壬���ܺ�������ʧ��
		else
			{
			g_Log.Write("��������Ϣѭ����MsgWaitForMultipleObjects()����������0x%X\r\n", result);
			PostMessage(sg_hMainWnd, WM_CLOSE, 0, 0);
			}
		} // end while
	} // end MainWndEventLoop



//-----------------------------------------------------------------------------
// Name: CreateGameWndThread()
// Desc: ������Ϸ�����߳�
//-----------------------------------------------------------------------------
int CreateGameWndThread()
	{
	// ��Ϸ�����߳���Ŀ�Ѵﵽ����
	if (sg_nGameWndThread >= nMAX_GAME_WND_THREAD)
		{
		g_Log.Write("��Ϸ������Ŀ�Ѵ����ޣ�������Ϸ����ʧ��\r\n");
		return 0;
		}

	// ������Ϸ�����߳�
	HANDLE hGameWndThread = CreateThread(NULL, 0, &GameWndThreadProc, NULL, 0, NULL);
	if (hGameWndThread == NULL)
		{
		g_Log.Write("��Ϸ�����̴߳���ʧ��\r\n");
		return -1;
		}

	// ������Ϸ�����߳̾��
	sg_ghGameWndThread[sg_nGameWndThread] = hGameWndThread;

	// ������Ϸ�����߳���Ŀ
	++sg_nGameWndThread;

	return 0;
	} // end CreateGameWndThread



//-----------------------------------------------------------------------------
// Name: GameWndThreadProc()
// Desc: ��Ϸ�����̴߳�����
//-----------------------------------------------------------------------------
DWORD WINAPI GameWndThreadProc(LPVOID lpParameter)
	{
	// ������Ϸ���ڶ���
	CGameWindow GameWnd;

	// ��ʼ����Ϸ���ڶ���
	if (GameWnd.Init() != 0)
		{
		g_Log.Write("��Ϸ���ڶ����ʼ��ʧ��\r\n");
		return -1;
		}

	// ��Ϸ������Ϣѭ��
	CGameWindow::EventLoop();

	return 0;
	} // end GameWndThreadProc



//-----------------------------------------------------------------------------
// Name: MainWndProc()
// Desc: ��������Ϣ������
//-----------------------------------------------------------------------------
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
	switch(msg)
		{
		// �����Ѵ�������δ��ʾ
		case WM_CREATE:
			{
			// ������Ϸ�����̣߳�ʧ���򷵻س�ʼ�����ɹ�
			if (CreateGameWndThread() != 0)
				return -1;
			return 0;
			}

		case WM_PAINT:
			{
			// ʹ������Ч��û�����ǵĻ����ڻ�һֱ�յ�����Ϣ
			PAINTSTRUCT ps;
   			HDC hdc = BeginPaint(hwnd,&ps);
			EndPaint(hwnd,&ps);
			return 0;
			}

		// �����µ���Ϸ����
		case nWM_CREATE_GAME_WND:
			{
			// �������һ�£��򴴽���Ϸ����
			// �������ʧ�ܣ����������ڻ����Ϣ��¼����־�������ճ�����
			if (wparam == nWPARAM_CREATE_GAME_WND && lparam == nLPARAM_CREATE_GAME_WND)
				CreateGameWndThread();
			return 0;
			}

		// ׼���رմ���
		case WM_CLOSE:
			{
			DestroyWindow(hwnd);
			return 0;
			}

		// �����Ѿ�����Ļ��ʧ�����ڱ�����
		case WM_DESTROY:
			{
			PostQuitMessage(0);
			return 0;
			}

		default:
			{
			return DefWindowProc(hwnd, msg, wparam, lparam);
			}

		} // end switch (msg)

	return 0;
	} // end MainWndProc



//-----------------------------------------------------------------------------
// Name: CloseGameWndThreadHandleByIndex()
// Desc: ͨ�������ر���Ϸ�����̵߳ľ�������������б�
//       ÿ����Ϸ�����߳̾���ر�ʱ�������������������������������Ч
//-----------------------------------------------------------------------------
void CloseGameWndThreadHandleByIndex(int const nThreadIndex)
	{
	assert(nThreadIndex >= 0 && nThreadIndex <= nMAX_GAME_WND_THREAD -1);

	///////////////////////////////////////////////////////////
	// �رո���Ϸ�����̵߳ľ��

	if (CloseHandle(sg_ghGameWndThread[nThreadIndex]) == 0)
		g_Log.Write("�ر���Ϸ�����߳̾��ʧ��\r\n");
	sg_ghGameWndThread[nThreadIndex] = NULL;

	///////////////////////////////////////////////////////////
	// �������ھ���б��е�˳��
	// �ҵ����һ����Ч������������Ч����ڸչرյľ��֮�����������λ��
	// ���û���ҵ���Ч�������ֻ����������������չرյľ����������б���󣬻���������Ч���֮��
	// ��Щ������������

	// �����һ��λ����ǰ����Ч�����ֱ���չرյľ����λ��
	int i = nMAX_GAME_WND_THREAD -1;
	for (; i > nThreadIndex; --i)
		{
		// �ҵ���Ч���������λ�ú�����ѭ��
		if (sg_ghGameWndThread[i] != NULL)
			{
			sg_ghGameWndThread[nThreadIndex] = sg_ghGameWndThread[i];
			sg_ghGameWndThread[i] = NULL;
			break;
			}
		}
	} // end CloseGameWndThreadHandleByIndex
