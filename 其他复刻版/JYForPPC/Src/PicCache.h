#ifndef _PICCACHE_H
#define _PICCACHE_H

#ifdef __cplusplus
extern "C"
{
#endif

// ����ʹ�õ����� 
struct CacheNode{    //��ͼcache����ڵ�
	SDL_Surface *s;               // ����ͼ��Ӧ�ı���
	INT xoff;                     // ��ͼƫ��
	INT yoff;
	INT id;                  //��ͼ���
    INT fileid;              //��ͼ�ļ����
    struct list_head list;        // ����ṹ��linux.h�е�list.h�ж���
} ;
struct PicFileCache{   //��ͼ�ļ�����ڵ�
	INT num;                    // �ļ���ͼ����
    INT *idx;                  // idx������
    struct CacheNode **pcache;  // �ļ������е���ͼ��Ӧ��cache�ڵ�ָ�룬Ϊ�����ʾû�С�
};

#define PIC_FILE_NUM 10   //�������ͼ�ļ�(idx/grp)����
INT Init_Cache(VOID);
INT JY_PicInit(VOID);
INT JY_PicLoadFile(const char*filename, INT id);
INT JY_LoadPic(INT fileid, INT picid, INT x,INT y,INT flag,INT value);
SDL_Surface *LoadPic(INT fileid,INT picid, INT *xoffset,INT *yoffset);
INT JY_GetPicXY(INT fileid, INT picid, INT *w,INT *h);
SDL_Surface* CreatePicSurface32(unsigned char *data, INT w,INT h,INT datalong);

#ifdef __cplusplus
}
#endif

#endif