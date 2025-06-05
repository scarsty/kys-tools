//-----------------------------------------------------------------------------
// Game.cpp - ��������
//-----------------------------------------------------------------------------

// INCLUDES ///////////////////////////////////////////////////////////////////
// DEFINES ////////////////////////////////////////////////////////////////////
// MACROS /////////////////////////////////////////////////////////////////////
// TYPES //////////////////////////////////////////////////////////////////////
// CLASS //////////////////////////////////////////////////////////////////////
// PROTOTYPES /////////////////////////////////////////////////////////////////
// EXTERNALS //////////////////////////////////////////////////////////////////
// GLOBALS ////////////////////////////////////////////////////////////////////
// FUNCTIONS //////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: __ProcessInput()
// Desc: ��������
//-----------------------------------------------------------------------------
int CGame::__ProcessInput()
	{
	///////////////////////////////////////////////////////////
	// ��ȡDirectInput����

	// ������ڴ��ڼ���״̬����ȡ�����豸����
	if ( m_piGameWnd->Check(GW_ACTIVATED) )
		{
		// ͨ���Ļ�ȡʧ�ܻ᷵�ؿ����ݣ������˳�
		if (DInput_Read_Keyboard() == -1)
			{
			// DIERR_INPUTLOST��ʧȥ���룬��Ҫ���»�ü���
			g_Log.Write("Lost keyboard input! That's impossible!\r\n");
			return -1;
			}

		// ͨ���Ļ�ȡʧ�ܻ᷵�ؿ����ݣ������˳�
		if (DInput_Read_Mouse() == -1)
			{
			// DIERR_INPUTLOST��ʧȥ���룬��Ҫ���»�ü���
			g_Log.Write("Lost mouse input! That's impossible!\r\n");
			return -1;
			}
		}
	// ������ڴ���δ����״̬������������豸����
	else
		{
		DInput_Keyboard_Reset_State();
		DInput_Mouse_Reset_State();
		}

	///////////////////////////////////////////////////////////
	// ����ESC���˳���Ϸ
	// ��ʱ����ִ���걾֡��Ȼ����뿪��ģ��

	if (DIKEY(DIK_ESCAPE))
		{
		// ������Ϸ״̬Ϊ�����˳�
		SET_BIT(m_nState, GM_CLOSING);
		return 0;
		}

	///////////////////////////////////////////////////////////
	// ȷ������
	// ���ֹ�������Ƶ����괦���н���

	if (m_nPressLength > 0)
		{
		--m_nPressLength;
		return 0;
		}

	///////////////////////////////////////////
	// �ı���Ļ���ĵ�ͼ����

	// ��õ�ͼ�������ֵ
	const LOC * const locWorldMapMax = m_WorldMap.locMax();
	
	// PageDown
	if (DIKEY(DIK_NEXT))
		{
		if (++m_Player.m_locWorld.x > locWorldMapMax->x)
			m_Player.m_locWorld.x = locWorldMapMax->x;
		}
	// PageUp
	else if (DIKEY(DIK_PRIOR))
		{
		if (--m_Player.m_locWorld.y < 0)
			m_Player.m_locWorld.y = 0;
		}
	// Home
	else if (DIKEY(DIK_HOME))
		{
		if (--m_Player.m_locWorld.x < 0)
			m_Player.m_locWorld.x = 0;
		}
	// End
	else if (DIKEY(DIK_END))
		{
		if (++m_Player.m_locWorld.y > locWorldMapMax->y)
			m_Player.m_locWorld.y = locWorldMapMax->y;
		}
	else
		return 0;

	// ���������Shift��������Ƶ��Ϊ60FPS������15FPS��֡�ʹ̶�Ϊ60ʱ��
	if (DIKEY(DIK_LSHIFT) || DIKEY(DIK_RSHIFT))
		m_nPressLength = 0;
	else
		m_nPressLength = 4;

	return 0;
	} // end __ProcessInput
