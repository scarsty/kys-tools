
//#include "font.h"
//#include "ascii.h"
//#include "util.h"
#include "Main.h"

TTF_Font *gpFont = NULL;

//�ַ���ת������
static Uint16 gbkj_f[128][256];        //GBK����-->����
static Uint16 gbkf_j[128][256] ;
static Uint16 gbk_unicode[128][256] ;
static Uint16 gbk_big5[128][256] ;
static Uint16 big5_gbk[128][256] ;
static BYTE big5_gbk0[2][256][129] ;//��-��
static SDL_RWops *pRwop=NULL;
static LPBYTE pFontbuf = NULL;
//��ʼ������
INT JY_InitFont(VOID)
{
	if (TTF_WasInit() == 0)
	{
		if ( TTF_Init() == -1)
			TerminateOnError(SDL_GetError());
	}
	LPSTR cfont = va("%s%s", JY_PREFIX, "\\Resource\\font\\font.ttf");
	
	////
	gpFont = TTF_OpenFont(cfont, g_iFontSize);
	if (gpFont == NULL)//��֧������·��
	{
		FILE *fpFont = NULL;
		fpFont = fopen(cfont,"rb");
		if (fpFont == NULL)
		{
			TerminateOnError("�Ҳ����ļ�: \\Resource\\font\\font.ttf!\n");
		}
		INT iLen = 0;
		fseek(fpFont, 0, SEEK_END);
		iLen = ftell(fpFont);
		fseek(fpFont, 0, SEEK_SET);
		pFontbuf = (LPBYTE)malloc(iLen);
		if (pFontbuf == NULL)
		{
			TerminateOnError("���������ڴ�ʧ��!\n");
		}
		iLen = fread(pFontbuf, 1, iLen, fpFont);
		fclose(fpFont);
		fpFont = NULL;

		pRwop = SDL_RWFromMem((void*)pFontbuf,iLen);
		if (pRwop == NULL)
		{
			TerminateOnError("ת�������ڴ�ʧ��\n%s",SDL_GetError());
		}
		gpFont = TTF_OpenFontRW(pRwop,0,g_iFontSize);
		if (gpFont == NULL)
		{
			TerminateOnError("ת������ʧ��\n%s",SDL_GetError());
		}
	}
	SafeFree(cfont);
	LoadMB();

	return 0;
}

//�ͷ�����
VOID JY_FreeFont(VOID)
{
	TTF_CloseFont(gpFont);
	gpFont = NULL;
	SafeFree(pFontbuf);
	SafeFree(pRwop);
	atexit(TTF_Quit);
}

//��������ļ�
//����ļ�����GBK˳������ÿ��GBK�ַ���Ӧ�����ַ���gbkf,big5,unicode
INT LoadMB(VOID)
{
	FILE *fp;
	int i,j;

	Uint16 gbk,gbkf,big5,unicode;
	char cFile[256] = {0};
	sprintf(cFile,"%s\\Resource\\data\\mb.dat",JY_PREFIX);
	fp = fopen(cFile,"rb");
	//fp = UTIL_OpenRequiredFile("\\Resource\\data\\mb.dat");
	if (fp == NULL)
		TerminateOnError("���ļ�ʧ��%s!\n", cFile);
	for(i=0;i<128;i++){
		for(j=0;j<256;j++){
			gbkj_f[i][j]=0;
			gbkf_j[i][j]=0;
			gbk_unicode[i][j]=0;
 
			gbk_big5[i][j]=0;
			big5_gbk[i][j]=0;
		}
	}

    for(i=0x81;i<=0xfe;i++)
		for(j=0x40;j<=0xfe;j++){
			if( j != 0x7f){
				gbk=i+j*256;
				fread(&gbkf,2,1,fp);
				fread(&big5,2,1,fp);
				fread(&unicode,2,1,fp);
                
				if(gbk!=gbkf){
 				    gbkj_f[i-128][j]=gbkf;
				    gbkf_j[(gbkf&0xff)-128][(gbkf&0xff00)>>8]=gbk;
				}
				gbk_unicode[i-128][j]=unicode;

				if(gbkj_f[i-128][j]==0){   //û�м���
				    gbk_big5[i-128][j]=big5;
				    big5_gbk[(big5 & 0xff)-128][( big5 &0xff00)>>8]=gbk;
				}
            }
		}

	UTIL_CloseFile(fp);
    sprintf(cFile,"%s\\Resource\\data\\mbfj.dat",JY_PREFIX);
	fp = fopen(cFile,"rb");
	//fp = UTIL_OpenRequiredFile("\\Resource\\data\\mbfj.dat");
	if (fp == NULL)
		TerminateOnError("���ļ�ʧ��%s!\n", cFile);
	BYTE btTemp0 = 0;
	BYTE btTemp1 = 0;
	BYTE btTemp2 = 0;
	BYTE btTemp3 = 0;
	for(int i=0xA0;i<=0xFE;i++)
	{
		for(int j=0x40;j<=0xFE;j++)
		{
			if (j<=0x7E || j >=0xA1)
			{
				fread(&btTemp0, 1, 1, fp);
				fread(&btTemp1, 1, 1, fp);
				big5_gbk0[0][j - 0x40][i - 0xA0] = btTemp0;				
				big5_gbk0[1][j - 0x40][i - 0xA0] = btTemp1;
			}
		}
	}
	UTIL_CloseFile(fp);

    return 0;
}

// �����ַ���ת��
// flag = 0   Big5 --> GBK     
//      = 1   GBK  --> Big5    
//      = 2   Big5 --> Unicode
//      = 3   GBK  --> Unicode
// ע��Ҫ��֤dest���㹻�Ŀռ䣬һ�㽨��ȡsrc���ȵ�����+1����֤ȫӢ���ַ�Ҳ��ת��Ϊunicode
INT  JY_CharSet(const char *src, char *dest, INT flag)
{
 
    char *psrc,*pdest;
    unsigned char b0,b1;
	INT d0;
	Uint16 tmpchar;

	psrc=(char*)src;
	pdest=dest;

    while(1){
        b0=*psrc;
		if(b0==0){       //�ַ�������
			if( (flag==0) || (flag==1) ){
				*pdest=0;
				break;
			}
			else{    //unicode������־ 0x0000?
				*pdest=0;
				*(pdest+1)=0;
				break;                
			}
		}
		if(b0<128){      //Ӣ���ַ�
			if( (flag==0) || (flag==1) ){  //��ת��
				*pdest=b0;
				pdest++;
				psrc++;
			}
			else{                //unicode ����Ӹ�0
				*pdest=b0;
				pdest++;
				*pdest=0;
				pdest++;
				psrc++;                
			}
		}
		else{              //�����ַ�
			b1=*(psrc+1);
            if(b1==0){     // ����������
                *pdest=' ';//*pdest='?';
				*(pdest+1)=0;
				break;
			}
			else{
				d0=b0+b1*256;
				switch(flag){
				case 0:   //Big5 --> GBK    
					//tmpchar=big5_gbk[d0];
					tmpchar=big5_gbk[b0-128][b1];
					if(gbkf_j[(tmpchar & 0xff)-128][( tmpchar &0xff00)>>8]>0)
					    tmpchar=gbkf_j[(tmpchar & 0xff)-128][( tmpchar &0xff00)>>8];
					break;
				case 1:   //GBK  --> Big5  
					if(gbkj_f[b0-128][b1]>0)
					    tmpchar=gbkj_f[b0-128][b1];
					else
						tmpchar=d0;
											
					tmpchar=gbk_big5[(tmpchar & 0xff)-128][( tmpchar &0xff00)>>8];
					break;
				case 2:   //Big5 --> Unicode
					tmpchar=big5_gbk[b0][b1];
					tmpchar=gbk_unicode[(tmpchar & 0xff)-128][( tmpchar &0xff00)>>8];
					break;
				case 3:   //GBK  --> Unicode
					tmpchar=gbk_unicode[b0-128][b1];
					break;                
				}

                *(Uint16*)pdest=tmpchar;

				pdest=pdest+2;
				psrc=psrc+2;
			}
        }
	}

    return 0;
}

//�����ַ���ת��
VOID Big5toUnicode(LPBYTE buf,int len)
{
	int i=0;
	while(i < len -1)
	{
		BYTE bb =buf[i];
		if (bb == 0)break;
		if (bb > 128)
		{
			BYTE bb1 = 0;
			bb1 =buf[i+1];
			BYTE bf0 = big5_gbk0[0][bb1 - 0x40][bb - 0xA0];//(b0, b1, 0)
            BYTE bf1 = big5_gbk0[1][bb1 - 0x40][bb - 0xA0];
			if (bf0 > 0 || bf1 > 0)
			{
				buf[i] = bf0;
				buf[i+1] = bf1;
			}
			i+=2;
		}
		else
		{
			i+=1;
		}
	}
}