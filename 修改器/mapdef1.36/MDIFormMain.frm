VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.MDIForm MDIFormMain 
   BackColor       =   &H8000000C&
   Caption         =   "��ӹȺ�b���๦�ܾ�݋��(δ�ҵ��[��)"
   ClientHeight    =   6870
   ClientLeft      =   165
   ClientTop       =   450
   ClientWidth     =   8865
   Icon            =   "MDIFormMain.frx":0000
   LinkTopic       =   "MDIForm1"
   StartUpPosition =   2  '��Ļ����
   WindowState     =   2  'Maximized
   Begin MSComctlLib.ProgressBar ProgressBar1 
      Align           =   2  'Align Bottom
      Height          =   255
      Left            =   0
      TabIndex        =   1
      Top             =   6315
      Width           =   8865
      _ExtentX        =   15637
      _ExtentY        =   450
      _Version        =   393216
      Appearance      =   1
   End
   Begin MSComctlLib.StatusBar StatusBar1 
      Align           =   2  'Align Bottom
      Height          =   300
      Left            =   0
      TabIndex        =   0
      Top             =   6570
      Width           =   8865
      _ExtentX        =   15637
      _ExtentY        =   529
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   7
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Object.Width           =   9349
            MinWidth        =   9349
         EndProperty
         BeginProperty Panel2 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Object.Width           =   2117
            MinWidth        =   2117
         EndProperty
         BeginProperty Panel3 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Object.Width           =   1764
            MinWidth        =   1764
         EndProperty
         BeginProperty Panel4 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Object.Width           =   1411
            MinWidth        =   1411
         EndProperty
         BeginProperty Panel5 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Object.Width           =   1764
            MinWidth        =   1764
         EndProperty
         BeginProperty Panel6 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Object.Width           =   1411
            MinWidth        =   1411
         EndProperty
         BeginProperty Panel7 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Object.Width           =   2822
            MinWidth        =   2822
         EndProperty
      EndProperty
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Times New Roman"
         Size            =   9
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
   End
   Begin VB.Menu mnuFuncView 
      Caption         =   "����/�O��(&O)"
      Begin VB.Menu nmuSetGamePath 
         Caption         =   "�O���[���bλ��"
      End
      Begin VB.Menu mnuSavManager 
         Caption         =   "�M�ȹ���"
         Enabled         =   0   'False
      End
      Begin VB.Menu mnuSetGameLanguage 
         Caption         =   "�[�򺆷��D�Q"
         Enabled         =   0   'False
      End
      Begin VB.Menu mnuExit 
         Caption         =   "�˳�(&X)"
      End
   End
   Begin VB.Menu mnuSavEdit 
      Caption         =   "�[��݋(&E)"
      Begin VB.Menu mnuEditRx 
         Caption         =   "�[������M��"
      End
      Begin VB.Menu mnuSavEditLine1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuEditSx 
         Caption         =   "�����؈D/�¼�"
      End
      Begin VB.Menu mnuEvent1 
         Caption         =   "�¼�/�Ի�"
         Enabled         =   0   'False
      End
      Begin VB.Menu mnu 
         Caption         =   "ս����ͼ"
         Enabled         =   0   'False
      End
      Begin VB.Menu mnuEarth 
         Caption         =   "�⾰��ͼ"
         Enabled         =   0   'False
      End
   End
   Begin VB.Menu mnuPicEdit 
      Caption         =   "ͼ��/��ͼ(&P)"
      Begin VB.Menu mnuTileEdit 
         Caption         =   "�N�D�YԴ"
      End
      Begin VB.Menu mnuPicEditLine1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuPicEdit31 
         Caption         =   "Ƭβȫ���Ӯ�"
      End
      Begin VB.Menu mnuPicEditLine3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCol 
         Caption         =   "��ɫ��"
         Enabled         =   0   'False
      End
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "����(&H)"
      Begin VB.Menu mnuReadme 
         Caption         =   "����(&C)"
      End
      Begin VB.Menu mnuAbout 
         Caption         =   "�P�(&A)"
      End
   End
   Begin VB.Menu mnu1 
      Caption         =   "�˵�1"
      Visible         =   0   'False
      Begin VB.Menu mnu1MapEdit1 
         Caption         =   "ˢ�D��"
         Index           =   0
      End
      Begin VB.Menu mnu1MapEdit1 
         Caption         =   "С�؈D"
         Checked         =   -1  'True
         Index           =   1
      End
      Begin VB.Menu mnu1MapEdit1 
         Caption         =   "�}�Ƶ����N��"
         Index           =   2
      End
      Begin VB.Menu mnu1MapEdit1 
         Caption         =   "Windows���D"
         Index           =   3
      End
      Begin VB.Menu mnu1MapEdit1 
         Caption         =   "���S��݋"
         Checked         =   -1  'True
         Index           =   4
      End
      Begin VB.Menu mnu1MapEdit1 
         Caption         =   "�@ʾ�¼���ӛ"
         Checked         =   -1  'True
         Index           =   5
      End
      Begin VB.Menu mnu1MapEdit1 
         Caption         =   "�N�D�x����"
         Enabled         =   0   'False
         Index           =   6
      End
      Begin VB.Menu mnu1_layer1 
         Caption         =   "�D��"
         Begin VB.Menu mnu1_Layer 
            Caption         =   "0��(�ر�)"
            Checked         =   -1  'True
            Index           =   0
         End
         Begin VB.Menu mnu1_Layer 
            Caption         =   "1��(���B/ֲ��)"
            Checked         =   -1  'True
            Index           =   1
         End
         Begin VB.Menu mnu1_Layer 
            Caption         =   "2��(����)"
            Checked         =   -1  'True
            Index           =   2
         End
         Begin VB.Menu mnu1_Layer 
            Caption         =   "3��(�¼�)"
            Checked         =   -1  'True
            Index           =   3
         End
         Begin VB.Menu mnu1_Layer 
            Caption         =   "4��(Yƫ�ƌ���1/3��)"
            Enabled         =   0   'False
            Index           =   4
         End
         Begin VB.Menu mnu1_Layer 
            Caption         =   "5��(Yƫ�ƌ���2��)"
            Enabled         =   0   'False
            Index           =   5
         End
      End
      Begin VB.Menu mnu1_Grid1 
         Caption         =   "�W��"
         Begin VB.Menu mnu1_Grid 
            Caption         =   "���@ʾ"
            Checked         =   -1  'True
            Index           =   0
         End
         Begin VB.Menu mnu1_Grid 
            Caption         =   "�W���ɫ"
            Enabled         =   0   'False
            Index           =   1
         End
         Begin VB.Menu mnu1_Grid 
            Caption         =   "4X"
            Index           =   2
         End
         Begin VB.Menu mnu1_Grid 
            Caption         =   "8X"
            Index           =   3
         End
         Begin VB.Menu mnu1_Grid 
            Caption         =   "16X"
            Index           =   4
         End
         Begin VB.Menu mnu1_Grid 
            Caption         =   "32X"
            Index           =   5
         End
         Begin VB.Menu mnu1_Grid 
            Caption         =   "64X"
            Index           =   6
         End
         Begin VB.Menu mnu1_Grid 
            Caption         =   "128X"
            Index           =   7
         End
      End
      Begin VB.Menu mnu1_Line1 
         Caption         =   "-"
      End
      Begin VB.Menu mnu1_Jindu1 
         Caption         =   "ȡ�M��"
         Begin VB.Menu mnu1_Jindu 
            Caption         =   "�M��һ"
            Checked         =   -1  'True
            Index           =   0
         End
         Begin VB.Menu mnu1_Jindu 
            Caption         =   "�M�ȶ�"
            Index           =   1
         End
         Begin VB.Menu mnu1_Jindu 
            Caption         =   "�M����"
            Index           =   2
         End
      End
      Begin VB.Menu mnu1_Line2 
         Caption         =   "-"
      End
      Begin VB.Menu mnu1MapEdit2 
         Caption         =   "���殔ǰ����"
         Index           =   0
      End
      Begin VB.Menu mnu1MapEdit2 
         Caption         =   "�P�]�؈D��݋��"
         Index           =   1
      End
   End
   Begin VB.Menu mnu2 
      Caption         =   "�˵�2"
      Visible         =   0   'False
      Begin VB.Menu mnu2_RxEdit1 
         Caption         =   "ˢ����ʾ"
         Index           =   0
      End
      Begin VB.Menu mnu2_RxEdit1 
         Caption         =   "������ʾʮ������"
         Enabled         =   0   'False
         Index           =   1
      End
      Begin VB.Menu mnu2_RxEdit1 
         Caption         =   "Windows������"
         Index           =   2
      End
      Begin VB.Menu mnu2_RxEdit1 
         Caption         =   "ȡ����..."
         Index           =   3
      End
      Begin VB.Menu mnu2_RxEdit1 
         Caption         =   "����..."
         Index           =   4
      End
      Begin VB.Menu mnu2_RxEdit1 
         Caption         =   "�ر�"
         Index           =   5
      End
      Begin VB.Menu mnu2_Line1 
         Caption         =   "-"
      End
      Begin VB.Menu mnu2_RxEdit2 
         Caption         =   "��Ʒ����ȫ��"
         Index           =   0
      End
      Begin VB.Menu mnu2_RxEdit2 
         Caption         =   "�����޸���Ʒ����..."
         Index           =   1
      End
      Begin VB.Menu mnu2_Line2 
         Caption         =   "-"
      End
      Begin VB.Menu mnu2_RxEdit3 
         Caption         =   "Simplified(��������GBK)"
         Index           =   0
      End
      Begin VB.Menu mnu2_RxEdit3 
         Caption         =   "Traditional(�c�餤��BIG5)"
         Checked         =   -1  'True
         Index           =   1
      End
   End
End
Attribute VB_Name = "MDIFormMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit


Private Sub MDIForm_Load()
If App.PrevInstance Then
 MsgBox "���������\�У�Ո�z�鴰���Ƿ���С��!", vbExclamation, "�����ѽ��\��!"
 Unload Me
 End
End If

ShowProgressInStatusBar True
Call setMenuText
GamePath = GetSetting("jyEditor", "startup", "gamepath", "")
If GamePath = "" Then
 MsgBox "Ո���O���[��λ�ã���t�o����݋", vbExclamation, "δ�O���[��·ޟ"
Else
 Me.Caption = "��ӹȺ�b���๦�ܾ�݋��(�[��·ޟ��" & GamePath & ")"
End If

mnuAbout_Click
If MsgBox("�Ǻ��w�����Ñ�᣿?" & vbCrLf & " Yes=GBK  /  No=BIG5", vbQuestion + vbYesNo) = vbYes Then
  mnu2_RxEdit3(0).Checked = True
  mnu2_RxEdit3(1).Checked = False
End If
Call iniVar(GamePath)
End Sub

Private Sub MDIForm_QueryUnload(Cancel As Integer, UnloadMode As Integer)
Unload Me
End
End Sub

Private Sub MDIForm_Unload(Cancel As Integer)
ShowProgressInStatusBar False
End
End Sub

Private Sub mnu1_refresh_Click()

End Sub







Private Sub mnuAbout_Click()
frmAbout.Show vbModal
End Sub

Private Sub mnuEditRx_Click()
Load frmRxEdit
End Sub

Private Sub mnuEditSx_Click()
'MsgBox "ע��:������û��ɣ�����������ϷĿ¼����ǰĬ��Ϊ��ȡ��һ���ļ����еĵ�ͼ������һ��Ҫ�ڽ�ӹȺ������Ϸ�ļ������ٽ�һ���ļ��У��ѱ����(��3���ļ�)���Ƶ����ļ����������У������Ҳ�����ͼ��", vbInformation, "��װ��֪"
 Me.MousePointer = 11
 Load frmMapEdit
 
End Sub

Private Sub mnuExit_Click()
Unload Me
End Sub


Private Sub mnuPicEdit31_Click()
frm320_200View.Show
End Sub

Private Sub mnuReadme_Click()
Shell "notepad " & App.Path & "\readme.txt", vbNormalFocus
End Sub

Private Sub mnuTileEdit_Click()
frmTileEdit.Show
End Sub

Private Sub nmuSetGamePath_Click()
 Dim TempFileName As String, I As Integer, tempStr As String
 'ComDlg1.Filter = "��ӹ������(z.*)|z.*|�浵�ļ�(*.grp)|*.grp|�����ļ�(*.*)|*.*"
 'ComDlg1.flags = 4  '��ֻ����ѡ
     Dim ofn As OPENFILENAME
    Dim Rtn As String

    ofn.lStructSize = Len(ofn)
    ofn.hwndOwner = Me.hwnd
    ofn.hInstance = App.hInstance
    ofn.lpstrFilter = "��ӹ������(Z.*);��n�ļ�(*.grp)"  '"��ӹ������(z.*)|z.*|��n�ļ�(*.grp)|*.grp|�����ļ�(*.*)|*.*"
    ofn.lpstrFile = Space(254)
    ofn.nMaxFile = 255
    ofn.lpstrFileTitle = Space(254)
    ofn.nMaxFileTitle = 255
    ofn.lpstrInitialDir = App.Path
    ofn.lpstrTitle = "���_��ӹ�[���ļ�"
    ofn.flags = 6148

    Rtn = GetOpenFileName(ofn)

    If Rtn >= 1 Then
        TempFileName = ofn.lpstrFile
    Else
        Exit Sub
    End If
 For I = Len(TempFileName) To 1 Step -1
  If Mid$(TempFileName, I, 1) = "\" Then
   tempStr = Left$(TempFileName, I)
   TempFileName = Right$(TempFileName, Len(TempFileName) - I)
   Exit For
  End If
 Next
 If GamePath = tempStr Then
     MsgBox "߀��ԭ����[��Ŀ�������]��׃", vbInformation
     Exit Sub
 Else
     GamePath = tempStr
     SaveSetting "jyEditor", "startup", "gamepath", GamePath
     Me.Caption = "��ӹȺ�b���๦�ܾ�݋��(�[��·ޟ��" & GamePath & ")"
     MsgBox "ע�⣺��׃�[��Ŀ���һ��Ҫ������һ�Έ�����݋����", vbExclamation
     Call iniVar(GamePath)
 End If

End Sub

Private Sub ShowProgressInStatusBar(ByVal bShowProgressBar As Boolean)
    Dim tRC As RECT
    If bShowProgressBar Then
        SendMessageAny StatusBar1.hwnd, SB_GETRECT, 1, tRC
        With tRC
            .Top = (.Top * Screen.TwipsPerPixelY)
            .Left = (.Left * Screen.TwipsPerPixelX)
            .Bottom = (.Bottom * Screen.TwipsPerPixelY) - .Top
            .Right = (.Right * Screen.TwipsPerPixelX) - .Left
        End With
        With ProgressBar1
            SetParent .hwnd, StatusBar1.hwnd
            .Move tRC.Left, tRC.Top, 1, tRC.Bottom
            .Visible = False
            .Value = 0
        End With
    Else
        SetParent ProgressBar1.hwnd, Me.hwnd
        ProgressBar1.Visible = False
    End If
End Sub

Private Sub mnu1MapEdit1_Click(Index As Integer) '��ͼ�༭�˵�
frmMapEdit.DoWithMenu1 Index
End Sub

Private Sub mnu1_Layer_Click(Index As Integer)
frmMapEdit.DoWithMenu2 Index
End Sub

Private Sub mnu1_Jindu_Click(Index As Integer)
frmMapEdit.DoWithMenu3 Index
End Sub

Private Sub mnu1MapEdit2_Click(Index As Integer)
frmMapEdit.DoWithMenu4 Index
End Sub
Private Sub mnu1_Grid_Click(Index As Integer)
frmMapEdit.DoWithMenu5 Index
End Sub

Private Sub mnu2_RxEdit1_Click(Index As Integer)
frmRxEdit.DoWithMenu1 Index
End Sub
Private Sub mnu2_RxEdit2_Click(Index As Integer)
frmRxEdit.DoWithMenu2 Index
End Sub
Private Sub mnu2_RxEdit3_Click(Index As Integer)
frmRxEdit.DoWithMenu3 Index
End Sub
Private Sub setMenuText()
mnuFuncView.Caption = "����/�O��(&O)"
nmuSetGamePath.Caption = "�O���[���bλ��"
mnuExit.Caption = "�˳�(&X)"
mnuSavEdit.Caption = "�[��݋(&E)"
mnuEditRx.Caption = "�[������M��"
mnuEditSx.Caption = "�����؈D/�¼�"
mnuHelp.Caption = "����(&H)"
mnuReadme.Caption = "����(&C)"
mnuAbout.Caption = "�P�(&A)"
mnuPicEdit.Caption = "ͼ��/��ͼ(&P)"
mnuTileEdit.Caption = "�N�D�YԴ"
mnuPicEdit31.Caption = "���}��Ƭβȫ���Ӯ�"

mnu1MapEdit1(0).Caption = "ˢ�D��"
mnu1MapEdit1(1).Caption = "С�؈D"
mnu1MapEdit1(2).Caption = "�}�Ƶ����N��"
mnu1MapEdit1(3).Caption = "Windows���D"
mnu1MapEdit1(4).Caption = "���S��݋"
mnu1MapEdit1(5).Caption = "�@ʾ�¼���ӛ"
mnu1MapEdit1(6).Caption = "�N�D�x����"
mnu1_layer1.Caption = "�D��"
mnu1_Layer(0).Caption = "0��(�ر�)"
mnu1_Layer(1).Caption = "1��(���B/ֲ��)"
mnu1_Layer(2).Caption = "2��(����)"
mnu1_Layer(3).Caption = "3��(�¼�)"
mnu1_Layer(4).Caption = "4��(Yƫ�ƌ���1/3��)"
mnu1_Layer(5).Caption = "5��(Yƫ�ƌ���2��)"
mnu1_Grid1.Caption = "�W��"
mnu1_Grid(0).Caption = "���@ʾ"
mnu1_Grid(1).Caption = "�W���ɫ"
mnu1_Grid(2).Caption = "4X"
mnu1_Grid(3).Caption = "8X"
mnu1_Grid(4).Caption = "16X"
mnu1_Grid(5).Caption = "32X"
mnu1_Grid(6).Caption = "64X"
mnu1_Grid(7).Caption = "128X"
mnu1_Jindu1.Caption = "ȡ�M��"
mnu1_Jindu(0).Caption = "�M��һ"
mnu1_Jindu(1).Caption = "�M�ȶ�  "
mnu1_Jindu(2).Caption = "�M����"
mnu1MapEdit2(0).Caption = "���殔ǰ����"
mnu1MapEdit2(1).Caption = "�P�]�؈D��݋��"

End Sub
