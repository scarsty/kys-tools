
/**************************************************************

��ӡ����DirectMusic��ش���

�޸Ĺ��ĵط���
������������Ϣ��pcm_sound
ɾȥpcm_sound::state���䳣�����壬��pcm_sound::dsbuffer�����书��

**************************************************************/

// watch for multiple inclusions
#ifndef DXLIB_DS
#define DXLIB_DS

// DEFINES ////////////////////////////////////////////////


//#define DM_NUM_SEGMENTS	64 // number of midi segments that can be cached in memory
//
//// midi object state defines
//#define MIDI_NULL     0   // this midi object is not loaded
//#define MIDI_LOADED   1   // this midi object is loaded
//#define MIDI_PLAYING  2   // this midi object is loaded and playing
//#define MIDI_STOPPED  3   // this midi object is loaded, but stopped

#define MAX_SOUNDS		256 // max number of sounds in system at once 

// directx 7.0 compatibility

#ifndef DSBCAPS_CTRLDEFAULT
#define DSBCAPS_CTRLDEFAULT (DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME )
#endif

// MACROS /////////////////////////////////////////////////

#define DSVOLUME_TO_DB(volume) ((DWORD)(-30*(100 - volume)))

// Convert from multibyte format to Unicode using the following macro:
#define MULTI_TO_WIDE( x,y )  MultiByteToWideChar( CP_ACP,MB_PRECOMPOSED, y,-1,x,_MAX_PATH);

// initializes a direct draw struct
#define DD_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }


// TYPES //////////////////////////////////////////////////

// ������������Ϣ
typedef struct pcm_sound_typ
	{
	LPDIRECTSOUNDBUFFER dsbuffer;   // �������ݽ���ָ��
	//int rate;                       // ����Ƶ�ʣ�ûʲô�ô�
	//int size;                       // ���ݴ�С��ûʲô�ô�
	//int id;                         // ������ţ���Զ������������е������ͬ��ûʲô�ô�
	int pause;						// ��������ͣ�Ĵ�����ÿ��ͣһ�μ�һ��0Ϊ����ͣ״̬
	DWORD play_flags;				// ����ʱ�Ĳ���dwFlags����¼������ȡ����ͣʱ����������
	} pcm_sound, *pcm_sound_ptr;

//// directmusic MIDI segment
//typedef struct DMUSIC_MIDI_TYP
//{
//IDirectMusicSegment        *dm_segment;  // the directmusic segment
//IDirectMusicSegmentState   *dm_segstate; // the state of the segment
//int                        id;           // the id of this segment               
//int                        state;        // state of midi song
//
//} DMUSIC_MIDI, *DMUSIC_MIDI_PTR;


// PROTOTYPES /////////////////////////////////////////////

// directsound
int DSound_Load_WAV(char *filename, int control_flags = DSBCAPS_CTRLDEFAULT);
int DSound_Replicate_Sound(int source_id);
int DSound_Play(int id, DWORD flags=0, int volume=0, int rate=0, int pan=0);
inline int DSound_Stop_Sound(int id);
int DSound_Stop_All_Sounds(void);
int DSound_Init(HWND hWnd);
int DSound_Shutdown(void);
inline int DSound_Delete_Sound(int id);
int DSound_Delete_All_Sounds(void);
int DSound_Status_Sound(int id);
int DSound_Set_Volume(int id,int vol);
int DSound_Set_Freq(int id,int freq);
int DSound_Set_Pan(int id,int pan);
inline int DSound_Pause_Sound(int id);
int DSound_Pause_All_Sounds(void);
inline int DSound_Unpause_Sound(int id);
int DSound_Unpause_All_Sounds(void);

//// directmusic
//int DMusic_Load_MIDI(char *filename);
//int DMusic_Play(int id);
//int DMusic_Stop(int id);
//int DMusic_Shutdown(void);
//int DMusic_Delete_MIDI(int id);
//int DMusic_Delete_All_MIDI(void);
//int DMusic_Status_MIDI(int id);
//int DMusic_Init(HWND hWnd);

// GLOBALS ////////////////////////////////////////////////


// EXTERNALS //////////////////////////////////////////////

extern LPDIRECTSOUND		lpds;           // directsound interface pointer
extern DSBUFFERDESC			dsbd;           // directsound description
extern DSCAPS				dscaps;         // directsound caps
extern HRESULT				dsresult;       // general directsound result
extern DSBCAPS				dsbcaps;        // directsound buffer caps

extern LPDIRECTSOUNDBUFFER	lpdsbprimary;   // the primary mixing buffer
extern pcm_sound			sound_fx[MAX_SOUNDS];    // the array of secondary sound buffers

extern WAVEFORMATEX			pcmwf;          // generic waveformat structure

//// direct music globals
//extern IDirectMusicPerformance    *dm_perf ;    // the directmusic performance manager 
//extern IDirectMusicLoader         *dm_loader;  // the directmusic loader
//
//// this hold all the directmusic midi objects
//extern DMUSIC_MIDI                dm_midi[DM_NUM_SEGMENTS];
//extern int dm_active_id;                               // currently active midi segment

#endif // DXLIB_DS