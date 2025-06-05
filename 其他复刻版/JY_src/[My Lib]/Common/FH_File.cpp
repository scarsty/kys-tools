
// INCLUDES ///////////////////////////////////////////////////////////////////

#include "FH_File.h"

// DEFINES ////////////////////////////////////////////////////////////////////
// TYPES //////////////////////////////////////////////////////////////////////
// PROTOTYPES /////////////////////////////////////////////////////////////////
// EXTERNALS //////////////////////////////////////////////////////////////////
// GLOBALS ////////////////////////////////////////////////////////////////////
// FUNCTIONS //////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Set_Process_Dir_to_Exe_Dir()
// �ѵ�ǰ���̵�·������Ϊ��ǰִ���ļ�·��
// ��ֹ���򴴽���ָֻ�������·�����ļ���λ�ò��ڵ�ǰ���������ļ��л����ļ�����
//-----------------------------------------------------------------------------
int Set_Process_Dir_to_Exe_Dir()
	{
	// ��õ�ǰִ���ļ�����·��
	char self_name[MAX_PATH];
	const DWORD self_name_len = GetModuleFileName(NULL, self_name, MAX_PATH);
	if (self_name_len == 0 || self_name_len == MAX_PATH)
		return -1;		// ��õ�ǰִ���ļ�����·������

	// ��ȡ��ǰִ���ļ�����·���е��̷���Ŀ¼
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	errno_t err = _splitpath_s(self_name, drive, _MAX_DRIVE, dir, _MAX_DIR, NULL, 0, NULL, 0);
	if (err != 0) return -2;	// �ֽ�·������

	// �ϲ���ȡ���ĵ�ǰִ���ļ����̷���Ŀ¼
	char exe_directory[_MAX_DRIVE + _MAX_DIR];
	err = _makepath_s(exe_directory, _MAX_DRIVE + _MAX_DIR, drive, dir, NULL, NULL);
	if (err != 0) return -3;	// �ϲ�·������

	// ���õ�ǰ���̵�·��Ϊ����ϲ�����·��
	if (!SetCurrentDirectory(exe_directory))
		return -4;		// ���õ�ǰ·������

	return 0;
	} // end Set_Process_Dir_to_Exe_Dir



//-----------------------------------------------------------------------------
// GetExeFileName_WithExt()
// �����ִ���ļ������ļ����еġ��ļ�����ִ���ļ���ͬ�ġ�ָ����չ�����ļ�������·��
// ����ִ���ļ���Ϊsample.exe��ͨ���˺������Ի�ȡͬ�ļ��е�sample.ini������·��
// �ļ����ڴ�ռ�����㹻�󣬵��������������
// ��չ������Ϊ�գ���չ����ַ����Ϊ��
//-----------------------------------------------------------------------------
int GetExeFileName_WithExt(char * const filename, const char * const ext)
	{
	assert(filename);

	// ��õ�ǰִ���ļ�����·��
	char self_name[MAX_PATH];
	const DWORD self_name_len = GetModuleFileName(NULL, self_name, MAX_PATH);
	if (self_name_len == 0 || self_name_len == MAX_PATH)
		return -1;		// ��õ�ǰִ���ļ�����·������

	// ��ȡ��ǰִ���ļ�����·���е��̷���Ŀ¼���ļ���
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	errno_t err = _splitpath_s(self_name, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, NULL, 0);
	if (err != 0) return -2;	// �ֽ�·������

	// �ϲ��ļ�����������չ��ext
	err = _makepath_s(filename, _MAX_PATH, drive, dir, fname, ext);
	if (err != 0) return -3;	// �ϲ�·������

	return 0;
	} // end GetExeFileName_WithExt



//-----------------------------------------------------------------------------
// ChangeFilenameExt()
// �޸Ĵ�����ļ�·���е���չ��
// �ļ����ڴ�ռ�����㹻��MAX_PATH�������������������
// ��չ������Ϊ�գ���չ����ַ����Ϊ��
//-----------------------------------------------------------------------------
int ChangeFilenameExt(char * const filename, const char * const ext)
	{
	assert(filename);

	// ��ȡ�ļ�·���е��̷���Ŀ¼���ļ���
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	errno_t err = _splitpath_s(filename, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, NULL, 0);
	if (err != 0) return -1;	// �ֽ�·������

	// �ϲ��ļ�����������չ��ext
	err = _makepath_s(filename, _MAX_PATH, drive, dir, fname, ext);
	if (err != 0) return -2;	// �ϲ�·������

	return 0;
	} // end ChangeFilenameExt



//-----------------------------------------------------------------------------
// LoadFile()
// ��ĳ���Ѵ��ڵ��ļ������ڴ棬ʹ���ڴ�ӳ��
// ���ʷ�ʽ��0-ֻ����1-��д��2-��д�������棨ʹ�õ��ڴ���������ֶࣩ
// ���޸Ĵ����Mem_File���󣬵�����ʧ��ʱ����ֵ������
// �ļ�Ϊ��ʱ������ʧ��
//-----------------------------------------------------------------------------
int LoadFile(const char * const filename, Mem_File * const p_memfile, DWORD nAccess)
	{
	assert(filename && *filename);
	assert(p_memfile);

	///////////////////////////////////////////////////////////
	// ȷ�����ʷ�ʽ

	DWORD nFileAccess;
	DWORD nPageAccess;
	DWORD nFileMapAccess;
	if (nAccess == 0)		// ֻ��
		{
		nFileAccess = GENERIC_READ;
		nPageAccess = PAGE_READONLY;
		nFileMapAccess = FILE_MAP_READ;
		}
	else if (nAccess == 1)		// ��д
		{
		nFileAccess = GENERIC_READ | GENERIC_WRITE;
		nPageAccess = PAGE_READWRITE;
		nFileMapAccess = FILE_MAP_WRITE;
		}
	else if (nAccess == 2)		// ��д��������
		{
		nFileAccess = GENERIC_READ | GENERIC_WRITE;
		nPageAccess = PAGE_WRITECOPY;
		nFileMapAccess = FILE_MAP_COPY;
		}
	else
		return -1;		// ����ķ��ʷ�ʽ

	///////////////////////////////////////////////////////////
	// �����ڴ�ӳ��

	// ���ļ�
	HANDLE hFile;
	if (nFileAccess == GENERIC_READ)
		hFile = CreateFile_r(filename);
	else
		hFile = CreateFile_w(filename);

	if (hFile == INVALID_HANDLE_VALUE)
		return -2;		// �޷����ļ�

	// ��ȡ�ļ�����
	p_memfile->nSize = GetFileSize(hFile, NULL);
	if (p_memfile->nSize == INVALID_FILE_SIZE)
		{
		CloseHandle(hFile);
		return -3;		// �޷���ȡ�ļ�����
		}
	else if (p_memfile->nSize == 0)
		{
		CloseHandle(hFile);
		p_memfile->pBuffer = NULL;
		p_memfile->nSize = 0;
		return -4;		// �ļ�Ϊ��
		}

	// �����ڴ��ļ�ӳ�����
	HANDLE const hFileMap = CreateFileMapping(hFile, NULL, nPageAccess, 0, 0, NULL);
	if (hFileMap == NULL)
		{
		CloseHandle(hFile);
		return -4;		// �ڴ��ļ�ӳ�������ʧ��
		}

	// �������ļ�ӳ�䵽���̿ռ�
	p_memfile->pBuffer = static_cast<BYTE *>( MapViewOfFile(hFileMap, nFileMapAccess, 0, 0, 0) );
	if (p_memfile->pBuffer == NULL)
		{
		CloseHandle(hFile);
		CloseHandle(hFileMap);
		return -5;		// �������ļ�ӳ�䵽���̿ռ�ʧ��
		}

	// �ر��ļ�������ڴ�ӳ����
	CloseHandle(hFile);
	CloseHandle(hFileMap);
	return 0;
	} // end LoadFile



//-----------------------------------------------------------------------------
// LoadFile_n()
// ���ڴ����½��̶���С���ļ���ʹ���ڴ�ӳ��
// ����ļ�����ַΪ�գ��򴴽������ڴ��ļ�
// ���޸Ĵ����Mem_File���󣬵�����ʧ��ʱ����ֵ������
// �ļ���СΪ0ʱ������ʧ��
//-----------------------------------------------------------------------------
int LoadFile_n(const char * const filename, Mem_File * const p_memfile, DWORD const nFileSize)
	{
	assert( (filename && *filename) || !filename );
	assert(p_memfile);

	if ( (p_memfile->nSize = nFileSize) == 0 )
		return -1;		// �ļ���СΪ0

	// �½��ļ������û�и����ļ����򲻴���
	HANDLE hFile;
	if (filename)
		{
		hFile = CreateFile_n(filename);
		if (hFile == INVALID_HANDLE_VALUE)
			return -2;		// �޷������ļ�
		}
	else
		hFile = INVALID_HANDLE_VALUE;		// CloseHandle()���Ա�������Ч�ľ��

	// �����ڴ��ļ�ӳ�����
	HANDLE const hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, nFileSize, NULL);
	if (hFileMap == NULL)
		{
		CloseHandle(hFile);
		return -3;		// �ڴ��ļ�ӳ�������ʧ��
		}

	// �������ļ�ӳ�䵽���̿ռ�
	p_memfile->pBuffer = static_cast<BYTE *>( MapViewOfFile(hFileMap, FILE_MAP_WRITE, 0, 0, 0) );
	if (p_memfile->pBuffer == NULL)
		{
		CloseHandle(hFile);
		CloseHandle(hFileMap);
		return -4;		// �������ļ�ӳ�䵽���̿ռ�ʧ��
		}

	// �ر��ļ�������ڴ�ӳ����
	CloseHandle(hFile);
	CloseHandle(hFileMap);
	return 0;
	} // end LoadFile_n
