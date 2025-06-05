// ��ȡidx/grp����ͼ�ļ���
// Ϊ����ٶȣ����û��淽ʽ��ȡ����idx/grp�����ڴ棬Ȼ�������ɸ��������
// �������ʵ�pic���ڻ��������
#include "Main.h"

static struct PicFileCache pic_file[PIC_FILE_NUM];     

LIST_HEAD(cache_head);             //����cache����ͷ

static INT currentCacheNum=0;      // ��ǰʹ�õ�cache��

static INT g_MAXCacheNum = 1000;          // ���Cache����
extern WORD g_wInitialWidth;
extern WORD g_wInitialHeight;
extern WORD g_wInitialHeight;

INT CacheFailNum=0;

// ��ʼ��Cache���ݡ���Ϸ��ʼʱ����
INT Init_Cache(VOID)
{
    INT i = 0;
    for(i=0;i<PIC_FILE_NUM;i++){
        pic_file[i].num =0;
        pic_file[i].idx =NULL;
        pic_file[i].pcache=NULL;
    }
    return 0;
}
// ��ʼ����ͼcache��Ϣ
// ������ͼ/����/ս���л�ʱ����
INT JY_PicInit(VOID)
{
    struct list_head *pos,*p;
    int i;
 
	//MEMORYSTATUS   buffer; 
	//GlobalMemoryStatus(&buffer);
	//INT i1 = buffer.dwTotalPhys/1024;
	//INT i2 = buffer.dwAvailPhys/1024;

    //�������Ϊ�գ�ɾ��ȫ������
    list_for_each_safe(pos,p,&cache_head){
        struct CacheNode *tmp= list_entry(pos, struct CacheNode , list);
        if(tmp->s!=NULL) 
			SafeFreeSdlSurface(tmp->s);       //ɾ������
		list_del(pos);		 
		SafeFree(tmp);
	}

    for(i=0;i<PIC_FILE_NUM;i++){
        pic_file[i].num =0;
        SafeFree(pic_file[i].idx);
        SafeFree(pic_file[i].pcache);
    }

	//GlobalMemoryStatus(&buffer);
	//INT i3 = buffer.dwTotalPhys/1024;
	//INT i4 = buffer.dwAvailPhys/1024;

    currentCacheNum=0; 
    CacheFailNum=0;
    return 0;
}

// �����ļ���Ϣ
// filename �ļ���  ����Ҫ��׺��idx/grp
// id  0-9
INT JY_PicLoadFile(const char*filename, INT id)
{
    INT i;
    struct CacheNode *tmpcache;
    char str[255];
    FILE *fp;
    INT count;

    if (id<0 || id>=PIC_FILE_NUM)
	{  // id������Χ
        return 1;
	}

	if(pic_file[id].pcache){        //�ͷŵ�ǰ�ļ�ռ�õĿռ䣬������cache
		INT i;
		for(i=0;i<pic_file[id].num;i++){   //ѭ��ȫ����ͼ��
            tmpcache=pic_file[id].pcache[i];
            if(tmpcache){       // ����ͼ�л�����ɾ��
			    if(tmpcache->s!=NULL) 
				    SDL_FreeSurface(tmpcache->s);       //ɾ������
			    list_del(&tmpcache->list);              //ɾ������
			    SafeFree(tmpcache);                  //�ͷ�cache�ڴ�
                currentCacheNum--;
            }
		}
        SafeFree(pic_file[id].pcache);
    }
    SafeFree(pic_file[id].idx);

    // ��ȡidx�ļ�
    sprintf(str,"%s%s.idx",JY_PREFIX,filename);
	
	fp = fopen(str,"rb");//UTIL_OpenRequiredFile(str);
	if (fp==NULL)
		TerminateOnError("JY_PicLoadFile: ���ļ�ʧ��%s!\n", str);
	pic_file[id].num =JY_GetFileLength(fp) / 4 / 6;
    pic_file[id].idx =(INT *)malloc((pic_file[id].num+1)*4*6);
    if(pic_file[id].idx ==NULL){
		TerminateOnError("JY_PicLoadFile: �����ڴ�ʧ��%s!\n", str);
		return 1;
    }

    count=fread(pic_file[id].idx,4,pic_file[id].num * 6,fp);
	//count = 0;
	//INT iTEmp = 0;
	//FILE *fpTemp = fopen("1.txt", "w");
	//for(i=0;i<pic_file[id].num;i++)
	//{
	//	fprintf(fpTemp, "%d ", i);
	//	for(int j=0;j<6;j++)
	//	{
	//		iTEmp = pic_file[id].idx[count];
	//		fprintf(fpTemp, "%d ", iTEmp);
	//		count++;
	//	}
	//	fprintf(fpTemp, "\n");
	//}
	//fclose(fpTemp);
    UTIL_CloseFile(fp);
  	INT iSize = pic_file[id].num*sizeof(struct CacheNode *);
	pic_file[id].pcache =(struct CacheNode **)malloc(iSize);
    if(pic_file[id].pcache ==NULL)
	{
		TerminateOnError("JY_PicLoadFile: ���ܷ����ڴ�  pcache memory\n");
		return 1;
    }

	for(i=0;i<pic_file[id].num;i++)
		pic_file[id].pcache[i]=NULL;

    return 0;
} 

// ���ز���ʾ��ͼ
// fileid        ��ͼ�ļ�id 
// picid     ��ͼ���
// x,y       ��ʾλ��
//  flag ��ͬbit����ͬ���壬ȱʡ��Ϊ0
//  B0    0 ����ƫ��xoff��yoff��=1 ������ƫ����
//  B1    0     , 1 �뱳��alpla �����ʾ, value Ϊalphaֵ(0-256), 0��ʾ͸��
//  B2            1 ȫ��
//  B3            1 ȫ��
//  value ����flag���壬Ϊalphaֵ�� 

INT JY_LoadPic(INT fileid, INT picid, INT x,INT y,INT flag,INT value)
{ 
    struct CacheNode *newcache,*tmpcache;
	INT find=0;
 
 	picid=picid/2;

	if(fileid<0 || fileid >=PIC_FILE_NUM || picid<0 || picid>=pic_file[fileid].num )    // ��������
		return 1;

	if(pic_file[fileid].pcache[picid]==NULL){   //��ǰ��ͼû�м���
		//����cache����
		newcache=(struct CacheNode *)malloc(sizeof(struct CacheNode));
		if(newcache==NULL)
		{
			TerminateOnError("JY_LoadPic: �����ڴ�ʧ��:newcache memory!\n");
			return 1;
		}

		newcache->s=LoadPic(fileid,picid,&newcache->xoff,&newcache->yoff);
        newcache->id =picid;
		newcache->fileid =fileid;
        pic_file[fileid].pcache[picid]=newcache;

        if(currentCacheNum<g_MAXCacheNum)
		{  //cacheû��
            list_add(&newcache->list ,&cache_head);    //���ص���ͷ
            currentCacheNum=currentCacheNum+1;
 		}
		else
		{   //cache ����
            tmpcache=list_entry(cache_head.prev, struct CacheNode , list);  //���һ��cache
            pic_file[tmpcache->fileid].pcache[tmpcache->id]=NULL;
			if(tmpcache->s!=NULL) 
				SafeFreeSdlSurface(tmpcache->s);       //ɾ������
			list_del(&tmpcache->list);
			SafeFree(tmpcache);
			
			list_add(&newcache->list ,&cache_head);    //���ص���ͷ
            CacheFailNum++;
            if(CacheFailNum % 100 ==1)
                TerminateOnError("JY_LoadPic: �����ڴ�ʧ��:Pic Cache is full!\n");
        }
    }
	else
	{   //�Ѽ�����ͼ
 		newcache=pic_file[fileid].pcache[picid];
		list_del(&newcache->list);    //�����cache������ժ��
		list_add(&newcache->list ,&cache_head);    //�ٲ��뵽��ͷ
	}

	if(flag & 0x00000001)
        BlitSurface(newcache->s , x,y,flag,value);
	else
        BlitSurface(newcache->s , x - newcache->xoff,y - newcache->yoff,flag,value);

    return 0;
}

// ������ͼ������
SDL_Surface *LoadPic(INT fileid,INT picid, INT *xoffset,INT *yoffset)
{ 	
	INT iFileLen = 0;
	INT iW = 0;
	INT iH = 0;
	INT iOffX = 0;
	INT iOffY = 0;
	INT iOffset = 0;

    SDL_Surface *surf=NULL;
	SDL_Surface *surfTemp=NULL;

    if(pic_file[fileid].idx ==NULL){
		TerminateOnError("LoadPic: fileid %d can not load?\n",fileid);
        return NULL;
    }

	iFileLen = pic_file[fileid].idx[picid * 6];
	iW = pic_file[fileid].idx[picid * 6+1] * g_iZoom;
	iH = pic_file[fileid].idx[picid * 6+2] * g_iZoom ;
	iOffX = pic_file[fileid].idx[picid * 6+3] * g_iZoom ;
	//iOffY = pic_file[fileid].idx[picid * 6+4] ;
	//����Yƫ��
	if (pic_file[fileid].idx[picid * 6+2] - pic_file[fileid].idx[picid * 6+4] > 0)
		iOffY = pic_file[fileid].idx[picid * 6+2] * g_iZoom - (pic_file[fileid].idx[picid * 6+2] - pic_file[fileid].idx[picid * 6+4]) * g_iZoom;
	else if (pic_file[fileid].idx[picid * 6+2] - pic_file[fileid].idx[picid * 6+4] < 0)
		iOffY = pic_file[fileid].idx[picid * 6+2] * g_iZoom + (pic_file[fileid].idx[picid * 6+4] - pic_file[fileid].idx[picid * 6+2]) * g_iZoom;
	else
		iOffY = iH;
	iOffset = pic_file[fileid].idx[picid * 6+5];
	
	*xoffset = iOffX;
	*yoffset = iOffY;
	LPBYTE pBuf = NULL;

	FILE *fp = NULL;

	if(iFileLen>0)
	{
		pBuf = (LPBYTE)malloc(iFileLen + 2);
		if (pBuf == NULL)
		{
			TerminateOnError("LoadPic:���ܷ����ڴ� mapPic Mem \n");
		}
		if ( fileid == 0 )
		{
			fp = gpGlobals->f.fpMmapGrp;
		}
		if ( fileid == 1 )
		{
			fp = gpGlobals->f.fpSmapGrp;
		}
		if ( fileid == 2 )
		{
			fp = gpGlobals->f.fpHdGrpGrp;
		}
		if ( fileid == 3 )
		{
			fp = gpGlobals->f.fpWmapGrp;
		}
		if ( fileid == 4 )
		{
			fp = gpGlobals->f.fpEftGrp;
		}
		if ( fileid == 5 )
		{
			if(gpGlobals->f.fpFightGrp == NULL)
			{
				SafeFree(pBuf);
				return NULL;
			}
			fp = gpGlobals->f.fpFightGrp;

		}
		if (JY_GRPReadChunk(pBuf,iFileLen,iOffset,fp) > -1)
		{
			surf = SDL_CreateRGBSurface(SDL_SWSURFACE, iW, iH,8, 0,0,0, 0);
			SDL_SetPalette(surf, SDL_LOGPAL|SDL_PHYSPAL, g_colors, 0, 256);
			INT iNum = 0;
			for(INT i=0;i<iH;i++)
			{
				for(INT j=0;j<iW;j++)
				{
					iNum = i/g_iZoom * iW/g_iZoom +j/g_iZoom;
					JY_PutPixel(surf,j,i,pBuf[iNum]);
					//iNum++;
				}
			}
			
			SDL_SetColorKey(surf,SDL_SRCCOLORKEY|SDL_RLEACCEL ,TRANSCOLOR);  //ʹ��RLE����  
		}
		SafeFree(pBuf);
    }
    fp == NULL;
    return surf;
}


//�õ���ͼ��С
INT JY_GetPicXY(INT fileid, INT picid, INT *w,INT *h)
{
    struct CacheNode *newcache;
	INT r=JY_LoadPic(fileid, picid, g_wInitialWidth+1,g_wInitialHeight+1,1,0);   //������ͼ����������λ��

    *w=0;
	*h=0;

	if(r!=0)
		return 1;

    newcache=pic_file[fileid].pcache[picid/2];

    if(newcache->s){      // ���У���ֱ����ʾ
        *w= newcache->s->w;
        *h= newcache->s->h;
	}

	return 0;
}

//����ԭ����Ϸ��RLE��ʽ��������
SDL_Surface* CreatePicSurface32(unsigned char *data, INT w,INT h,INT datalong)
{    
	INT x1=0,y1=0,p=0;    
	INT i,j;
	INT yoffset;
	INT row;
	INT start;
    INT x;
    INT solidnum;
	SDL_Surface *ps1,*ps2 ;
    Uint32 *data32=NULL;

    data32=(Uint32 *)malloc(w*h*4);
	if(data32==NULL){
		TerminateOnError("CreatePicSurface32: ���ܷ����ڴ�  data32 memory!");
		return NULL;
	}

	for(i=0;i<w*h;i++)
		data32[i]=TRANSCOLOR;

	for(i=0;i<h;i++){
        yoffset=i*w;       
        row=data[p];            // i�����ݸ���
		start=p;
		p++;
		if(row>0){
			x=0;                // i��Ŀǰ��
            while(1){
                x=x+data[p];    // i�пհ׵����������͸����
				if(x>=w)        // i�п�ȵ�ͷ������
					break;

				p++;
                solidnum=data[p];  // ��͸�������
                p++;
				for(j=0;j<solidnum;j++){
                    data32[yoffset+x]=data[p];//m_color32[data[p]];
					p++;
					x++;
				}
                if(x>=w)
					break;     // i�п�ȵ�ͷ������
				if(p-start>=row) 
					break;    // i��û�����ݣ�����
			}
            if(p+1>=datalong) 
				break;
		}
	}
 
    ps1=SDL_CreateRGBSurfaceFrom(data32,w,h,32,w*4,0xff0000,0xff00,0xff,0);  //����32λ����
	if(ps1==NULL){
		TerminateOnError("CreatePicSurface32: ���ܷ����ڴ�  SDL_Surface ps1!");
	}
    ps2 = ps1;
	//ps2=SDL_DisplayFormat(ps1);   // ��32λ�����Ϊ��ǰ����
	if(ps2==NULL){
		TerminateOnError("CreatePicSurface32: ���ܷ����ڴ�  SDL_Surface ps2!\n");
	}

	SafeFreeSdlSurface(ps1);      
	SafeFree(data32);   	
    SDL_SetColorKey(ps2,SDL_SRCCOLORKEY|SDL_RLEACCEL ,TRANSCOLOR);  //ʹ��RLE����

    return ps2;
   	
}
