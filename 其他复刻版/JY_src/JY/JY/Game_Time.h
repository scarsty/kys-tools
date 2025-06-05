//-----------------------------------------------------------------------------
// ��Ϸʱ�����
// �ڶദ����ϵͳ�У�һ���߳�ֻ����һ������������
// ��������֡����ת����ֻ�ھ�ȷ�̶�֡���Ҳ���֡ʱ��׼ȷ�����û������
//-----------------------------------------------------------------------------

#ifndef GAME_TIME
#define GAME_TIME

// INCLUDES ///////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <assert.h>

#include "Game_Public.h"
#include "FH_Bit.h"

// DEFINES ////////////////////////////////////////////////////////////////////

// ʱ�����״̬
#define GT_ALLOW_DROP_FRAME		(1 << 0)		// ����֡
#define GT_RUNNING_SLOWLY		(1 << 1)		// ��ģ�鵱ǰ�����ٶȱ�Ԥ����
#define GT_DROPPED_THIS_FRAME	(1 << 31)		// �Ѷ�����ǰ֡

// MACROS /////////////////////////////////////////////////////////////////////
// TYPES //////////////////////////////////////////////////////////////////////
// CLASS //////////////////////////////////////////////////////////////////////

class CGameTime
	{
	public:
		CGameTime() : m_nState(0) {}
		~CGameTime() {}

		int Init(DWORD nFrameRate);
		// �ȴ����涨ʱ�䣨������֡��
		int Pass();
		// ����Ϸ��ͣ�ָֻ�ʱ���ָ���Ϸʱ��
		int Restore();
		// �����Ƿ���Ҫ��֡
		bool ShouldDropThisFrame();
		// ����������ǰ֡
		void AnnounceDropThisFrame();
		// ��õ�ǰCPU����
		int GetCurrentCpuCount(LONGLONG *pnCpuCount);
		// �Ѻ���ת��ΪCPU����
		LONGLONG MsToCpuCount(DWORD ms);
		// ֱ�ӷ���1���CPU����
		LONGLONG CpuCount1s();

		// ���ʱ�����״̬
		DWORD Check(DWORD bits) {return CHECK_BIT(m_nState, bits);}

#ifdef DEBUG
		float m_fFpsMain;						// ÿ��֡������ģ�飩
		float m_fFpsImage;						// ÿ��֡������ģ���е���ʾͼ��������FPS��
		float m_fPercent;						// һ����ĳ��������ʱ��İٷֱ�
#endif // DEBUG

	private:
		DWORD m_nState;							// ʱ�����״̬��λ�������
		LONGLONG m_nCPUFreq;					// ÿ���CPUʱ��������̶����������ɾ�̬��Ա��̫�鷳�ˣ�
		LONGLONG m_nCPUFreqFrame;				// ÿ֡�ĶCPUʱ�����
		LONGLONG m_nCPUCounterLastFrame;		// ��һ֡ʱ��¼��CPUʱ���ܼ���

#ifdef DEBUG
		DWORD m_nFramesMain;					// ��һ�뵽���ھ�����֡������ģ�飩
		DWORD m_nFramesImage;					// ��һ�뵽���ھ�����֡������ģ���е���ʾͼ��
		LONGLONG m_nCPUCounterLastSecond;		// ��һ��ʱ��¼��CPUʱ���ܼ���
#endif // DEBUG
	};

// PROTOTYPES /////////////////////////////////////////////////////////////////
// EXTERNALS //////////////////////////////////////////////////////////////////
// GLOBALS ////////////////////////////////////////////////////////////////////
// FUNCTIONS //////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: Init()
// Desc: ��ʼ����Ϸʱ��
//       ��Ҫ����֡�ʣ�֡��Ϊ0��ʾ��������
//-----------------------------------------------------------------------------
inline int CGameTime::Init(DWORD const nFrameRate)
	{
	// ��ȡCPUƵ��
	LARGE_INTEGER Counter;
	if (QueryPerformanceFrequency(&Counter) == 0)
		return -1;
	m_nCPUFreq = Counter.QuadPart;

	// ����ÿ֡�ĶCPUʱ�����
	// ������֡��ʱÿ֡�ĶCPUʱ�����Ϊ0��Ҳ������֡��û�ж�֡�ı�׼��
	if (nFrameRate != 0)
		{
		m_nCPUFreqFrame = m_nCPUFreq / nFrameRate;
		SET_BIT(m_nState, GT_ALLOW_DROP_FRAME);
		}
	else
		{
		m_nCPUFreqFrame = 0;
		RESET_BIT(m_nState, GT_ALLOW_DROP_FRAME);
		}

	// ��ʼ����һ֡ʱ�����
	if (QueryPerformanceCounter(&Counter) == 0)
		return -2;
	m_nCPUCounterLastFrame = Counter.QuadPart;

	#ifdef DEBUG
	// ��ʼ��������Ϣ��ر���
	m_fFpsMain = 0.0;
	m_fFpsImage = 0.0;
	m_fPercent = 0.0;

	m_nFramesMain = 0;
	m_nFramesImage = 0;

	// ��ʼ����һ��ʱ�����
	m_nCPUCounterLastSecond = Counter.QuadPart;
	#endif // DEBUG

	return 0;
	} // end Init



//-----------------------------------------------------------------------------
// Name: Pass()
// Desc: �ȴ����涨ʱ�䣨������֡��
//       ��ȡ����ѭ������ʱ�䣬�����涨��ʱ����ȴ���ֱ����Ϊֹ������ֱ�ӷ���
//-----------------------------------------------------------------------------
inline int CGameTime::Pass()
	{
	// ��ȡ��ǰCPUʱ���ܼ���
	LARGE_INTEGER Counter;
	if (QueryPerformanceCounter(&Counter) == 0)
		return -1;

	// ����趨Ϊ�̶�֡������ȴ����涨ʱ��
	if (m_nCPUFreqFrame != 0)
		{
		// ��ǰʱ���ѵ���򳬹���֡�涨����ʱ���***2��***ʱ��������Ϸ������־
		if (Counter.QuadPart - m_nCPUCounterLastFrame >= m_nCPUFreqFrame *2)
			SET_BIT(m_nState, GT_RUNNING_SLOWLY);
		// �������Ϸ������־���ȴ�����涨ʱ��
		else
			{
			RESET_BIT(m_nState, GT_RUNNING_SLOWLY);
			while (Counter.QuadPart - m_nCPUCounterLastFrame < m_nCPUFreqFrame)
				{
				Sleep(1);
				if (QueryPerformanceCounter(&Counter) == 0)
					return -1;
				}
			}
		// �Ѿ��������ĵ�ǰʱ����Ϊ��һ֡�Ŀ�ʼ����
		// ��û���˷�ʱ������ȥҪ����һ֡���Ա��ּ�¼�ľ�ȷ
		m_nCPUCounterLastFrame += m_nCPUFreqFrame;
		}

#ifdef DEBUG
	// ��ģ��֡������1
	++m_nFramesMain;

	// ���û�ж�����ǰ֡������ʾͼ��֡������1
	if (!CHECK_BIT(m_nState, GT_DROPPED_THIS_FRAME))
		++m_nFramesImage;

	// ����һ��������һ��ƽ��֡��
	if (Counter.QuadPart - m_nCPUCounterLastSecond  >= m_nCPUFreq)
		{
		float tmp = (float)m_nCPUFreq / (Counter.QuadPart - m_nCPUCounterLastSecond);
		m_fFpsMain = m_nFramesMain * tmp;
		m_fFpsImage = m_nFramesImage * tmp;
		// ֡������
		m_nFramesMain = 0;
		m_nFramesImage = 0;
		// �ѵ�ǰ������Ϊ��һ��Ŀ�ʼ����
		m_nCPUCounterLastSecond = Counter.QuadPart;
		}

	// �ָ���֡״̬��Ϊ��һ֡��׼��
	RESET_BIT(m_nState, GT_DROPPED_THIS_FRAME);
#endif // DEBUG

	return 0;
	} // end Pass



//-----------------------------------------------------------------------------
// Name: Restore()
// Desc: ����Ϸ��ͣ�ָֻ�ʱ���ָ���Ϸʱ��
//-----------------------------------------------------------------------------
inline int CGameTime::Restore()
	{
	LARGE_INTEGER Counter;
	if (QueryPerformanceCounter(&Counter) == 0)
		return -1;

	// ������һ֡ʱ�����
	m_nCPUCounterLastFrame = Counter.QuadPart;

#ifdef DEBUG
	// ֡������
	m_nFramesMain = 0;
	m_nFramesImage = 0;
	// ������һ��ʱ�����
	m_nCPUCounterLastSecond = Counter.QuadPart;
#endif // DEBUG

	return 0;
	} // end Restore



//-----------------------------------------------------------------------------
// Name: ShouldDropThisFrame()
// Desc: �����Ƿ���Ҫ��֡��������ǰ֡�Ļ�����ƣ�
//       �������֡���ٶȹ�������֡�����򲻶�֡
//-----------------------------------------------------------------------------
inline bool CGameTime::ShouldDropThisFrame()
	{
	if (CHECK_BIT(m_nState, GT_ALLOW_DROP_FRAME) && CHECK_BIT(m_nState, GT_RUNNING_SLOWLY))
		return true;
	else
		return false;
	}



//-----------------------------------------------------------------------------
// Name: AnnounceDropThisFrame()
// Desc: ����������ǰ֡
//-----------------------------------------------------------------------------
inline void CGameTime::AnnounceDropThisFrame()
	{
#ifdef DEBUG
	SET_BIT(m_nState, GT_DROPPED_THIS_FRAME);
#endif // DEBUG
	}



//-----------------------------------------------------------------------------
// Name: GetCurrentCpuCount()
// Desc: ��õ�ǰCPU����
//-----------------------------------------------------------------------------
inline int CGameTime::GetCurrentCpuCount(LONGLONG * const pnCpuCount)
	{
	assert(pnCpuCount);

	LARGE_INTEGER Counter;
	if (QueryPerformanceCounter(&Counter) == 0)
		return -1;
	*pnCpuCount = Counter.QuadPart;
	return 0;
	}



//-----------------------------------------------------------------------------
// Name: MsToCpuCount()
// Desc: �Ѻ���ת��ΪCPU����
//-----------------------------------------------------------------------------
inline LONGLONG CGameTime::MsToCpuCount(DWORD const ms)
	{
	static LONGLONG nCPUFreqMs = m_nCPUFreq / 1000;
	return (nCPUFreqMs * ms);
	}



//-----------------------------------------------------------------------------
// Name: CpuCount1s()
// Desc: ֱ�ӷ���1���CPU����
//-----------------------------------------------------------------------------
inline LONGLONG CGameTime::CpuCount1s()
	{
	return m_nCPUFreq;
	}

#endif // GAME_TIME