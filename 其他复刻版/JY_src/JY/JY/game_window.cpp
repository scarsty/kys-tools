
// INCLUDES ///////////////////////////////////////////////////////////////////

#include "game_window.h"
#include "FH_Log.h"
#include "config.h"
#include "Game.h"

// DEFINES ////////////////////////////////////////////////////////////////////
// MACROS /////////////////////////////////////////////////////////////////////
// TYPES //////////////////////////////////////////////////////////////////////
// CLASS //////////////////////////////////////////////////////////////////////
// PROTOTYPES /////////////////////////////////////////////////////////////////
// EXTERNALS //////////////////////////////////////////////////////////////////

extern CLog g_Log;								// ��־����
extern CConfig g_Cfg;							// �������ö���
extern HINSTANCE g_hInstance;					// ����ʵ�����

// GLOBALS ////////////////////////////////////////////////////////////////////

const char gcGAME_WND_CLASS_NAME[] = "GAME_WND";		// ��Ϸ��������
const char gcGAME_WND_TITLE[] = "��ӹȺ����";			// ��Ϸ���ڱ���

static bool sg_bWndClassReged = false;					// �Ƿ���ע�ᴰ����

// FUNCTIONS //////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: CGameWindow()
// Desc: ���캯��
//-----------------------------------------------------------------------------
CGameWindow::CGameWindow()
	{
	m_nState = 0;		// ��Ϊ���ڴ����ú�ᷢ���Ѽ������Ϣ����������Ҳû�м�����Բ���Ҫ����
	m_nClientWidth = g_Cfg.m_nFrameWidth;		// ���ÿͻ������
	m_nClientHeight = g_Cfg.m_nFrameHeight;
	}



//-----------------------------------------------------------------------------
// Name: ~CGameWindow()
// Desc: ��������
//-----------------------------------------------------------------------------
CGameWindow::~CGameWindow()
	{
	///////////////////////////////////////////////////////////
	// �ȴ�������Ϸ�����̴߳������߳̽�����Ȼ��ر���Щ�̵߳ľ��
	// ��ǰֻ������һ����Ϸ�߳�

	WaitForMultipleObjects(1, &m_hGameThread, true, INFINITE);

	CloseHandle(m_hGameThread);

	///////////////////////////////////////////////////////////
	}



//-----------------------------------------------------------------------------
// Name: Init()
// Desc: ��Ϸ���ڳ�ʼ��
//-----------------------------------------------------------------------------
int CGameWindow::Init()
	{
	///////////////////////////////////////////////////////////
	// �����δע�ᴰ���࣬�򴴽���ע��֮

	if (!sg_bWndClassReged)
		{
		if (__RegWndClass() != 0)
			{
			g_Log.Write("ע�ᴰ����ʧ��\r\n");
			return -1;
			}
		sg_bWndClassReged = true;
		}

	///////////////////////////////////////////////////////////
	// ������Ϸ����

	if (__CreateWnd() != 0)
		{
		g_Log.Write("������Ϸ����ʧ��\r\n");
		return -1;
		}

	///////////////////////////////////////////////////////////
	// ������ʾ�����󣬳�ʼ����Ϸ���������Ϣ
	// �յ�WM_CREATE��Ϣʱ���ڻ�δ��ʾ������ֻ�������λ��ִ��

	if (__InitWindowInfo() != 0)
		{
		g_Log.Write("��ʼ����Ϸ���������Ϣʧ��\r\n");
		return -1;
		}

	///////////////////////////////////////////////////////////
	// ������Ϸ�߳�
	// ��ʱ�����Ѿ���ʾ����ʼ�������Ѿ�û������

	if (__CreateGameThread() != 0)
		{
		g_Log.Write("������Ϸ�߳�ʧ��\r\n");
		return -1;
		}

	return 0;
	} // end Init



//-----------------------------------------------------------------------------
// Name: __RegWndClass()
// Desc: ������ע�ᴰ����
//-----------------------------------------------------------------------------
int CGameWindow::__RegWndClass()
	{
	// ��䴰����ṹ��
	WNDCLASSEX winclass;
	winclass.cbSize         = sizeof(WNDCLASSEX);
	winclass.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= __WndProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= sizeof(CGameWindow *);			// �����Ŵ��ڹ�����ʵ����ַ�Ŀռ�
	winclass.hInstance		= g_hInstance;
	winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL;
	winclass.lpszClassName	= gcGAME_WND_CLASS_NAME;
	winclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

	// ע�ᴰ����
	if (!RegisterClassEx(&winclass))
		return -1;

	return 0;
	} // end __RegWndClass



//-----------------------------------------------------------------------------
// Name: __CreateWnd()
// Desc: ��������
//-----------------------------------------------------------------------------
int CGameWindow::__CreateWnd()
	{
	///////////////////////////////////////////////////////////
	// �������ü��㴰������

	DWORD nWindowStyle;						// ������ʽ
	DWORD nWindowLeft, nWindowTop;			// �������ϱ�Եλ��
	DWORD nWindowWidth, nWindowHeight;		// ���ڿ�ߣ��Ϳͻ�����ͬ��

	// ȫ��ģʽ
	if (g_Cfg.m_bFullScreen)
		{
		nWindowStyle = WS_POPUP | WS_VISIBLE;
		nWindowLeft = 0;
		nWindowTop  = 0;
		nWindowWidth  = m_nClientWidth;
		nWindowHeight = m_nClientHeight;
		}
	// ����ģʽ
	else
		{
		nWindowStyle = WS_CAPTION | WS_VISIBLE;
		nWindowLeft = CW_USEDEFAULT;
		nWindowTop  = CW_USEDEFAULT;

		// �����û�����С����������ؼ��ͱ�Ե�󴰿ڵĴ�С
		// ע�ⲻ�ܼ���WS_OVERLAPPED��ʽ�������Լ���ͬ����ϸ�߿��������WS_CAPTION
		RECT rectWindow = {0, 0, m_nClientWidth, m_nClientHeight};
		if (AdjustWindowRect(&rectWindow, nWindowStyle, false) == 0)
			return -1;

		nWindowWidth  = rectWindow.right - rectWindow.left;
		nWindowHeight = rectWindow.bottom - rectWindow.top;
		}

	///////////////////////////////////////////////////////////
	// ��������
	// ˳��ѵ�ǰ���ڹ�����ʵ����ַ��WM_CREATE��Ϣ������Ϣ��������

	m_hWnd = CreateWindowEx(0,									// ������չ��ʽ
							gcGAME_WND_CLASS_NAME,				// ��������
							gcGAME_WND_TITLE,					// ������
							nWindowStyle,						// ������ʽ
							nWindowLeft, nWindowTop,			// �������Ͻ�λ��
							nWindowWidth, nWindowHeight,		// ���ڿ��
							NULL,								// �����ھ��
							NULL,								// �˵����
							g_hInstance,						// ����ʵ�����
							this);								// ��ѡ����������WM_CREATE��Ϣ�н���
	if (m_hWnd == NULL)
		return -2;

	return 0;
	} // end __CreateWnd



//-----------------------------------------------------------------------------
// Name: __InitWindowInfo()
// Desc: ��ʼ����Ϸ���������Ϣ
//-----------------------------------------------------------------------------
int CGameWindow::__InitWindowInfo()
	{
	///////////////////////////////////////////////////////////
	// ����û���λ��

	// �����û������Ͻ�����Ļ�ϵ�λ��
	POINT client_point = {0, 0};		// �û������Ͻ�������Լ�������
	if ( ClientToScreen(m_hWnd, &client_point) == 0 )
		return -2;

	m_rectClient.left   = client_point.x;
	m_rectClient.top    = client_point.y;
	m_rectClient.right  = m_rectClient.left + m_nClientWidth;
	m_rectClient.bottom = m_rectClient.top  + m_nClientHeight;

	///////////////////////////////////////////////////////////
	// ��ô����ı����û����ıߵĲ�ֵ

	// ��ô�������λ��
	RECT window_rect;
	if ( GetWindowRect(m_hWnd, &window_rect) == 0 )
		return -3;

	m_rectAdjusted.left		= window_rect.left - m_rectClient.left;
	m_rectAdjusted.top		= window_rect.top - m_rectClient.top;
	m_rectAdjusted.right	= window_rect.right - m_rectClient.right;
	m_rectAdjusted.bottom	= window_rect.bottom - m_rectClient.bottom;

	return 0;
	} // end __InitWindowInfo



//-----------------------------------------------------------------------------
// Name: __CreateGameThread()
// Desc: ������Ϸ�߳�
//-----------------------------------------------------------------------------
int CGameWindow::__CreateGameThread()
	{
	// �����̣߳����̴߳��������ݵ�ǰ���ڹ�����ʵ����ַ
	m_hGameThread = CreateThread(NULL, 0, &__GameThreadProc, this, 0, NULL);
	if (m_hGameThread == NULL)
		{
		g_Log.Write("������Ϸ�߳�ʧ��\r\n");
		return -1;
		}

	return 0;
	} // end __CreateGameThread



//-----------------------------------------------------------------------------
// Name: __GameThreadProc()
// Desc: ��Ϸ�̴߳�����
//-----------------------------------------------------------------------------
DWORD WINAPI CGameWindow::__GameThreadProc(LPVOID lpParameter)
	{
	assert(lpParameter);

	// ���յ�ǰ���ڹ�����ʵ����ַ
	CGameWindow * const piGameWnd = reinterpret_cast< CGameWindow * >(lpParameter);

	// ������Ϸ����
	CGame Game(piGameWnd);

	///////////////////////////////////////////////////////////
	// ��Ϸ��ʼ�������ʧ����������Ϸ�̲߳��رյ�ǰ��Ϸ����

	if (Game.Init() != 0)
		{
		g_Log.Write("��Ϸ��ʼ��ʧ��\r\n");

		// ��ǰ��Ϸ���ڷ�����Ϣ���رոô���
		if (PostMessage(piGameWnd->hWnd(), WM_CLOSE, 0, 0) == 0)
			{
			g_Log.Write("__GameThreadProc()����ǰ��Ϸ���ڷ��͹رմ�����Ϣʧ��\r\n");
			return -1;
			}

		return -1;
		}

	///////////////////////////////////////////////////////////
	// ��Ϸ��ѭ��
	// ��Ϸ/��Ϸ���ڻ�û������Ϊ�ر�״̬ʱһֱѭ��

	while ( !piGameWnd->Check(GW_CLOSING) )
		{
		// ������ڱ���С��������Ϸû����ͣ������Ϸ��ͣ
		if ( piGameWnd->Check(GW_MINIMIZED) && !Game.Check(GM_PAUSED) )
			Game.Pause();
		// �������û�б���С��������Ϸ��ͣ�ˣ���ָ�����
		else if ( !piGameWnd->Check(GW_MINIMIZED) && Game.Check(GM_PAUSED) )
			Game.Unpause();

		// ��Ϸû����ͣ/���ڹر�
		if ( !Game.Check(GM_PAUSED | GM_CLOSING) )
			{
			// ��Ϸ��ģ�飬����һ֡�ڵ����д���
			if (Game.Main() != 0)
				{
				g_Log.Write("��Ϸ��ģ�����\r\n");

				// ��ǰ��Ϸ���ڷ�����Ϣ���رոô���
				if (PostMessage(piGameWnd->hWnd(), WM_CLOSE, 0, 0) == 0)
					{
					g_Log.Write("__GameThreadProc()����ǰ��Ϸ���ڷ��͹رմ�����Ϣʧ��\r\n");
					return -1;
					}

				return -1;
				}
			}
		// ��Ϸ����ͣ����Ϣһ���
		else if (Game.Check(GM_PAUSED))
			{
			Sleep(100);
			}
		// ��Ϸ���ڹرգ�������Ϸ����û�����ڹر�
		else
			{
			// ��ǰ��Ϸ���ڷ�����Ϣ���رոô���
			if (PostMessage(piGameWnd->hWnd(), WM_CLOSE, 0, 0) == 0)
				{
				g_Log.Write("__GameThreadProc()����ǰ��Ϸ���ڷ��͹رմ�����Ϣʧ��\r\n");
				return -1;
				}

			// ������Ϸ��ѭ��
			break;
			}
		}

	return 0;
	} // end __GameThreadProc



//-----------------------------------------------------------------------------
// Name: EventLoop()
// Desc: ��Ϣѭ��
//-----------------------------------------------------------------------------
WPARAM CGameWindow::EventLoop()
	{
	MSG msg;

	// ���ո��̵߳�������Ϣ����Ϣ�յ���ŷ���
	// �õ�WM_QUITʱ��������0������ʱ����-1��������Ƿ����
	while(GetMessage(&msg, NULL, 0, 0) != 0)
		DispatchMessage(&msg);

	return msg.wParam;
	} // end EventLoop



//-----------------------------------------------------------------------------
// Name: __WndProc()
// Desc: ������Ϣ������
//-----------------------------------------------------------------------------
LRESULT CALLBACK CGameWindow::__WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
	switch(msg)
		{
		// �����Ѵ�������δ��ʾ
		// lparamָ��Ľṹ�����д�������ʱ���ӵ�����
		case WM_CREATE:
			{
			// ��ȡ���ڹ�����ʵ����ַ
			CREATESTRUCT *lpCS = reinterpret_cast< LPCREATESTRUCT >(lparam);
			CGameWindow *piGameWnd = reinterpret_cast< CGameWindow * >(lpCS->lpCreateParams);

			// �Ѵ��ڹ�����ʵ����ַ�ӵ�ϵͳ����ʵ������ռ䣬�Ժ�ͨ�����ھ����ȡ
			// �����Ƿ�ɹ�����Ҫ���㹦���������ǰ��LastError��־��Ȼ�����SetWindowLongPtr
			// �������0��LastError��Ϊ0����û�гɹ����أ�����ɹ�
			SetLastError(0);
			LONG_PTR nRet = SetWindowLongPtr(hwnd, 0, reinterpret_cast<LONG_PTR>(piGameWnd));
			if (nRet == 0 && GetLastError() != 0)
				{
				g_Log.Write("�Ѵ��ڹ�����ʵ����ַ�ӵ�ϵͳ����ʵ������ռ�ʱʧ��\r\n");
				return -1;
				}

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

		// ���ڱ��ƶ�����д�û���λ��
		// lparam�����û��������Ͻ���Ļ����
		case WM_MOVE:
			{
			// ��ȡ��ǰ���ڹ�����ʵ����ַ��ʧ��ʱ�Ĵ������ں��������ã�
			CGameWindow *piGameWnd = __piGameWnd(hwnd);
			if (piGameWnd == NULL) return -1;

			piGameWnd->m_rectClient.left	= (int)(short) LOWORD(lparam);
			piGameWnd->m_rectClient.top		= (int)(short) HIWORD(lparam);
			piGameWnd->m_rectClient.right	= piGameWnd->m_rectClient.left + piGameWnd->m_nClientWidth;
			piGameWnd->m_rectClient.bottom	= piGameWnd->m_rectClient.top  + piGameWnd->m_nClientHeight;
			return 0;
			}

		// ���ڴ�С���ı�
		// wparam���Ǳ仯���ͣ�lparam�����û����Ŀ��
		case WM_SIZE:
			{
			// ��ȡ��ǰ���ڹ�����ʵ����ַ��ʧ��ʱ�Ĵ������ں��������ã�
			CGameWindow *piGameWnd = __piGameWnd(hwnd);
			if (piGameWnd == NULL) return -1;

			switch (wparam)
				{
				// ������С��
				//*********
				case SIZE_MINIMIZED:
					{
					g_Log.Write("Window: %s -> %s\r\n",
						piGameWnd->Check(GW_MINIMIZED) ? "min" : "normal", "Min");
					SET_BIT(piGameWnd->m_nState, GW_MINIMIZED);
					break;
					}
				// ����ͨ����ʽ�Ĵ�С�ı䣨����С������󻯣�
				// ���ܵ����Ϊ���϶����ڱ�Ե�ı��˴�С������󻯻���С���ָ�*********
				// ������ֻ���ܳ��֡�����С���ָ���������*********
				case SIZE_RESTORED:
					{
					g_Log.Write("Window: %s -> %s\r\n",
						piGameWnd->Check(GW_MINIMIZED) ? "min" : "normal", "Normal");
					RESET_BIT(piGameWnd->m_nState, GW_MINIMIZED);
					break;
					}
				}
			return 0;
			}

		// ���ڱ�����򱻽���
		// wparam��ʾ�����Ǳ�������Ǳ�������lparam������һ������״̬���ڸı�Ĵ��ڵ��߳�ID
		case WM_ACTIVATEAPP:
			{
			// ��ȡ��ǰ���ڹ�����ʵ����ַ��ʧ��ʱ�Ĵ������ں��������ã�
			CGameWindow *piGameWnd = __piGameWnd(hwnd);
			if (piGameWnd == NULL) return -1;

			// ���ڱ�������ڱ�����ʱҲ���յ�����Ϣ��
			if (wparam)
				SET_BIT(piGameWnd->m_nState, GW_ACTIVATED);
			// ���ڱ����������ڱ��ر�ʱҲ���յ�����Ϣ��
			else
				RESET_BIT(piGameWnd->m_nState, GW_ACTIVATED);
			return 0;
			}

		// ׼���رմ���
		case WM_CLOSE:
			{
			// ��ȡ��ǰ���ڹ�����ʵ����ַ��ʧ��ʱ�Ĵ������ں��������ã�
			CGameWindow *piGameWnd = __piGameWnd(hwnd);
			if (piGameWnd == NULL) return -1;

			SET_BIT(piGameWnd->m_nState, GW_CLOSING);
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
	} // end __WinProc



//-----------------------------------------------------------------------------
// Name: __piGameWnd()
// Desc: ���ݴ��ھ���ҵ���ǰ���ڹ�����ʵ����ַ���õ�ַ����ϵͳ����ʵ���Ķ���ռ���
//       ������Ϣ�������У��ɹ����ص�ַ��ʧ�ܷ���0
//-----------------------------------------------------------------------------
inline CGameWindow *CGameWindow::__piGameWnd(HWND const hWnd)
	{
	CGameWindow *piGameWnd = reinterpret_cast< CGameWindow * >( GetWindowLongPtr(hWnd, 0) );

	// ����޷���ȡ��ǰ���ڹ�����ʵ����ַ����رմ���
	if (piGameWnd == NULL)
		{
		g_Log.Write("��ȡ��ǰ���ڹ�����ʵ����ַʧ��\r\n");
		PostMessage(hWnd, WM_CLOSE, 0, 0);
		return 0;
		}

	return piGameWnd;
	} // end __piGameWnd
