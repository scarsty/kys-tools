//-----------------------------------------------------------------------------
// �ļ�ϵͳ���
// ���ļ������ڴ�ʱֻʹ���ڴ��ļ�ӳ��ķ�������ʹ�ø��Ƶ������ڴ�ķ���
//-----------------------------------------------------------------------------

#ifndef FH_FILE
#define FH_FILE

// INCLUDES ///////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// DEFINES ////////////////////////////////////////////////////////////////////
// MACROS /////////////////////////////////////////////////////////////////////
// TYPES //////////////////////////////////////////////////////////////////////

// �ڴ��е��ļ���Ϣ
typedef struct _Mem_File
	{
	BYTE *pBuffer;
	DWORD nSize;
	} Mem_File;

// CLASS //////////////////////////////////////////////////////////////////////
// PROTOTYPES /////////////////////////////////////////////////////////////////

inline HANDLE CreateFile_r(const char *filename);
inline HANDLE CreateFile_n(const char *filename);
inline HANDLE CreateFile_w(const char *filename);
inline void UnloadFile(Mem_File *p_memfile);
int Set_Process_Dir_to_Exe_Dir();
int GetExeFileName_WithExt(char *filename, const char *ext);
int ChangeFilenameExt(char *filename, const char *ext);
int LoadFile(const char *filename, Mem_File *p_memfile, DWORD nAccess);
int LoadFile_n(const char *filename, Mem_File *p_memfile, DWORD nFileSize);
inline void UnloadFile(Mem_File *p_memfile);
inline void UnloadFile_n(Mem_File *p_memfile);

// GLOBALS ////////////////////////////////////////////////////////////////////
// FUNCTIONS //////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// CreateFile_r()
// ��ĳ���Ѵ����ļ���������ȡ���������
// �ļ���������������
// ���ú�Ҫ��鷵�صľ���Ƿ�ΪINVALID_HANDLE_VALUE
//-----------------------------------------------------------------------------
inline HANDLE CreateFile_r(const char * const filename)
	{
	return CreateFile(filename,
					  GENERIC_READ,
					  FILE_SHARE_READ,
					  NULL,
					  OPEN_EXISTING,
					  FILE_ATTRIBUTE_NORMAL,
					  NULL);
	} // end CreateFile_r



//-----------------------------------------------------------------------------
// CreateFile_n()
// �½�ĳ���ļ�������д������
// ԭ�ļ�������ڻᱻ����
// ���ú�Ҫ��鷵�صľ���Ƿ�ΪINVALID_HANDLE_VALUE
//-----------------------------------------------------------------------------
inline HANDLE CreateFile_n(const char * const filename)
	{
	return CreateFile(filename,
					  GENERIC_READ | GENERIC_WRITE,
					  FILE_SHARE_READ,
					  NULL,
					  CREATE_ALWAYS,
					  FILE_ATTRIBUTE_NORMAL,
					  NULL);
	} // end CreateFile_n



//-----------------------------------------------------------------------------
// CreateFile_w()
// �򿪻򴴽�ĳ���ļ�������д������
// ����ļ��Ѵ��ڣ����Զ�ȡ����
// ���ú�Ҫ��鷵�صľ���Ƿ�ΪINVALID_HANDLE_VALUE
//-----------------------------------------------------------------------------
inline HANDLE CreateFile_w(const char * const filename)
	{
	return CreateFile(filename,
					  GENERIC_READ | GENERIC_WRITE,
					  FILE_SHARE_READ,
					  NULL,
					  OPEN_ALWAYS,
					  FILE_ATTRIBUTE_NORMAL,
					  NULL);
	} // end CreateFile_w



//-----------------------------------------------------------------------------
// UnloadFile()
// �ͷŶ����ڴ���ļ���ȡ���ļ�ӳ��
// ����иĶ�������д��Ӳ�̵Ļ����Ķ��ᱻд��Ӳ��
//-----------------------------------------------------------------------------
inline void UnloadFile(Mem_File * const p_memfile)
	{
	assert(p_memfile);

	if (p_memfile->pBuffer)
		{
		UnmapViewOfFile(p_memfile->pBuffer);		// ȡ���ļ�ӳ��Ҳ���ܳ����������
		p_memfile->pBuffer = NULL;
		p_memfile->nSize = 0;
		}
	} // end UnloadFile



//-----------------------------------------------------------------------------
// UnloadFile_n()
// ������ļ��ĸ��ģ��ͷ��ڴ棬ȡ���ļ�ӳ��
//-----------------------------------------------------------------------------
inline void UnloadFile_n(Mem_File * const p_memfile)
	{
	UnloadFile(p_memfile);
	}

#endif // FH_FILE