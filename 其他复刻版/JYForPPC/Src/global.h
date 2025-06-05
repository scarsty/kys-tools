#ifndef GLOBAL_H
#define GLOBAL_H

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif
#define                   HONGCOLOR          21
#define                   ZHICOLOR          79
#define					  TRANSCOLOR         232
#define					  BAICOLOR         101
#define					  HUANGCOLOR         44
#define                   BACKCOLOR          0
#define                   WARDATASIZE        186//ս�����ݴ�С  war.sta���ݽṹ
#define XSCALE 18
#define YSCALE 9
#pragma pack(push,1)
enum JYPERSONSTATUS
{
    enum_sex = 1,//�Ա�
	enum_grade = 2,//�ȼ�
	enum_exp = 3,//����
	enum_hp = 4,//����
	enum_hpMax = 5,//��������
	enum_ShouShang = 6,//���˳̶�
	enum_ZhongDu = 7,//�ж��̶�
	enum_Tili = 8,//����
	enum_WuPinXiuLian = 9,//��Ʒ����
	enum_WuQi = 10,//����
	enum_Fangju = 11,//����
	enum_NeiliXingZhi = 12,//��������
	enum_Neili = 13,//����
	enum_NeiliMax = 14,//��������
	enum_GongJiLi = 15,//������
	enum_QingGong = 16,//�Ṧ
	enum_FangYuLi = 17,//������
	enum_YiLiao = 18,//ҽ��
	enum_YongDu = 19,//�ö�
	enum_JieDu = 20,//�ⶾ
	enum_KangDu = 21,//����
	enum_QuanZhang = 22,//ȭ��
	enum_YuJian = 23,//����
	enum_ShuaDao = 24,//ˣ��
	enum_TeSHuBingQi = 25,//�������
	enum_AnQiJiQiao = 26,//��������
	enum_WuXueChangShi = 27,//��ѧ��ʶ
	enum_PinDe = 28,//Ʒ��
	enum_GongjiDaiDu = 29,//��������
	enum_ZuoYouHuBo = 30,//���һ���
	enum_ShengWang = 31,//����
	enum_ZiZhi = 32,//����
	enum_XiuLianWuPin = 33,//������Ʒ
	enum_XiuLianDianShu = 34,//��������
	enum_XueHuiWuGong=35,//ѧ���书
};

typedef struct UKeyList
{
	int uK_up, uK_down, uK_left, uK_right, uK_esc, uK_ok;
	int uK_e, uK_w, uK_r, uK_a;
	BOOL bVkey;
} DefinedKey;

typedef struct Building_Type{   
	int x;
	int y;
	int num;
}BuildingType;

//�¼�����
typedef struct tagEventType
{
	short isGo;
	short id;
	short EventNum1;
	short EventNum2;
	short EventNum3;
	short PicNum[3];
	short PicDelay;
	short x;
	short y;
}EVENTTYPE, *LPEVENTTYPE;

//�����¼�
typedef struct tagKdefType
{
	short iDataLength;
	short *pData;
	short iNumlabel;
}KDEFTYPE, *LPKDEFTYPE;
//��������
typedef struct tagBaseAttrib
{
	short ChengChuang;//�˴�
	short WeiZhi;//����
	short WMapX;//���ͼ�˺�����X
	short WMapY;//���ͼ�˺�����Y
	short SMapX;//���볡���ĺ�����X
	short SMapY;//���볡���ĺ�����Y
	short Way;//����Է���
	short BoatX;//��X
	short BoatY;//��Y
	short BoatX1;//��X1
	short BoatY1;//��Y1
	short BoatWay;//
	short Group[6];//����
	short WuPin[200][2];
}BASEATTRIB,*LPBASEATTRIB;
//��������
typedef struct tagPersonAttrib
{
	short id;//����
	short PhotoId;//ͷ��
	short HpAdd;//��������
	short r4;//����
	BYTE name1big5[10];//����
	BYTE name2big5[10];//���
	short sex;//�Ա�
	short grade;//�ȼ�
	ushort exp;//����
	short hp;//����
	short hpMax;//��������
	short ShouShang;//���˳̶�
	short ZhongDu;//�ж��̶�
	short Tili;//����
	ushort WuPinXiuLian;//��Ʒ����
	short WuQi;//����
	short Fangju;//����
	short ChuZhaoDongHuaZhenShu[5];//���ж���֡��/ҽ���ö��ⶾ/ȭ/��/��/����
	short ChuZhaoDongHuaYanChi[5];//���ж����ӳ�/ҽ���ö��ⶾ/ȭ/��/��/����
	short WuGongYinXiaoYanChi[5];//�书��Ч�ӳ�/ҽ���ö��ⶾ/ȭ/��/��/����
	short NeiliXingZhi;//��������
	short Neili;//����
	short NeiliMax;//��������
	short GongJiLi;//������
	short QingGong;//�Ṧ
	short FangYuLi;//������
	short YiLiao;//ҽ��
	short YongDu;//�ö�
	short JieDu;//�ⶾ
	short KangDu;//����
	short QuanZhang;//ȭ��
	short YuJian;//����
	short ShuaDao;//ˣ��
	short TeSHuBingQi;//�������
	short AnQiJiQiao;//��������
	short WuXueChangShi;//��ѧ��ʶ
	short PinDe;//Ʒ��
	short GongjiDaiDu;//��������
	short ZuoYouHuBo;//���һ���
	short ShengWang;//����
	short ZiZhi;//����
	short XiuLianWuPin;//������Ʒ
	ushort XiuLianDianShu;//��������
	short WuGong[10];//�书
	short WuGongDengji[10];//�书
	short XieDaiWuPin[4];//Я����Ʒ
	short XieDaiWuPinShuLiang[4];//Я����Ʒ����
}PERSONATTRIB, *LPPERSONATTRIB;
//��Ʒ����
typedef struct tagThingsAttrib
{
	short id;//����
	BYTE name1big5[20];//��Ʒ��
	BYTE name2big5[20];//��Ʒ��
	BYTE ShuoMingbig5[30];//˵��
	short LianChuWuGong;//�����书
	short AnQiDongHuaBianHao;//�����������
	short ShiYongRen;//ʹ����
	short ZhuangBeiLeiXing;//װ������
	short XianShiWuPin;//��ʾ��Ʒ˵��
	short LeiXing;//����
	short WeiZhi[3];//δ֪
	short JiaShengMing;//������
	short JiaSHengMingZuiDa;//���������ֵ
	short JiaZhongDuJieDu;//���ж��ⶾ
	short JiaTiLi;//������
	short GaiBianNeiLiXingZhi;//�ı���������
	short JiaNeiLi;//������
	short JiaNeiLiZuiDa;//���������ֵ
	short JiaGongJiLi;//�ӹ�����
	short JiaQingGong;//���Ṧ
	short JiaFangYuLi;//�ӷ�����
	short JiaYiLiao;//��ҽ��
	short JiaShiDu;//��ʹ��
	short JiaJieDu;//�ӽⶾ
	short JiaKangDu;//�ӿ���
	short JiaQuanZhang;//��ȭ��
	short JiaYuJian;//������
	short JiaShuaDao;//��ˣ��
	short JiaTeShuBingQi;//���������
	short JiaAnQiJiQiao;//�Ӱ�������
	short JiaWuXueChangShi;//����ѧ��ʶ
	short JiaPinDe;//��Ʒ��
	short JiaGongJiCiShu;//�ӹ�������
	short JiaGongFuDaiDu;//�ӹ������
	short JinXiuLianRenWu;//����������
	short XuNeiLiXingZhi;//����������
	short XuNeiLi;//������
	short XuGongJiLi;//�蹥����
	short XuQingGong;//���Ṧ
	short XuShiDu;//��ʹ��
	short XuYiLiao;//��ҽ��	
	short XuJieDu;//��ⶾ
	short XuQuanZhang;//��ȭ��
	short XuYuJian;//������
	short XuShuaDao;//��ˣ��
	short XuTeShuBingQi;//���������
	short XuAnQiJiQiao;//�谵������
	short XuZiZHi;//������
	short XuExpLianChengMiJi;//����������Ҫ����
	short XuExpLianChuWuPin;//������Ʒ�辭��
	short XuCaiLiao;//�����
	short LianChuWuPin[5];//������Ʒ
	short XuYaoWuPinShuLiang[5];//��Ҫ��Ʒ����	
}THINGSATTRIB, *LPTHINGSATTRIB;
//����
typedef struct tagSceneType
{
	short SceneID;
	BYTE Name1[10];
	short OutMusic;
	short InMusic;
	short JumpScene;
	short InCondition;
	short MMapInX1;
	short MMapInY1;
	short MMapInX2;
	short MMapInY2;
	short InX;
	short InY;
	short OutX[3];
	short OutY[3];
	short JumpX1;
	short JumpY1;
	short JumpX2;
	short JumpY2;
}SCENETYPE, *LPSCENETYPE;
//�书����
typedef struct tagWuGongAttrib
{
	short ChengChuang;//����
	BYTE Name1[10];//����
	short WeiZhi[5];//����
	short ChuZhaoYinXiao;//������Ч
	short WuGongLeiXing;//�书����
	short WuGongDongHua;//�书����&��Ч
	short ShangHaiLeiXing;//�˺�����
	short GongJiFanWei;//������Χ
	short XiaoHaoNeiLi;//������������
	short DiRenZhongDu;//�����ж�����
	short GongJiLi[10];//������
	short YiDongFanWei[10];//�ƶ���Χ
	short ShaShangFanWei[10];//ɱ�˷�Χ
	short JiaNeiLi[10];//������
	short ShaShangNeiLi[10];//ɱ������
}WUGONGATTRIB,*LPWUGONGATTRIB;
//С���̵�
typedef struct tagXiaoBaoAttrib
{
	short WuPin[5];//��Ʒ
	short WuPinShuLiang[5];//����
	short WuPinJiaGe[5];//�۸�
}XIAOBAOATTRIB,*LPXIAOBAOGATTRIB;

//ս����Ա��Ϣ
typedef struct tagWarGroup
{
	short iPerson;	
	BOOL  bSelf;
	short x;
	short y;
	BOOL  bDie;
	short iWay;
	short iPicCode;
	short iPicType;
	short iQingGong;
	short iYiDongBushu;
	short iDianShu;
	short iExp;
	short iAutoFoe;
}WARGROUP;

//ս��sta
typedef struct tagWarSta
{
	short iWarId;
	BYTE  Name[10];
	short iMapId;
	ushort iExp;
	short iMusic;
	short WarManualPerson[6];
	short WarAutoPerson[6];
	short PersonX[6];
	short PersonY[6];

	short WarfoePerson[20];
	short foeX[20];
	short foeY[20];
}WARSTA,*LPWARSTA;
//���ƶ�����
typedef struct tagMoveStep
{
	short x[100];
	short y[100];
	short num;
}MOVESTEP,*LPMOVESTEP;
//ս��
typedef struct tagWarAttrib
{
	short iWarId;//
	short *pSData;//
	LPWARSTA pWarSta;//
	WARGROUP warGroup[26];//
	short iPersonNum;//
	short iAutoFight;//
	short iCurID;//
	BOOL  bShowHead;//
	short iEffect;//
	INT   iEffectColor[7];//
	short SelectPerson[6][2];//��ս��Ա
	BOOL  bSelectOther;//�Ƿ����ѡ���Ա
	LPMOVESTEP pMoveStep;//�ƶ�����
	LPMOVESTEP pAttackStep;//��������
	short LastPerson;//���һ���ж���Ա
	short iRound;//����ʱ�Ƿ�����غ�
	short iType;//����Ƿ�����

	short iFlagMaxShaShangPerson;//�����ж���ɱ�������
	short iFlagMaxYiLiaoPerson;//�����ж���ҽ�������
	short iFlagMaxShouShangPerson;//���������
	short iFlagMaxJieDuPerson;//�����ж��Ľⶾ�����
	short iFlagMaxZhongDuPerson;//�ж������
	short iFlagMinFoePerson;//�з�������

}WARATTRIB,*LPWARATTRIB;

typedef struct tagFILES
{
	FILE            *fpMmapGrp; //�⾰��ͼ��
	FILE            *fpSmapGrp; //�ھ���ͼ��

	FILE            *fpTalkIdx; //�Ի�����
	FILE            *fpTalkGrp; //�Ի�
	
	FILE            *fpHdGrpGrp;//ͷ������

	FILE            *fpWmapGrp;//ս����ͼ��
	FILE            *fpEftGrp;//�书Ч����	

	FILE            *fpFightGrp;//�书������	
} FILES, *LPFILES;

typedef struct tagGameData
{
	WARATTRIB   war;
	//
	INT			iSceneNum;	//��ǰ�������
	INT         iSceneEventCode;
	LPKDEFTYPE pKdefList;//�����¼��ű�
	INT         iKdefListCount;
	BOOL       bLoadKdef;
	//---------------------------------R�浵����Ϣ
	LPPERSONATTRIB pPersonList;//��������
	INT         iPersonCount;
	LPSCENETYPE pSceneTypeList;//����������Ϣ
	INT         iSceneTypeListCount;
	LPBASEATTRIB pBaseList;//��������
	LPTHINGSATTRIB pThingsList;//��Ʒ����
	INT         iThingsListCount;
	INT         iHaveThingsNum;
	INT         iLastUseThingNum;//�ϴ�ʹ����Ʒλ��
	INT         iLastUseSystemNum;//�ϴ�ϵͳ�˵�λ��
	INT         iLastUseTransNum;//�ϴδ���λ��
	LPXIAOBAOGATTRIB pXiaoBaoList;//С���̵�
	LPWUGONGATTRIB pWuGongList;//�书����
	INT         iWuGongCount;
    //-----------------------------------

	//---------------------------------D�浵����Ϣ
	//EVENTTYPE sceneEventList[100][200];//�����¼��б�
	short *pD;
	//-----------------------------------
	
	//---------------------------------S
	short *pScene;
	//-----------------------------------

	//---------------------------------���ͼ����
	short *pEarth;
	short *pSurface;
	short *pBuilding;
	short *pBuildX;
	short *pBuildY;
	BOOL  bLoadMmap;
	short iXScale;
	short iYScale;
	short iMMapAddX;
	short iMMapAddY;
	short iSMapAddX;
	short iSMapAddY;
	short iWMapAddX;
	short iWMapAddY;
	BuildingType Build[2000];        // ������������
	int iBuildNumber;     //ʵ���������
	//---------------------------------
	INT             Status;
	PERSONATTRIB Hero;

	short iMyCurrentPic;
	short iMyPic;              //���ǵ�ǰ��ͼ
    short iMytick;             //����û����·�ĳ���֡��
	short iDirectX[4];
	short iDirectY[4];
	//---------------------------------
	short iSubSceneX;
	short iSubSceneY;

} GAMEDATA,*LPGAMEDATA;

typedef struct tagGLOBALVARS
{
	FILES            f;
	GAMEDATA         g;
	INT             bCurrentSaveSlot;    // current save slot (1-5)
	DefinedKey		curDefKey;
	BOOL             fGameStart;
	BOOL             fGameLoad;
	BOOL             fGameSave;
} GLOBALVARS, *LPGLOBALVARS;
#pragma pack(pop)
extern INT g_CharSet;
extern LPGLOBALVARS gpGlobals;

BOOL GetIniValue(const char* file, const char* session,const char* key,const char*defvalue, char* value);
BOOL GetIniField(FILE* fp, const char* session,const char* key,const char*defvalue, char* value);
int ReadLine(FILE *fp, char *buffer, int maxlen);
char** spilwhit(char* str, const char* strsp, DWORD* buf, int buflen, int* ct);
int FindChars(const char* p, const char* f, BOOL br);
void JY_DrawVKeys(void* pixels);
void ClearBuf(LPBYTE buf,int ilen);
INT JY_InitGlobals(VOID);
VOID JY_FreeGlobals(VOID);
VOID JY_InitGameData(INT iSaveSlot);
VOID JY_LoadSaveSlot(INT iSaveSlot);
INT JY_LoadAllEvent(VOID);
INT JY_LoadMMap(VOID);
INT JY_LoadScene(VOID);
INT limitX(INT x, INT xmin, INT xmax);
VOID JY_TRANS(VOID);
VOID JY_TRANS_DATA(LPSTR fileName,LPCSTR fileidxName,INT iType);
INT JY_LoadKdef(VOID);
VOID JY_SaveSaveSlot(INT iSaveSlot);
INT JY_LoadWarScene(VOID);
#ifdef __cplusplus
}
#endif

#endif