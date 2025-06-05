Attribute VB_Name = "Module2"
Option Explicit

 Declare Function CreateCompatibleDC Lib "gdi32" (ByVal hdc As Long) As Long
 Declare Function CreateDIBSection Lib "gdi32" (ByVal hdc As Long, pBitmapInfo _
                         As BITMAPINFO, ByVal un As Long, lplpVoid As Long, ByVal handle As Long, ByVal dw As Long) _
                         As Long
 Declare Function SelectObject Lib "gdi32" (ByVal hdc As Long, ByVal hObject As Long) _
                         As Long
 Declare Function DeleteDC Lib "gdi32" (ByVal hdc As Long) As Long
 Declare Function DeleteObject Lib "gdi32" (ByVal hObject As Long) As Long
 Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Destination As Any, Source As Any, ByVal Length As Long)
 Declare Function BitBlt Lib "gdi32" (ByVal hDestDC As Long, ByVal X As Long, ByVal Y _
As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As _
Long, ByVal ySrc As Long, ByVal dwRop As Long) As Long
 Declare Function GetDC Lib "user32" (ByVal hwnd As Long) As Long
 Declare Function GetDIBits Lib "gdi32" (ByVal aHDC As Long, ByVal hBitmap As Long, ByVal nStartScan As Long, ByVal nNumScans As Long, lpBits As Any, lpBI As BITMAPINFO, ByVal wUsage As Long) As Long
Declare Function GetCursorPos Lib "user32" (lpPoint As POINTAPI) As Long
Declare Function GetPixel Lib "gdi32" (ByVal hdc As Long, ByVal X As Long, ByVal Y As Long) As Long
Declare Function ReleaseDC Lib "user32" (ByVal hwnd As Long, ByVal hdc As Long) As Long




 Declare Function GetLastError Lib "kernel32" () As Long
Public Type BITMAP24INFOHEADER '4� bytes
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

Type RGBQUAD
        rgbBlue As Byte
        rgbGreen As Byte
        rgbRed As Byte
        rgbReserved As Byte
End Type

 Type BITMAPINFO
        bmiHeader As BITMAP24INFOHEADER

        bmiColors As RGBQUAD    ' RGB, so length here doesn't matter
End Type

' �������� ��ͼ�Զ�������
' ƫ��(x,y)����˼����ͼ����ʱ����ʾƫ���������⸱ͼ����ʾʱҪ��x��y�����ƫ�ƶ��٣�ע���Ǹ�ƫ�ƣ�
 Type RLEPic
    Width As Integer   ' ͼƬ���
    Height As Integer  ' ͼƬ�߶�
    X As Integer       ' ͼƬxƫ��
    Y As Integer       ' ͼƬyƫ��
    dataLong As Long   ' ͼƬRLEѹ�����ݳ���
    Data() As Byte     ' ͼƬRLEѹ������
    Data32() As Long   ' ͼƬ32λѹ������
End Type
Type RLEPic1
    Width As Integer   ' ͼƬ���
    Height As Integer  ' ͼƬ�߶�
    X As Integer       ' ͼƬxƫ��
    Y As Integer       ' ͼƬyƫ��
    Data() As Byte     ' ͼƬRLEѹ������
End Type
Public mColor_RGB(256) As Long
Public palR(0 To 255) As Byte
Public palG(0 To 255) As Byte
Public palB(0 To 255) As Byte
Public palOK As Boolean


' ��ȡ��ɫ������
' jinyong����ɫ���ǰ���256ɫ��ÿɫrgb��һ���ֽ�
Public Sub SetColor()
Dim Filenum As Integer
Dim I As Long, tempstr1 As String
   tempstr1 = "��ȡ��ɫ��ʧ�ܡ�"
   If ChkRequiredFile(GamePath & MMAP_COL, 768, tempstr1, tempstr1, tempstr1) < 2 Then Exit Sub
   
    Filenum = FreeFile()
    Open GamePath & MMAP_COL For Binary Access Read As Filenum
    For I = 0 To 255
        Get Filenum, , palR(I)
        Get Filenum, , palG(I)
        Get Filenum, , palB(I)
        palB(I) = palB(I) * 4           ' ��ɫֵ��Ҫ��4
        palG(I) = palG(I) * 4
        palR(I) = palR(I) * 4
        
        ' ת��Ϊ32λ��ɫֵ��32λ��ɫֵ���λΪ0�����ఴ��rgb˳������
        mColor_RGB(I) = palB(I) + palG(I) * 256& + palR(I) * 65536
    Next I

End Sub
