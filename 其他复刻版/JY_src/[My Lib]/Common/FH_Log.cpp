
// INCLUDES ///////////////////////////////////////////////////////////////////

#include <vector>

#include "FH_Log.h"

// DEFINES ////////////////////////////////////////////////////////////////////
// TYPES //////////////////////////////////////////////////////////////////////

// ���ڵľ����Ĭ����Ϣ��������ַ
typedef struct _Wnd_Handle_And_Def_Proc
	{
	HWND hwnd;
	WNDPROC def_wnd_proc;
	} Wnd_Handle_And_Def_Proc;

// PROTOTYPES /////////////////////////////////////////////////////////////////

INT_PTR CALLBACK Edit_Control_Proc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
void Save_Def_Wnd_Proc(HWND hwnd, WNDPROC def_wnd_proc);
WNDPROC Load_Def_Wnd_Proc(HWND hwnd);

// EXTERNALS //////////////////////////////////////////////////////////////////
// GLOBALS ////////////////////////////////////////////////////////////////////

// ���ڵľ����Ĭ����Ϣ��������ַ��Ӧ��
std::vector<Wnd_Handle_And_Def_Proc> Vec_Wnd_Handle_And_Def_Proc;

// FUNCTIONS //////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: Init_File()
// Desc: Log�ļ���ʼ��
//       ������ļ���Ϊ��ʱ��Log�ļ���Ϊ��ǰִ���ļ���.log��·���͵�ǰ������ͬ
//       ���ļ�������ʹ��ʱ���û��򿪲鿴���ݣ��������޸�
//
//����ֵ��
// 0	��������
//-1	֮ǰ��ִ�й���ʼ��
//-2~-4	�޷�����Log�ļ���
//-5	�޷��򿪻򴴽�Log�ļ�
//-----------------------------------------------------------------------------
int CLog::Init_File(const char *log_file_name)
	{
	// ������ļ�����ַ��Ϊ�յĻ�����ָ����ַ����Ͳ���Ϊ��
	assert( log_file_name == NULL || (log_file_name && *log_file_name) );

	if (_h_file) return -1;		// ֮ǰ��ִ�й���ʼ��

	// ������ļ�����ַΪ��ʱ������Ĭ��Log�ļ���
	char default_log_file_name[_MAX_PATH];
	if (log_file_name == NULL)
		{
		// ��õ�ǰִ���ļ�����·��
		char self_name[MAX_PATH];
		const DWORD self_name_len = GetModuleFileName(NULL, self_name, MAX_PATH);
		if (self_name_len == 0 || self_name_len == MAX_PATH)
			return -2;		// ��õ�ǰִ���ļ�����·������

		// ��ȡ��ǰִ���ļ�����·���е��ļ���
		char fname[_MAX_FNAME];
		errno_t err = _splitpath_s(self_name, NULL, 0, NULL, 0, fname, _MAX_FNAME, NULL, 0);
		if (err != 0) return -3;	// �ֽ�·������

		// �ϲ�Log�ļ�������չ��Ϊ".log"
		err = _makepath_s(default_log_file_name, _MAX_PATH, NULL, NULL, fname, ".log");
		if (err != 0) return -4;	// �ϲ�·������

		// ��Log�ļ�����ַָ��Ĭ��Log�ļ���
		log_file_name = default_log_file_name;
		}

	// ��Log�ļ�
	HANDLE hFile = CreateFile(log_file_name,
							  GENERIC_WRITE,
							  FILE_SHARE_READ,
							  NULL,
							  OPEN_ALWAYS,
							  FILE_ATTRIBUTE_NORMAL,
							  NULL);
	if (hFile == INVALID_HANDLE_VALUE) return -5;		// �޷��򿪻򴴽�Log�ļ�

	// �ļ�ָ���Ƶ��ļ�ĩβ����д�뷽ʽΪ���ӣ�
	SetFilePointer(hFile, 0, NULL, FILE_END);

	// �ڳɹ���ʼ��֮��Ž����ļ����
	_h_file = hFile;
	return 0;
	} // end Init_File



//-----------------------------------------------------------------------------
// Name: Init_Edit_Control()
// Desc: ��ʼ��������־�����Edit�ؼ���������Rich Edit�ؼ���
//       �ؼ��ɵ����ߴ������ؼ���ʽ�����ܴ�ֱ����(ES_AUTOVSCROLL / WS_VSCROLL)������ʾ����(ES_MULTILINE)
//
//����ֵ��
// 0	��������
// -1	֮ǰ��ִ�й���ʼ��
// -2	��ȡ�ؼ�ԭ����ʽʧ�ܣ�����ԭ��ʽΪ��
// -3	�ؼ���ʽ����Ҫ��
// -4	�޸Ŀؼ���ʽʧ��
// -5	ˢ�¿ؼ���ʽʧ��
// -6	�޷��滻�ؼ���Ϣ������
//-----------------------------------------------------------------------------
int CLog::Init_Edit_Control(HWND const hEditControl)
	{
	assert(hEditControl);

	if (_h_edit_control) return -1;		// ֮ǰ��ִ�й���ʼ��

	///////////////////////////////////////////////////////////
	// ��Ⲣ�޸Ŀؼ���ʽ

	// ��ȡ�ؼ�ԭ����ʽ
	// ������ʧ��ʱ����0�����Ҫ���ص�ֵ�������0Ҳ�᷵��0
	DWORD Pre_Control_Style = static_cast<DWORD>( GetWindowLongPtr(hEditControl, GWL_STYLE) );
	if (Pre_Control_Style == 0) return -2;		// ��ȡ�ؼ�ԭ����ʽʧ�ܣ�����ԭ��ʽΪ��

	// �����ʽ�Ƿ�������Ҫ
	if (!(Pre_Control_Style & ES_AUTOVSCROLL || Pre_Control_Style & WS_VSCROLL) ||
		!(Pre_Control_Style & ES_MULTILINE))
		return -3;		// �ؼ���ʽ����Ҫ��

	// �޸Ŀ��ڴ������޸ĵ���ʽ��ȥ��������Tab�л����ÿؼ���
	DWORD Control_Style = Pre_Control_Style;
	Control_Style &= ~WS_TABSTOP;

	// �������޸ĵ���ʽ
	// ������ʧ��ʱ����0�����Ҫ���ص�ֵ�������0Ҳ�᷵��0
	Pre_Control_Style = static_cast<DWORD>( SetWindowLongPtr(hEditControl, GWL_STYLE, Control_Style) );
	if (Pre_Control_Style == 0) return -4;		// �޸Ŀؼ���ʽʧ�ܣ�ԭ��ʽ����Ϊ�գ���Ϊ��ǰ���Ѿ������ˣ�

	// ˢ�¿ؼ���ʽ
	DWORD result = SetWindowPos(hEditControl, 0, 0, 0, 0, 0,
								SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	if (result == 0) return -5;		// ˢ�¿ؼ���ʽʧ��

	///////////////////////////////////////////////////////////
	// �����ؼ�����

	// ���ÿؼ��������ַ���Ϊ���
	SendMessage(hEditControl, EM_SETLIMITTEXT, 0, 0);

	///////////////////////////////////////////////////////////
	// �Զ��ؼ���Ϣ������

	// ���Զ���Ϣ����������Ĭ����Ϣ������
	// ������ʧ��ʱ����0�����Ҫ���ص�ֵ�������0Ҳ�᷵��0
	// ����Ч�ĺ�����ַ������Ϊ0����˷���0��˵��ʧ��
	WNDPROC DefControlProc = reinterpret_cast<WNDPROC>(
		SetWindowLongPtr(hEditControl, GWLP_WNDPROC, reinterpret_cast<LONG>(Edit_Control_Proc)) );
	if (DefControlProc == 0) return -6;		// �޷��滻�ؼ���Ϣ������

	// ���浱ǰ�ؼ���Ĭ����Ϣ��������ַ
	Save_Def_Wnd_Proc(hEditControl, DefControlProc);

	// �ڳɹ���ʼ��֮��Ž��ܿؼ����
	_h_edit_control = hEditControl;
	return 0;
	} // end Init_Edit_Control



//-----------------------------------------------------------------------------
// Name: Write()
// Desc: �������ָ��ǰ׺����־��Ϣ��ָ��Ŀ��
//       ���Ȱ���Ϣת��Ϊ�ַ����������������Ȼ����ø�Ŀ����������
//       Ŀ�����Ϊ�����������Ŀ������δ����ʼ���ģ�����Ը�Ŀ�꣬�����ش���
//       ����ͳһд��\r\n
//
// �����������Ŀ�������Чλ���Ҳ���������ʼ����Ŀ�꣬��������Ȼ���������������
// ����������Ŀ�궼����Ч�ģ����ú궨��ģ�������Ա�֤�����Ŀ�궼û�б���ʼ��ʱ�����������ء�
// ����������д����ʱ����ܶ�������ļ�����䣬��ֻҪ�ڳ���ʼʱ����ʼ��Log�ļ����ͼ���û��ʱ����ʧ��
//
// ����ֵ��ָֻʾ�����һ������
// 0	��������
// -1	д��ǰ׺��������ʧ��
// -2	д�봫����Ϣ��������ʧ��
// -101~-200	���������̨����
// -201~-300	������ļ�����
// -301~-400	�����Edit�ؼ�����
//-----------------------------------------------------------------------------
int CLog::Write(DWORD log_target, LOG_PREFIX const log_prefix, const char * const str, ...)
	{
	assert(str);

	// ������������Ŀ���е���ЧĿ��
	// ���Ŀ������Ч����ֱ�ӽ����Ŀ���еĶ�Ӧλ����
	if (_h_file == NULL)			log_target &= ~TAR_FILE;
	if (_h_edit_control == NULL)	log_target &= ~TAR_EDIT_CONTROL;

	if (log_target == NULL) return 0;		// ���Ŀ��Ϊ��

	///////////////////////////////////////////////////////////
	// ����Ϣ�����������

	// ����ʽ���ǰ׺������������¼д�뻺������ǰ׺�ַ���
	int count_pre = _Sprint_Prefix(log_prefix);
	if (count_pre < 0) return -1;		// д��ǰ׺��������ʧ��

	// �Ѵ�����Ϣ�������������λ����ǰ׺�ĺ���
	va_list arglist;
	va_start(arglist, str);
	int count = vsprintf_s(_buffer + count_pre, LOG_BUFFER_SIZE - count_pre, str, arglist);
	va_end(arglist);
	if (count < 0) return -2;		// д�봫����Ϣ��������ʧ��

	// ���㻺�������ܹ���д����ַ�������������β���ַ���
	// sprintf_s()��֤��������Կ��ַ���β��������ַ���һ���Ȼ�����sizeС
	int total_count = count_pre + count;

	///////////////////////////////////////////////////////////
	// ��������д����ϣ����ø�Ŀ����Ӧ�ķ�����¼����

	// ���ݿ���Ҫ��������Ŀ�꣬������Ϊһ��Ŀ�����ͷ��أ�����Ҫָʾ������������Ҫ�õ�����ֵ����
	// ����ֵ����ֻ��ָʾ�����һ������
	int ret_val = 0;

	// ���������̨
	if (log_target & TAR_CONSOLE)
		{
		int result = _Write_Console(_buffer, total_count);
		if (result != 0) ret_val = -100 + result;		// ���������̨����
		}

	// ������ļ�
	if (log_target & TAR_FILE)
		{
		int result = _Write_File(_buffer, total_count);
		if (result != 0) ret_val = -200 + result;		// ������ļ�����
		}

	// �����Edit�ؼ�
	if (log_target & TAR_EDIT_CONTROL)
		{
		int result = _Write_Edit_Control(_buffer, total_count);
		if (result != 0) ret_val = -300 + result;		// �����Edit�ؼ�����
		}

	return ret_val;
	} // end Write



//-----------------------------------------------------------------------------
// Name: Write()
// Desc: �������Ĭ��ǰ׺����־��Ϣ��Ĭ��Ŀ��
//       ����Ĭ�ϲ�������ȫ�հ������ȫ��ͬ�������Ĵ���
//-----------------------------------------------------------------------------
int CLog::Write(const char * const str, ...)
	{
	assert(str);

	// ����δ����Ĳ���
	DWORD log_target = _target;
	LOG_PREFIX const log_prefix = _prefix;

	///////////////////////////////////////////////////////////////////////////
	// ��ȫ�հ������ȫ��ͬ�������Ĵ��룬�Ǳ�����޸���ֻ����ȫ���ƹ�������

	// ������������Ŀ���е���ЧĿ��
	// ���Ŀ������Ч����ֱ�ӽ����Ŀ���еĶ�Ӧλ����
	if (_h_file == NULL)			log_target &= ~TAR_FILE;
	if (_h_edit_control == NULL)	log_target &= ~TAR_EDIT_CONTROL;

	if (log_target == NULL) return 0;		// ���Ŀ��Ϊ��

	///////////////////////////////////////////////////////////
	// ����Ϣ�����������

	// ����ʽ���ǰ׺������������¼д�뻺������ǰ׺�ַ���
	int count_pre = _Sprint_Prefix(log_prefix);
	if (count_pre < 0) return -1;		// д��ǰ׺��������ʧ��

	// �Ѵ�����Ϣ�������������λ����ǰ׺�ĺ���
	va_list arglist;
	va_start(arglist, str);
	int count = vsprintf_s(_buffer + count_pre, LOG_BUFFER_SIZE - count_pre, str, arglist);
	va_end(arglist);
	if (count < 0) return -2;		// д�봫����Ϣ��������ʧ��

	// ���㻺�������ܹ���д����ַ�������������β���ַ���
	// sprintf_s()��֤��������Կ��ַ���β��������ַ���һ���Ȼ�����sizeС
	int total_count = count_pre + count;

	///////////////////////////////////////////////////////////
	// ��������д����ϣ����ø�Ŀ����Ӧ�ķ�����¼����

	// ���ݿ���Ҫ��������Ŀ�꣬������Ϊһ��Ŀ�����ͷ��أ�����Ҫָʾ������������Ҫ�õ�����ֵ����
	// ����ֵ����ֻ��ָʾ�����һ������
	int ret_val = 0;

	// ���������̨
	if (log_target & TAR_CONSOLE)
		{
		int result = _Write_Console(_buffer, total_count);
		if (result != 0) ret_val = -100 + result;		// ���������̨����
		}

	// ������ļ�
	if (log_target & TAR_FILE)
		{
		int result = _Write_File(_buffer, total_count);
		if (result != 0) ret_val = -200 + result;		// ������ļ�����
		}

	// �����Edit�ؼ�
	if (log_target & TAR_EDIT_CONTROL)
		{
		int result = _Write_Edit_Control(_buffer, total_count);
		if (result != 0) ret_val = -300 + result;		// �����Edit�ؼ�����
		}

	return ret_val;
	//
	///////////////////////////////////////////////////////////////////////////
	} // end Write



//-----------------------------------------------------------------------------
// Name: _Sprint_Prefix()
// Desc: ����ʽ���ǰ׺��������
//       ����������ַ�������������β���ַ���������ʧ�ܣ��򷵻ظ�ֵ
//-----------------------------------------------------------------------------
int CLog::_Sprint_Prefix(LOG_PREFIX const log_prefix)
	{
	int count_pre = 0;		// д�뻺������ǰ׺�ַ���
	switch (log_prefix)
		{
		// ��
		case PRE_NULL:
			{
			return 0;
			}
		// ����
		case PRE_DATE:
			{
			// ��ȡ��ǰʱ��
			SYSTEMTIME time;
			GetLocalTime(&time);
			// �����ǰ���ڣ�����ֵΪд����ַ�����
			count_pre = sprintf_s(_buffer, LOG_BUFFER_SIZE, "[%4d/%02d/%02d] ",
								  time.wYear, time.wMonth, time.wDay);
			break;
			}
		// ������ʱ��
		case PRE_DATE_TIME:
			{
			SYSTEMTIME time;
			GetLocalTime(&time);
			count_pre = sprintf_s(_buffer, LOG_BUFFER_SIZE, "[%4d/%02d/%02d %02d:%02d:%02d] ",
								  time.wYear, time.wMonth, time.wDay,
								  time.wHour, time.wMinute, time.wSecond);
			break;
			}
		// ʱ��
		case PRE_TIME:
			{
			SYSTEMTIME time;
			GetLocalTime(&time);
			count_pre = sprintf_s(_buffer, LOG_BUFFER_SIZE, "[%02d:%02d:%02d] ",
								  time.wHour, time.wMinute, time.wSecond);
			break;
			}
		// ʱ�䣨�����룩
		case PRE_TIME_MS:
			{
			SYSTEMTIME time;
			GetLocalTime(&time);
			count_pre = sprintf_s(_buffer, LOG_BUFFER_SIZE, "[%02d:%02d:%02d.%03d] ",
								  time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
			break;
			}
		// ��Գ���ʼ����ʱ��������ʱ��
		case PRE_RELATIVE_DAY_TIME:
			{
			// �á���ϵͳ���������ھ����ĺ���������ȥ����ϵͳ����������ʼ���о����ĺ�������
			DWORD ms = GetTickCount() - _tick_count_start;
			count_pre = sprintf_s(_buffer, LOG_BUFFER_SIZE, "[%d day(s) %02d:%02d:%02d] ",
								  ms/(24*60*60*1000),
								  ms/(60*60*1000)%24,(ms/(60*1000))%60, (ms/1000)%60);
			break;
			}
		// ��Գ���ʼ����ʱ��ʱ�䣨�����룩
		case PRE_RELATIVE_TIME_MS:
			{
			DWORD ms = GetTickCount() - _tick_count_start;
			count_pre = sprintf_s(_buffer, LOG_BUFFER_SIZE, "[%d:%02d:%02d.%03d] ",
								  ms/(60*60*1000), (ms/(60*1000))%60, (ms/1000)%60, ms%1000);
			break;
			}
		// ��Գ���ʼ����ʱ�������������룩
		case PRE_RELATIVE_S_MS:
			{
			DWORD ms = GetTickCount() - _tick_count_start;
			count_pre = sprintf_s(_buffer, LOG_BUFFER_SIZE, "[%d.%03d] ", ms/1000, ms%1000);
			break;
			}
		} // end switch (log_prefix)

	return count_pre;
	} // end _Sprint_Prefix



//-----------------------------------------------------------------------------
// Name: _Write_Console()
// Desc: ��¼���ݵ�����̨
//       ���뻺���ַ������ֽ���
//       ���б���д��\r\n�����ں�����ת��Ϊ\n��
//-----------------------------------------------------------------------------
int CLog::_Write_Console(const char * buffer, int const count)
	{
	assert(buffer && count > 0 && count < LOG_BUFFER_SIZE);

	///////////////////////////////////////////////////////////
	// ɾ�������е�\r
	// ����̨�ڻ���д��\r\nʱ������һ��\r���ڿ���̨�Ͽ���������ֻ��������ļ����ܿ��õ�

	// ��ʱ����
	char tmp_buffer[LOG_BUFFER_SIZE];
	char *p_tmp = tmp_buffer;

	// ��ԭ�����ж�ȡ���ݣ��ѳ�\r��������ַ����θ��Ƶ���ʱ������
	const char *p_buffer = buffer;
	const char * const buffer_end = buffer + count;
	while (p_buffer < buffer_end)
		{
		if (*p_buffer != '\r')
			*p_tmp++ = *p_buffer++;
		else
			++p_buffer;
		}
	// ����ʱ�����м��Ͻ�β�Ŀ��ַ�
	*p_tmp = '\0';

	// ��Ҫ����Ļ���ָ��Ϊ��ʱ����
	buffer = tmp_buffer;

	///////////////////////////////////////////////////////////

	return printf("%s",buffer);
	}



//-----------------------------------------------------------------------------
// Name: _Write_File()
// Desc: ��¼���ݵ��ļ�
//       ���뻺���ַ������ֽ���
//       ���б���д��\r\n
//-----------------------------------------------------------------------------
int CLog::_Write_File(const char * const buffer, int const count)
	{
	assert(buffer && count > 0 && count < LOG_BUFFER_SIZE);

	// ������д���ļ�
	DWORD nBytes;
	WriteFile(_h_file, buffer, count, &nBytes, NULL);

	// ���������ļ��������е����ݣ�Ƶ����¼����ʱӲ�̸��ɹ��أ�ֻ��ȥ����䣩
	//FlushFileBuffers(_h_file);

	return 0;
	}



//-----------------------------------------------------------------------------
// Name: _Write_Edit_Control()
// Desc: ��¼�ı����ݵ�Edit�ؼ�
//       ���뻺���ַ������ֽ������ֽ�����ʱ�ò�����
//       ���б���д��\r\n
//-----------------------------------------------------------------------------
int CLog::_Write_Edit_Control(const char * const buffer, int const count)
	{
	assert(buffer && count > 0 && count < LOG_BUFFER_SIZE);

	// ��Edit�ؼ��ڵĹ���Ƶ����
	SendMessage(_h_edit_control, EM_SETSEL, -1, -1);

	// �Ѹ����ɵ����ָ��ӵ�Edit�ؼ��ڹ���������ִ�
	SendMessage(_h_edit_control, EM_REPLACESEL, false, (LPARAM)buffer);

	return 0;
	}



//-----------------------------------------------------------------------------
// Name: Edit_Control_Proc()
// Desc: ������־�����Edit�ؼ����Զ���Ϣ������
//-----------------------------------------------------------------------------
INT_PTR CALLBACK Edit_Control_Proc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
	{
	switch(msg)
		{
		// �������ͼ�����Ϣ
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MBUTTONDBLCLK:
			return true;

		default:
			{
			// ��ȡ���ؼ���Ĭ����Ϣ��������ַ
			WNDPROC Def_Wnd_Proc = Load_Def_Wnd_Proc(hDlg);
			// ���صĵ�ַΪ��ʱ��˵������ľ�������þ����ǰ��δ���棬��Щ�����ܱ���д�߱ܿ�
			assert(Def_Wnd_Proc != NULL);
			// ���ÿؼ�ԭ������Ϣ������
			return CallWindowProc(Def_Wnd_Proc, hDlg, msg, wParam, lParam);
			}
		}
	} // end Edit_Control_Proc



//-----------------------------------------------------------------------------
// Name: Save_Def_Wnd_Proc()
// Desc: ���浱ǰ���ڵ�Ĭ����Ϣ��������ַ
//-----------------------------------------------------------------------------
void Save_Def_Wnd_Proc(HWND hwnd, WNDPROC def_wnd_proc)
	{
	Wnd_Handle_And_Def_Proc tmp = {hwnd, def_wnd_proc};
	Vec_Wnd_Handle_And_Def_Proc.push_back(tmp);
	}



//-----------------------------------------------------------------------------
// Name: Load_Def_Wnd_Proc()
// Desc: ��ȡ����ľ������Ĵ��ڵ�Ĭ����Ϣ��������ַ
//       ���ؿյ�ַ��ʾ����ľ�������þ����ǰ��δ���棬��Щ�����ܱ���д�߱ܿ�
//-----------------------------------------------------------------------------
WNDPROC Load_Def_Wnd_Proc(HWND hwnd)
	{
	// ����vector����Ѱ�Ҵ��������ҵ��򷵻���Ӧ�ĺ�����ַ
	for (std::vector<Wnd_Handle_And_Def_Proc>::iterator it = Vec_Wnd_Handle_And_Def_Proc.begin();
		 it != Vec_Wnd_Handle_And_Def_Proc.end(); ++it)
		{
		if ( (*it).hwnd == hwnd )
			return (*it).def_wnd_proc;
		}

	// û���ҵ��򷵻ؿյ�ַ
	return NULL;
	}

