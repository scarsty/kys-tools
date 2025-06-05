
#ifndef SCRIPT_H
#define SCRIPT_H

#ifdef __cplusplus
extern "C"
{
#endif
short JY_RunTriggerScript(short wScriptEntry,short aiWuPin);
VOID JY_ThingSet(short iWuPin,short iNum);//�����Ʒ
short JY_FindThing(short iWuPin);//������Ʒ
VOID SetSceneInStatus(short iNum);//���ó���״̬
short JY_CheckGroupNum(VOID);//����Ա����
VOID JY_GroupSet(short iPerson,BOOL bIn);//��Ա�롢���
VOID JY_ThingSort(VOID);//
VOID JY_Sleep(VOID);//˯��
short JY_FindPerson(short iPerson);//���Ҷ�����ĳ��
VOID JY_SetScenePic(short iScene,short iLayer,short x,short y,short Pic);//����ĳ����ĳ��(x,y)ͼ��
VOID JY_SetPersonStatus(short iType,short iPerson,short iNum,bool bSet);//����ĳ��ĳ����ֵ��bSet=true������ֵ=iNum��bSet=false��ԭ��ֵ�ϼ�
VOID JY_MoveScene(short xOld,short yOld,short xNew,short yNew,short iDrawHero);//�ƶ�����iDrawHero=1��ʾ��������
VOID JY_MoveSub(short direct);
short JY_UseThing(short iPersonIdx,short iWuPinIdx);//ĳ��ʹ����Ʒ
short JY_UseThingForUser(short iPerson,short iWuPin,short iWuPinNum);
short JY_UseThingForNpc(short iPerson,short iWuPinIdx);//
short JY_UserYLJD(short iUsePersonIdx,short iDestPersonIdx,short iType);//ĳ����ĳ��ҽ�ƽⶾ
short JY_AllPersonExit(VOID);
VOID JY_CheckBook(VOID);
short JY_WuDaoDaHui(VOID);
VOID JY_GaoChangMiGong(VOID);
extern BOOL       g_fScriptSuccess;
#ifdef __cplusplus
}
#endif

#endif