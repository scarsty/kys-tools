Attribute VB_Name = "Modmain"
Option Explicit

Public Const XSCALE = 18
Public Const YSCALE = 9

Public Const MASKCOLOR = &H707030

Public Type BITMAPINFOHEADER '4� bytes
        biSize As Long

        biWidth As Long
        biHeight As Long
        biPlanes As Integer
        biBitCount As Integer
        biCompression As Long
        biSizeImage As Long
        biXPelsPerMeter As Long
        biYPelsPerMeter As Long
        biClrUsed As Long
        biClrImportant As Long
End Type

Public Type RGBQUAD
        rgbBlue As Byte
        rgbGreen As Byte
        rgbRed As Byte
        rgbReserved As Byte
End Type

Public Type BITMAPINFO
        bmiHeader As BITMAPINFOHEADER

        bmiColors As RGBQUAD    ' RGB, so length here doesn't matter
End Type






Public Declare Function CreateCompatibleDC Lib "gdi32" (ByVal hdc As Long) As Long
Public Declare Function CreateDIBSection Lib "gdi32" (ByVal hdc As Long, pBitmapInfo _
As BITMAPINFO, ByVal un As Long, lplpVoid As Long, ByVal handle As Long, ByVal dw As Long) _
As Long

Public Declare Function SelectObject Lib "gdi32" (ByVal hdc As Long, ByVal hObject As Long) _
As Long
Public Declare Function DeleteDC Lib "gdi32" (ByVal hdc As Long) As Long
Public Declare Function DeleteObject Lib "gdi32" (ByVal hObject As Long) As Long


Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal length As Long)
Public Declare Sub MoveMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal length As Long)


Public Declare Function BitBlt Lib "gdi32" (ByVal hDestDC As Long, ByVal x As Long, ByVal Y _
As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As _
Long, ByVal ySrc As Long, ByVal dwRop As Long) As Long


Public Declare Function GetLastError Lib "kernel32" () As Long



Public Declare Function GetPrivateProfileInt Lib "kernel32" Alias "GetPrivateProfileIntA" (ByVal lpApplicationName As String, ByVal lpKeyName As String, ByVal nDefault As Long, ByVal lpFileName As String) As Long

'Public Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpDefault As String, ByVal lpReturnedString As String, ByVal nSize As Long, ByVal lpFileName As String) As Long
Public Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpDefault As String, addr As Byte, ByVal nSize As Long, ByVal lpFileName As String) As Long
Public Declare Function WritePrivateProfileString Lib "kernel32" Alias "WritePrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpString As Any, ByVal lpFileName As String) As Long




Public Type WarDataType   ' ս����������   ��Щ�����޷����봰�壬��֧�� public type
    id As Integer
    namebig5(9) As Byte
    Name As String
    mapid As Integer
    Experience As Integer
    musicid As Integer
    Warperson(5) As Integer
    SelectWarperson(5) As Integer
    personX(5) As Integer
    personY(5) As Integer
    Enemy(19) As Integer
    EnemyX(19) As Integer
    EnemyY(19) As Integer
End Type

Public WarData() As WarDataType
Public Warnum As Long             ' ս������


Public Type statementAttribType  ' ָ������
    length As Long               ' ָ���
    isGoto As Long               ' ָ���Ƿ�Ϊ����ת�ƣ�0���� 1��
    yesOffset As Long            ' ��������ת�Ƶ�ַ��ָ���еڼ�����
    noOffset As Long             ' ����������ת�Ƶ�ַ��ָ���еڼ�����
    notes As String              ' ָ��˵��
End Type

Public StatAttrib() As statementAttribType



Public kdefword() As Integer   ' ����kdef�¼�����������
Public kdeflong As Long        ' �¼��ļ�����

Public KDEFIDX() As Long       ' �¼�kdef����
Public numkdef As Long         ' �����¼�����

Public Type KdefType           ' �����¼�����
    DataLong  As Long          ' �¼����������ݳ���
    Data() As Integer          ' �¼�����������
    kdef As Collection         ' �¼�ָ���
    numlabel As Long           ' �¼��б����Ŀ
End Type

Public KdefInfo() As KdefType

Public KdefName As Collection       ' ָ�������ּ���


Public ClipboardStatement As Collection  ' �����µ�ָ������

Public ClipboardKdef As Collection  ' �����µ�ָ������



Public Talk() As String        ' �Ի��ַ���
Public TalkIdx() As Long           ' �Ի�����
Public numtalk As Long      ' �Ի�����

Public Type PersonAttrib       ' ��������
    r1 As Integer
    PhotoId As Integer
    r3 As Integer
    r4 As Integer
    name1big5(9) As Byte
    Name1 As String
    name2big5(9) As Byte
    name2 As String
    
End Type

Public Person() As PersonAttrib  ' ������������
Public PersonNum As Long         ' �������

Public Type ThingsAttrib         ' ��Ʒ����
    r1 As Integer
    name1big5(19) As Byte
    Name1 As String
    name2big5(19) As Byte
    name2 As String
End Type

Public Things() As ThingsAttrib  ' ��Ʒ��������
Public Thingsnum As Long         ' ��Ʒ����



Public Type SceneType
    SceneID As Integer                ' ��������
    Name1(9) As Byte                  ' ����
    OutMusic As Integer               ' ��������
    InMusic  As Integer               ' ��������
    JumpScene As Integer              ' ��ת����
    InCondition As Integer            ' ��������
    MMapInX1 As Integer               ' ����ͼ�������
    MMapInY1 As Integer
    MMapInX2 As Integer
    MMapInY2 As Integer
    InX As Integer                    ' ������ʼ����
    InY As Integer
    OutX(2) As Integer                ' ������������
    OutY(2) As Integer
    JumpX1 As Integer               ' ������ת������
    JumpY1 As Integer
    JumpX2 As Integer               ' ������ת������
    JumpY2 As Integer
End Type

Public Scene() As SceneType  ' ������������
Public Scenenum As Long          ' ��������

Public Type WuGongAttrib           ' �书����
    r1 As Integer
    name1big5(19) As Byte
    Name1 As String
End Type


Public WuGong() As WuGongAttrib    ' �书��������
Public WuGongnum As Long           ' �书����


Public Type RLEPic
    isEmpty As Boolean
    Width As Integer   ' ͼƬ���
    height As Integer  ' ͼƬ�߶�
    x As Integer       ' ͼƬxƫ��
    Y As Integer       ' ͼƬyƫ��
    DataLong As Long   ' ͼƬRLEѹ�����ݳ���
    Data() As Byte     ' ͼƬRLEѹ������
    Data32() As Long   ' ͼƬ32λѹ������
End Type

Public HeadPic() As RLEPic  ' ����ͷ������
Public Headnum As Long      ' ����ͷ�����

Public WarPic() As RLEPic  ' ս��ͼƬ����
Public Warpicnum As Long   ' ս������

Public g_PP As RLEPic     ' �༭��ͼ�ã����ݲ�����


' D* �¼���Ϣ

Public Type D_Event_type
    isGo As Integer
    id As Integer
    EventNum1 As Integer
    EventNum2 As Integer
    EventNum3 As Integer
    picnum(2) As Integer
    PicDelay As Integer
    x As Integer
    Y As Integer
End Type

Public g_DD As D_Event_type          ' �޸ĳ����¼����崰��ʹ�ã��������ݲ�����



Public HeadtoPerson() As Collection   ' ����ͷ��id������id

Public mcolor_RGB(256) As Long  ' ��ɫ��


Type G_VarType
    JYPath As String
    IniFilename As String
    Palette As String
    MMAPIDX As String
    MMAPGRP As String
    MMAPStruct(5) As String
    SMAPIDX As String
    SMAPGRP As String
    WarMapIDX As String
    WarMapGrp As String
    WarDefine As String
    WarMapDefIDX As String
    WarMapDefGRP As String
    TalkIdx As String
    TalkGRP As String
    RIDX(3) As String
    RGRP(3) As String
    DIDX(3) As String
    DGRP(3) As String
    SIDX(3) As String
    SGRP(3) As String
    RecordNote(3) As String
    EXE As String
    KDEFIDX As String
    KDEFGRP As String
    HeadIDX As String
    HeadGRP As String
End Type


Public G_Var As G_VarType

Public Charset As String

'Public IniFilename As String


Public Sub Main()
Dim tmpstrArray()  As String
Dim i As Long
On Error GoTo Label1
    Charset = "GBK"
    
    Call LoadMB
    
    G_Var.IniFilename = App.Path & "\fishedit.ini"
    
    'ConvertBig5INI
        
    G_Var.JYPath = ""
    
    frmSelectCharset.Show vbModal
    
    'If Charset = "GBK" Then
        G_Var.IniFilename = App.Path & "\fishedit.ini"
    'Else
      '  G_Var.IniFilename = App.Path & "\fishedit.big5.ini"
    'End If
    
    G_Var.Palette = GetINIStr("File", "Palette")
    
    G_Var.MMAPIDX = GetINIStr("File", "MMAPIDX")
    G_Var.MMAPGRP = GetINIStr("File", "MMAPGRP")
    tmpstrArray = Split(GetINIStr("File", "MMAPStruct"), ",")
    For i = 0 To 4
        G_Var.MMAPStruct(i) = tmpstrArray(i)
    Next i
    
    G_Var.SMAPIDX = GetINIStr("File", "SMAPIDX")
    G_Var.SMAPGRP = GetINIStr("File", "SMAPGRP")
    
    G_Var.WarMapIDX = GetINIStr("File", "WarMAPIDX")
    G_Var.WarMapGrp = GetINIStr("File", "WarMAPGRP")
    G_Var.WarDefine = GetINIStr("File", "WarDefine")
    G_Var.WarMapDefIDX = GetINIStr("File", "WarMAPDefIDX")
    G_Var.WarMapDefGRP = GetINIStr("File", "WarMAPDefGRP")
    
    
    G_Var.TalkIdx = GetINIStr("File", "TalkIDX")
    G_Var.TalkGRP = GetINIStr("File", "TalkGRP")
    
    G_Var.KDEFIDX = GetINIStr("File", "kdefIDX")
    G_Var.KDEFGRP = GetINIStr("File", "kdefGRP")
    
    G_Var.HeadIDX = GetINIStr("File", "HeadIDX")
    G_Var.HeadGRP = GetINIStr("File", "HeadGRP")
    
    tmpstrArray = Split(GetINIStr("File", "RIDX"), ",")
    For i = 0 To 3
        G_Var.RIDX(i) = tmpstrArray(i)
    Next i
    tmpstrArray = Split(GetINIStr("File", "RGRP"), ",")
    For i = 0 To 3
        G_Var.RGRP(i) = tmpstrArray(i)
    Next i
    tmpstrArray = Split(GetINIStr("File", "DIDX"), ",")
    For i = 0 To 3
        G_Var.DIDX(i) = tmpstrArray(i)
    Next i
    tmpstrArray = Split(GetINIStr("File", "DGRP"), ",")
    For i = 0 To 3
        G_Var.DGRP(i) = tmpstrArray(i)
    Next i
    tmpstrArray = Split(GetINIStr("File", "SIDX"), ",")
    For i = 0 To 3
        G_Var.SIDX(i) = tmpstrArray(i)
    Next i
    tmpstrArray = Split(GetINIStr("File", "SGRP"), ",")
    For i = 0 To 3
        G_Var.SGRP(i) = tmpstrArray(i)
    Next i
    
    tmpstrArray = Split(GetINIStr("File", "RecordNote"), ",")
    For i = 0 To 3
        G_Var.RecordNote(i) = tmpstrArray(i)
    Next i
    
    
    G_Var.EXE = GetINIStr("File", "EXEFilename")
    
    
    MDIMain.Show
 
Exit Sub

Label1:
    MsgBox Err.Description
    If (MDIMain Is Nothing) = False Then
        Unload MDIMain
    End If


End Sub




' ��ȡr1�ļ�
Public Sub ReadRR()
Dim idnum As Long
Dim filenum As Long
Dim idxR() As Long
Dim i As Long, j As Long
Dim offset As Long
'Dim length As Long
'Dim result As Long
    filenum = OpenBin(G_Var.JYPath & G_Var.RIDX(1), "R")
    idnum = LOF(filenum) / 4
    ReDim idxR(idnum)
    idxR(0) = 0
    For i = 1 To idnum
       Get filenum, , idxR(i)
    Next i
    Close (filenum)
    
    PersonNum = (idxR(2) - idxR(1)) / &HB6
    
    ReDim Person(PersonNum - 1)
    filenum = OpenBin(G_Var.JYPath & G_Var.RGRP(1), "R")
    offset = idxR(1)
    For i = 0 To PersonNum - 1
        Get filenum, offset + i * &HB6 + 1, Person(i).r1
        Get filenum, , Person(i).PhotoId
        Get filenum, , Person(i).r3
        Get filenum, , Person(i).r4
        Get filenum, , Person(i).name1big5
        Get filenum, , Person(i).name2big5
        Person(i).Name1 = Big5toUnicode(Person(i).name1big5, 10)
        Person(i).name2 = Big5toUnicode(Person(i).name2big5, 10)
        
    Next i
 
    Thingsnum = (idxR(3) - idxR(2)) / &HBE
    ReDim Things(Thingsnum - 1)
    offset = idxR(2)
    For i = 0 To Thingsnum - 1
        Get filenum, offset + i * &HBE + 1, Things(i).r1
        Get filenum, , Things(i).name1big5
        Get filenum, , Things(i).name2big5
        Things(i).Name1 = Big5toUnicode(Things(i).name1big5, 20)
        Things(i).name2 = Big5toUnicode(Things(i).name2big5, 20)
        
    Next i
 
    Scenenum = (idxR(4) - idxR(3)) / &H34
    ReDim Scene(Scenenum - 1)
    offset = idxR(3)
       
    Get filenum, offset + 1, Scene
    
    WuGongnum = (idxR(5) - idxR(4)) / &H88
    ReDim WuGong(WuGongnum - 1)
    offset = idxR(4)
    For i = 0 To WuGongnum - 1
        Get filenum, offset + i * &H88 + 1, WuGong(i).r1
        Get filenum, , WuGong(i).name1big5
        WuGong(i).Name1 = Big5toUnicode(WuGong(i).name1big5, 20)
        
    Next i
 
    Close (filenum)
    
End Sub

' ��ȡ������Ƭ��ת��Ϊ32λrle

Public Sub LoadPicFile(fileid As String, filepic As String, picdata() As RLEPic, picdatanum As Long)

Dim filenum As Integer, filenum2 As Integer
Dim i As Long
Dim value As Integer
Dim rr As Integer, gg As Integer, bb As Integer
Dim offset As Long
Dim picLong As Long
Dim num As Long
Dim xx As Long, yy As Long

Dim picNum2
Dim HeadIDX() As Long
   
    filenum = OpenBin(fileid, "R")
    picdatanum = LOF(filenum) / 4
    ReDim HeadIDX(picdatanum)
    ReDim picdata(picdatanum)
    HeadIDX(0) = 0
    For num = 1 To picdatanum ' ��ͼ��ͼ����������
        Get filenum, , HeadIDX(num)
    Next num
    Close filenum
        
    
    filenum = OpenBin(filepic, "R")
    For num = 0 To picdatanum - 1 ' ��ͼ��ͼ����������
        If HeadIDX(num) < 0 Then
            picLong = 0
        Else
            offset = HeadIDX(num)
            picLong = HeadIDX(num + 1) - HeadIDX(num)
            If (num = picdatanum - 1) And (HeadIDX(num + 1) <> LOF(filenum)) And HeadIDX(num) > 0 Then ' ���һ��idxӦ�õ����ļ�����
                picLong = LOF(filenum) - HeadIDX(num)
            End If
        End If
        If picLong > 0 Then
            picdata(num).isEmpty = False
            Get filenum, offset + 1, picdata(num).Width
            Get filenum, , picdata(num).height
            Get filenum, , picdata(num).x
            Get filenum, , picdata(num).Y
            picdata(num).DataLong = picLong - 8
            ReDim picdata(num).Data(picdata(num).DataLong - 1)
            Get filenum, , picdata(num).Data
            Call RLEto32(picdata(num))
        Else
            picdata(num).isEmpty = True
        End If
    Next num
    Close filenum
    

    
End Sub




' ����ͼ���ݵ�8BitRLEѹ�����ݣ�ת��Ϊ32Bit�������Ժ���
' RLEѹ����ʽ��
' ��һ���ֽ�Ϊ��һ�����ݳ��ȣ������ֽڣ�
' ����һ���ֽ�Ϊ͸�����ݵ�������������Ϊ��͸�����ݵ������Ȼ���ǲ�͸����ÿ�����ݵ�8λ��ɫ��
' �ظ����ϣ�ֱ����һ���ֽڽ���
' ��ȡ��һ�����ݣ�ֱ��û�к�������
Public Sub RLEto32(pic As RLEPic)
Dim p As Long  ' ָ��RLE���ݵ�ָ��
Dim p32 As Long   ' ָ��32λ��ѹ�����ݵ�ָ��
Dim i As Long, j As Long
Dim row As Byte
Dim temp As Long
Dim start As Long
Dim maskNum As Long
Dim solidNum As Long
   
    ReDim pic.Data32(pic.DataLong)
   
    p = 0
    p32 = 0
    For i = 1 To pic.height
        row = pic.Data(p)     ' ��ǰ�����ݸ���
        pic.Data32(p) = row
        start = p             ' ��ǰ����ʼλ��
        p = p + 1
        If row > 0 Then
            p32 = 0
            Do
                maskNum = pic.Data(p)  ' �������
                pic.Data32(p) = row
                p = p + 1
      
                p32 = p32 + maskNum
                If p32 >= pic.Width Then  ' ��������ɺ�λ��ָ���Ѿ�ָ�����Ҷ�
                    Exit Do
                End If
                solidNum = pic.Data(p) ' ʵ�ʵ�ĸ���
                pic.Data32(p) = solidNum
                p = p + 1
                For j = 1 To solidNum
                    temp = pic.Data(p)
                    pic.Data32(p) = mcolor_RGB(temp)
                    p32 = p32 + 1
                    p = p + 1
                Next j
                If p32 >= pic.Width Then   ' ʵ�ʵ���ɺ�λ��ָ���Ѿ�ָ�����Ҷ�
                    Exit Do
                End If
                If p - start >= row Then           ' ��ǰ�������Ѿ�����
                    Exit Do
                End If
            Loop
            If p + 1 >= pic.DataLong Then
                Exit For
            End If
        End If
    Next i
   
End Sub



' ��ȡ��ɫ������
' jinyong����ɫ���ǰ���256ɫ��ÿɫrgb��һ���ֽ�
Public Sub SetColor()
Dim filenum As Integer
Dim i As Long
Dim rr As Byte, gg As Byte, bb As Byte
    
    'filenum = FreeFile()
    filenum = OpenBin(G_Var.JYPath & G_Var.Palette, "R")
    For i = 0 To 255
        Get filenum, , rr
        Get filenum, , gg
        Get filenum, , bb
        rr = rr * 4           ' ��ɫֵ��Ҫ��4
        gg = gg * 4
        bb = bb * 4
        ' ת��Ϊ32λ��ɫֵ��32λ��ɫֵ���λΪ0�����ఴ��rgb˳������
        mcolor_RGB(i) = bb + gg * 256& + rr * 65536
    Next i

End Sub


' ����ͼ�����ݵ�addrָ��ĵ�ַ
' picnum ��ͼ���
' width height addrָ���dib���
' x1,y1,��ͼλ��
Public Sub genPicData(pic As RLEPic, addr As Long, ByVal Width As Long, ByVal height As Long, ByVal x1 As Long, ByVal Y1 As Long)
Dim i As Long, j As Long
Dim x As Long, Y As Long
Dim row As Byte
Dim start As Long
Dim p As Long
Dim maskNum As Byte
Dim solidNum As Byte
Dim yoffset As Long
Dim xoffset As Long
Dim offset As Long
    
   'x1 = x1 - pic.x
   'y1 = y1 - pic.y
    
    If x1 >= 0 And Y1 >= 0 And x1 + pic.Width <= Width And Y1 + pic.height <= height Then
        p = 0
        For i = 1 To pic.height
            Y = i
            yoffset = (Y + Y1 - 1) * Width
            
            row = pic.Data(p)
            start = p
            p = p + 1
            If row > 0 Then
                x = 0
                Do
                    x = x + pic.Data(p)
                    If x >= pic.Width Then
                        Exit Do
                    End If
                    p = p + 1
                    solidNum = pic.Data(p)
                    p = p + 1
                    xoffset = x + (x1)
                    offset = xoffset + yoffset
                    Call CopyMemory(ByVal (addr + offset * 4), pic.Data32(p), 4 * solidNum)
                    x = x + solidNum
                    p = p + solidNum
                    If x >= pic.Width Then
                        Exit Do
                    End If
                    If p - start >= row Then
                        Exit Do
                    End If
                Loop
                If p + 1 >= pic.DataLong Then
                    Exit For
                End If
            End If
        Next i
    Else
        p = 0
        For i = 1 To pic.height
            Y = i
            yoffset = (Y + Y1 - 1) * Width
            
            row = pic.Data(p)
            start = p
            p = p + 1
            If row > 0 Then
                x = 0
                Do
                    x = x + pic.Data(p)
                    If x >= pic.Width Then
                        Exit Do
                    End If
                    p = p + 1
                    solidNum = pic.Data(p)
                    p = p + 1
                    xoffset = x + (x1)
                    
                    If Y1 + Y - 1 >= 0 And Y1 + Y < height And xoffset + solidNum >= 0 And xoffset < Width Then
                        Dim p2 As Long
                        Dim ee As Long
                        
                        If xoffset < 0 Then
                            offset = yoffset
                            p2 = p - xoffset
                            ee = solidNum + xoffset
                        Else
                            offset = xoffset + yoffset
                            p2 = p
                            ee = solidNum
                        End If
                        If xoffset + solidNum >= Width Then
                            ee = ee - (xoffset + solidNum - Width + 1)
                        End If
                        Call CopyMemory(ByVal (addr + offset * 4), pic.Data32(p2), 4 * ee)
                    End If
                    x = x + solidNum
                    p = p + solidNum
                    If x >= pic.Width Then
                        Exit Do
                    End If
                    If p - start >= row Then
                        Exit Do
                    End If
                Loop
                If p + 1 >= pic.DataLong Then
                    Exit For
                End If
            End If
        Next i
    End If
            
End Sub


Public Sub ShowPicDIB(pic As RLEPic, hdc As Long, ByVal xoffset As Long, ByVal yoffset As Long)
 
Dim addr As Long
Dim temp As Long
Dim dib As New clsDIB
    If pic.isEmpty = True Then Exit Sub

    
    dib.CreateDIB pic.Width, pic.height
    
    
    
  
    ' �ڵ�ǰ����λ����ͼ
    Call genPicData(pic, dib.addr, pic.Width, pic.height, 0, 0)
    
   temp = BitBlt(hdc, xoffset - pic.x, yoffset - pic.Y, pic.Width, pic.height, dib.CompDC, 0, 0, &HCC0020)


End Sub

Public Sub ConvertBig5INI()
Dim filenum As Long
Dim filename As String
Dim tmpdata() As Byte
Dim tmpoutput() As Byte
Dim p As Long
Dim b0 As Byte, b1 As Byte
Dim c0 As Byte, c1 As Byte
    filename = G_Var.IniFilename
    filenum = OpenBin(filename, "R")
    ReDim tmpdata(LOF(filenum) - 1)
    ReDim tmpoutput(LOF(filenum) - 1)
    Get #filenum, , tmpdata

    p = 0
    While (p < LOF(filenum))
        b0 = tmpdata(p)
        If b0 < 128 Then
            tmpoutput(p) = b0
            p = p + 1
        Else
           b1 = tmpdata(p + 1)
           c0 = gbk_big5(b0, b1, 0)
           c1 = gbk_big5(b0, b1, 1)
           If c0 > 0 Or c1 > 0 Then
               tmpoutput(p) = c0
               tmpoutput(p + 1) = c1
           Else
               tmpoutput(p) = AscB("?")
               tmpoutput(p + 1) = AscB("?")
           End If
           p = p + 2
        End If
        
    Wend
    Close filenum
    filename = App.Path & "\fishedit.big5.ini"

    filenum = OpenBin(filename, "WN")
    Put #filenum, , tmpoutput
    Close filenum

End Sub






Public Sub LoadSMap(id As Long, picdata() As RLEPic, picnum As Long)
    Call LoadPicFile(G_Var.JYPath & G_Var.SMAPIDX, G_Var.JYPath & G_Var.SMAPGRP, picdata, picnum)

End Sub




' ��kdef�ļ�
Public Sub ReadKdef()
Dim filenum As Long
Dim i  As Long
    
    filenum = OpenBin(G_Var.JYPath & G_Var.KDEFIDX, "R")
        numkdef = LOF(filenum) / 4
        ReDim KDEFIDX(numkdef)
        KDEFIDX(0) = 0
        For i = 1 To numkdef
            Get filenum, , KDEFIDX(i)
            KDEFIDX(i) = KDEFIDX(i) / 2
        Next i
    Close (filenum)
    
    ReDim KdefInfo(numkdef - 1)
    filenum = OpenBin(G_Var.JYPath & G_Var.KDEFGRP, "R")
        For i = 0 To numkdef - 1
            KdefInfo(i).numlabel = 0
            KdefInfo(i).DataLong = (KDEFIDX(i + 1) - KDEFIDX(i))
            ReDim KdefInfo(i).Data(KdefInfo(i).DataLong - 1)
            Get filenum, KDEFIDX(i) * 2 + 1, KdefInfo(i).Data
        Next i
    Close
    
    
End Sub

' ��kdef�ļ�
Public Sub savekdef(filename As String)
Dim filenum As Long
Dim filenum2 As Long

Dim i  As Long, j As Long

Dim length As Long
Dim offset As Long
    
    filenum = OpenBin(G_Var.JYPath & G_Var.KDEFIDX, "WN")
    
    filenum2 = OpenBin(G_Var.JYPath & filename, "WN")
    KDEFIDX(0) = 0
    For i = 0 To numkdef - 1
        length = KdefInfo(i).DataLong
        KDEFIDX(i + 1) = KDEFIDX(i) + length
        For j = 0 To length - 1
            Put #filenum2, , KdefInfo(i).Data(j)
        Next j
        Put #filenum, , CLng(KDEFIDX(i + 1) * 2)
    Next i
    Close (filenum)
    Close (filenum2)


End Sub



