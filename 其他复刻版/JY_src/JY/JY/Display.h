//-----------------------------------------------------------------------------
// Game.cpp - ��ʾͼ��
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
// Name: __Display()
// Desc: ��ʾͼ��
//-----------------------------------------------------------------------------
int CGame::__Display()
	{
	// ��պ󱸻������
	DDraw_Fill_Surface(lpddsback,0);

	// ����ȫ������Ļ���
	CImageManager::SurfaceDeactivateAll();

#ifdef DEBUG
	// ��ʼ��ʱ
	StartTimer(&m_nCpuCount);
#endif // DEBUG

	// ��ʾ�����ͼ
	if (m_WorldMap.Show(&m_Player.m_locWorld) != 0)
		{
		g_Log.Write("��ʾ�����ͼʧ��\r\n");
		return -1;
		}

#ifdef DEBUG
	// ������ʱ
	StopTimer(&m_nCpuCount);
	m_nCpuCountSum += m_nCpuCount;
#endif // DEBUG

#ifdef DEBUG
	///////////////////////////////////////////////////////////
	// ��ʾ������Ϣ

	char buffer[80];

	// ��ʾ��ǰ�������꣬���ñ�����/�����ɱ�����
	sprintf_s(buffer, 80, "(%d, %d) %u/%u",
		m_Player.m_locWorld.x, m_Player.m_locWorld.y,
		CImageManager::SurfaceUsed(), CImageManager::MaxSurface());
	Draw_Text_GDI(buffer, 8, m_piGameWnd->ClientHeight() - 20, RGB(192,192,0), lpddsback);

	// ÿ�����һ��ʱ���ϵ
	// ��Ϊ��ʱ���̡�����ʱ���������ܵ���ͣ��Ӱ�죬�������ս��Ҳ����
	// ����֡ʱ�����ǵȻ�����涨֡������㣬��˳��˻ָ�������ϼ���ʱ���ϵ��Ҳû���κ�Ӱ��

	// ��ȡ��ǰʱ��
	LONGLONG nCpuCountCurrent;
	m_Time.GetCurrentCpuCount(&nCpuCountCurrent);

	// ��ǰʱ�����ϴμ���ʱ�������һ�������
	if ( nCpuCountCurrent - m_nCpuCountLastCalc >= m_Time.CpuCount1s() )
		{
		m_Time.m_fPercent = (float)m_nCpuCountSum / (nCpuCountCurrent - m_nCpuCountLastCalc);
		m_nCpuCountLastCalc = nCpuCountCurrent;
		m_nCpuCountSum = 0;
		}

	// ��ʾÿ��֡������ʾͼ��/��ģ�飩��һ����Show()����ʱ��ٷֱ�
	sprintf_s(buffer, 80, "FPS: %.1f/%.1f  Show: %.1f%%",
		m_Time.m_fFpsImage, m_Time.m_fFpsMain, m_Time.m_fPercent *100);
	Draw_Text_GDI(buffer, 8, 8, RGB(192,192,0), lpddsback);
#endif // DEBUG

	///////////////////////////////////////////////////////////

	// ��ת����
	if (DDraw_Flip(m_piGameWnd->rectClient()) != 1)
		{
		g_Log.Write("��ת����ʧ��\r\n");
		return -1;
		}

	return 0;
	} // end __Display
