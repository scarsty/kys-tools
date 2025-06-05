//-----------------------------------------------------------------------------
// ��Ϸ����
//-----------------------------------------------------------------------------

#ifndef GAME
#define GAME

// INCLUDES ///////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <assert.h>

#include "FH_Bit.h"
#include "game_window.h"

#include "Game_Public.h"
#include "Game_Time.h"
#include "Player.h"
#include "World_Map.h"

// DEFINES ////////////////////////////////////////////////////////////////////

// ��Ϸ״̬
#define GM_PAUSED				(1 << 0)		// ��Ϸ����ͣ
#define GM_CLOSING				(1 << 1)		// ��Ϸ���ڹر�

// MACROS /////////////////////////////////////////////////////////////////////
// TYPES //////////////////////////////////////////////////////////////////////
// CLASS //////////////////////////////////////////////////////////////////////

// ��Ϸ����
class CGame
	{
	public:
		CGame(CGameWindow *piGameWnd);
		~CGame();

		int Init();
		// ��Ϸ��ģ��
		int Main();
		// ��ͣ��Ϸ
		int Pause();
		// ��Ϸ����ͣ�лָ�
		int Unpause();
		// ������Ϸ����
		void Save() {}

		// �����Ϸ״̬
		DWORD Check(DWORD bits) {return CHECK_BIT(m_nState, bits);}

	private:
		// ��������
		int __ProcessInput();
		// ��ʾͼ��
		int __Display();


		DWORD m_nState;							// ��Ϸ״̬
		CGameWindow * const m_piGameWnd;		// ���ڴ��ڹ�����ʵ����ַ
		CGameTime m_Time;						// ��Ϸʱ�����
		CPlayer m_Player;						// ��Ϸ�߶���
		CWorldMap m_WorldMap;					// �����ͼ����

		int m_nPressLength;						// ������������ʱ�䣬�������ʱ�������һ�ΰ�������

#ifdef DEBUG
		LONGLONG m_nCpuCount;					// ��һ�δ��������ѵģ�CPU����
		LONGLONG m_nCpuCountSum;				// �ۼƻ�ȡ��CPU����
		LONGLONG m_nCpuCountLastCalc;			// �ϴμ���ʱ���ϵ��ʱ��
#endif // DEBUG
	};

// PROTOTYPES /////////////////////////////////////////////////////////////////
// EXTERNALS //////////////////////////////////////////////////////////////////
// GLOBALS ////////////////////////////////////////////////////////////////////
// FUNCTIONS //////////////////////////////////////////////////////////////////

#endif // GAME