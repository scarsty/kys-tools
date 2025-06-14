unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes,
  SysUtils,
  FileUtil,
  Forms,
  Controls,
  Graphics,
  Dialogs,
  StdCtrls,
  LConvEncoding;

type

  { TForm1 }

  TForm1 = class(TForm)
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    Button4: TButton;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    //procedure FormCreate(Sender: TObject);
  private
    { private declarations }
  public
    { public declarations }
  end;

  TIntArray = array of integer;
  TByteArray = array of byte;

var
  Form1: TForm1;
  list, list1: TStringList;
  b, i, i1, t1, t: integer;
  tempstr: utf8string;
  tempstr1: ansistring;
  cf: char;
  TDef: TByteArray;
  TIdx: TIntArray;

implementation

{$R *.lfm}

{ TForm1 }

procedure ReadTalk(talknum: integer; var talk: Tbytearray; needxor: integer = 0);
var
  len, offset, i: integer;
begin
  len := 0;
  if talknum = 0 then
  begin
    offset := 0;
    len := TIdx[0];
  end
  else
  begin
    offset := TIdx[talknum - 1];
    len := TIdx[talknum] - offset;
  end;

  setlength(talk, len + 1);
  move(TDef[offset], talk[0], len);
  if needxor = 1 then
    for i := 0 to len - 1 do
    begin
      talk[i] := talk[i] xor $FF;
      if talk[i] = 255 then
        talk[i] := 0;
      if talk[i] = 13 then
        talk[i] := 32;
      if talk[i] = 10 then
        talk[i] := 32;
    end;
  talk[len] := 0;
end;

//载入IDX和GRP文件到变长数据, 不适于非变长数据

function LoadIdxGrp(stridx, strgrp: string; var idxarray: TIntArray; var grparray: TByteArray): integer;
var
  idx, grp, len, tnum: integer;
begin
  tnum := 0;
  if FileExists(strgrp) and FileExists(stridx) then
  begin
    grp := FileOpen(strgrp, fmopenread);
    len := FileSeek(grp, 0, 2);
    setlength(grparray, len + 4);
    FileSeek(grp, 0, 0);
    FileRead(grp, grparray[0], len);
    FileClose(grp);

    idx := FileOpen(stridx, fmopenread);
    tnum := FileSeek(idx, 0, 2) div 4;
    setlength(idxarray, tnum + 1);
    FileSeek(idx, 0, 0);
    FileRead(idx, idxarray[0], tnum * 4);
    FileClose(idx);
  end;
  Result := tnum;

end;



procedure TForm1.Button1Click(Sender: TObject);
begin
  list := TStringList.Create;
  list.loadfromfile('talkutf8.txt');
  b := filecreate('talk1.grp');
  i1 := filecreate('talk1.idx');
  t1 := 0;
  for i := 0 to list.Count - 1 do
  begin
    tempstr := (list.Strings[i]);
    t1 := t1 + FileWrite(b, tempstr[1], length(tempstr));
    cf := char(0);
    t1 := t1 + FileWrite(b, cf, 1);
    t1 := t1 + FileWrite(b, cf, 1);
    FileWrite(i1, t1, 4);
  end;
  //list.SaveToFile('talkutf8.txt');
  FileClose(b);
  FileClose(i1);

  b := filecreate('talk.grp');
  i1 := filecreate('talk.idx');
  t1 := 0;
  for i := 0 to list.Count - 1 do
  begin
    tempstr1 := UTF8ToCP950(list.Strings[i]);
    for t := 1 to length(tempstr1) do
      tempstr1[t] := char(byte(tempstr1[t]) xor $FF);
    t1 := t1 + FileWrite(b, tempstr1[1], length(tempstr1));
    cf := char(0);
    t1 := t1 + FileWrite(b, cf, 1);
    //t1 := t1 + filewrite(b, cf, 1);
    FileWrite(i1, t1, 4);
  end;
  //list.SaveToFile('talkutf8.txt');
  FileClose(b);
  FileClose(i1);


  list.Free;
  ShowMessage('Done.');
  copyfile('talk1.grp', 'talk2.grp', False);
  copyfile('talk1.idx', 'talk2.idx', False);

end;

procedure TForm1.Button2Click(Sender: TObject);
var
  namestr: array of byte;
  list: TStringList;
  str: string;
begin
  list := TStringList.Create;
  for i := 0 to LoadIdxGrp('talk.idx', 'talk.grp', TIdx, TDef) - 1 do
  begin
    readtalk(i, Namestr, 1);
    if length(namestr) > 0 then
      tempstr := UTF8Decode(CP950ToUTF8(PChar(@namestr[0])))
    else
      tempstr := '';
    str := UTF8Encode(tempstr);
    list.add(str);
  end;
  list.SaveToFile('talkutf8.txt');
  list.Free;
  ShowMessage('Done.');
end;

procedure TForm1.Button3Click(Sender: TObject);
var
  namestr: array of byte;
  list: TStringList;
  str: string;
begin
  list := TStringList.Create;
  for i := 0 to LoadIdxGrp('talk1.idx', 'talk1.grp', TIdx, TDef) - 1 do
  begin
    readtalk(i, Namestr, 0);
    if length(namestr) > 0 then
      tempstr := PwideChar(@namestr[0])
    else
      tempstr := '';
    str := UTF8Encode(tempstr);
    list.add(str);
  end;
  list.SaveToFile('talkutf8export.txt');
  list.Free;
  ShowMessage('Done.');
end;

procedure TForm1.Button4Click(Sender: TObject);
begin
  list := TStringList.Create;
  list.loadfromfile('talkutf8.txt');
  list1 := TStringList.Create;
  list1.loadfromfile('1.txt');
  for i := 0 to list.Count - 1 do
  begin
    if list1.Strings[i] <> '0' then
      list.Strings[i]:='';
  end;
  list.SaveToFile('talkutf8new.txt');
  list.Free;
  list1.Free;
  ShowMessage('Done.');

end;


end.
