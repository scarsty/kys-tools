//-----------------------------------------------------------------------------
// ��¼��Ϣ�����ֶ��󣨿���̨���ļ����ؼ���
//-----------------------------------------------------------------------------

#ifndef FH_LOG
#define FH_LOG

// INCLUDES ///////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// DEFINES ////////////////////////////////////////////////////////////////////

#define LOG_BUFFER_SIZE		1024		// һ�������������󳤶�

// ��־���������
#define TAR_NULL			0
#define TAR_CONSOLE			(1 << 0)		// ����̨
#define TAR_FILE			(1 << 1)		// �ļ�
#define TAR_EDIT_CONTROL	(1 << 2)		// Edit�ؼ�

// MACROS /////////////////////////////////////////////////////////////////////
// TYPES //////////////////////////////////////////////////////////////////////

// ��־ǰ׺�������־ǰ�����������
// �����ʱ���Ϊ����ʱ��
enum LOG_PREFIX
	{
	PRE_NULL,
	PRE_DATE,				// ����ʱ��
	PRE_DATE_TIME,
	PRE_TIME,
	PRE_TIME_MS,
	PRE_RELATIVE_DAY_TIME,	// ���ʱ�䣬�����㵽49��
	PRE_RELATIVE_TIME_MS,
	PRE_RELATIVE_S_MS
	};

// CLASS //////////////////////////////////////////////////////////////////////

class CLog
	{
	public:
		CLog()
			{
			_tick_count_start = GetTickCount();		// �����ڳ���ͷ������࣬�������ʱ�����ƫ��
			_target = TAR_CONSOLE;
			_prefix = PRE_NULL;
			_h_file = NULL;
			_h_edit_control = NULL;
			}

		~CLog()
			{
			if (_h_file)
				CloseHandle(_h_file);
			}

		// ����Ĭ�ϼ�¼��ʽ�����Ķ���������ǰ׺��ʲô��
		// �˴������÷�ʽ�Ƿ��Ѿ���ʼ����������ÿ�����Ŀ���ʼ��֮ǰ����
		void Set(DWORD log_target, LOG_PREFIX log_prefix)
			{
			_target = log_target;
			_prefix = log_prefix;
			}

		// ��ʼ��Log���Ŀ�꣨����̨���ó�ʼ����
		// ��ʼ��������־������ļ�
		int Init_File(const char *log_file_name = NULL);
		// ��ʼ��������־�����Edit�ؼ�
		int Init_Edit_Control(HWND hEditControl);

		// ����ʼ��Log���Ŀ�꣨���Բ�����ʼ�����������������������������
		// ����ʼ��������־������ļ�
		void Uninit_File()
			{
			// �ر��ļ�����վ��
			if (_h_file)
				{
				CloseHandle(_h_file);
				_h_file = NULL;
				}
			// ���Ŀ��λ
			_target &= ~TAR_FILE;
			}
		// ����ʼ��������־�����Edit�ؼ����ؼ������ڳ����˳�֮ǰ�رգ�
		void Uninit_Edit_Control()
			{
			// ��վ�������Ŀ��λ
			_h_edit_control = NULL;
			_target &= ~TAR_EDIT_CONTROL;
			}

		// ��¼��־��Ϣ�������������ǰ׺����ʹ��Ĭ��ֵ
		// ���������Ĵ����зǳ�����ظ��������ڿɱ������ԭ���޷���ȡ�������У��ú���̫�鷳
		int Write(const char *str, ...);
		int Write(DWORD log_target, LOG_PREFIX log_prefix, const char *str, ...);

	private:
		// ����ʽ���ǰ׺��������
		int _Sprint_Prefix(LOG_PREFIX log_prefix);

		// ����������ݼ�¼����
		int _Write_Console(const char *buffer, int count);
		int _Write_File(const char *buffer, int count);
		int _Write_Edit_Control(const char *buffer, int count);

		char _buffer[LOG_BUFFER_SIZE];		// ��־���ݵĻ���ռ�
		DWORD _target;						// Log��Ϣ��Ĭ�����Ŀ��
		LOG_PREFIX _prefix;					// ���Log��Ϣʱ��Ĭ��ǰ׺
		DWORD _tick_count_start;			// ����ʼ����ʱϵͳ�����еĺ�����
		HANDLE _h_file;						// ������־������ļ��ľ��
		HWND _h_edit_control;				// ������־�����Edit�ؼ��ľ��
	};

// PROTOTYPES /////////////////////////////////////////////////////////////////
// GLOBALS ////////////////////////////////////////////////////////////////////
// FUNCTIONS //////////////////////////////////////////////////////////////////

#endif // FH_LOG