unit Unit1;

interface

uses
  System.SysUtils, System.Types, System.UITypes, System.Classes, System.Variants,
  FMX.Types, FMX.Controls, FMX.Forms, FMX.Graphics, FMX.Dialogs, FMX.StdCtrls,
  FMX.Controls.Presentation, FMX.Edit, IdBaseComponent, IdComponent,
  IdTCPConnection, IdTCPClient, IdHTTP, FMX.ScrollBox, FMX.Memo;

type
  TForm1 = class(TForm)
    Label1: TLabel;
    IdHTTP1: TIdHTTP;
    Edit1: TEdit;
    Label2: TLabel;
    Switch1: TSwitch;
    Label3: TLabel;
    Switch2: TSwitch;
    Label4: TLabel;
    Switch3: TSwitch;
    Label5: TLabel;
    Switch4: TSwitch;
    Label6: TLabel;
    Button1: TButton;
    procedure Button1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure Switch1Click(Sender: TObject);
    procedure Switch2Click(Sender: TObject);
    procedure Switch3Click(Sender: TObject);
    procedure Switch4Click(Sender: TObject);
    procedure Label1Click(Sender: TObject);

  private
    { Private declarations }
    procedure UpLedStatus( web_data:string);
  public
    { Public declarations }
  end;




var
  Form1: TForm1;


led5,led4,led3,led2:boolean;


implementation

{$R *.fmx}


procedure TForm1.Button1Click(Sender: TObject);
var
  url:string;
  web_data:string;
begin

  url := 'http://' + edit1.Text ;
  web_data := IdHTTP1.Get(url);
  UpLedStatus(web_data);
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
//窗口打开初始变量
  led5:=false;
  led4:=false;
  led3:=false;
  led2:=false;
end;




procedure TForm1.Label1Click(Sender: TObject);
begin

end;

//LED5开关切换
procedure TForm1.Switch1Click(Sender: TObject);
var
  url:string;
  status_char:string;
  web_data:string;
begin

  //如果灯是打开,那么执行关闭
  if led5 then
  begin
     status_char:= '/5/off';
  end
  else
    begin
      status_char:= '/5/on';
    end;


  url := 'http://' + edit1.Text + status_char ;
  web_data := IdHTTP1.Get(url);
  UpLedStatus(web_data);
end;


//LED4开关切换
procedure TForm1.Switch2Click(Sender: TObject);
var
  url:string;
  status_char:string;
  web_data:string;
begin

  //如果灯是打开,那么执行关闭
  if led4 then
  begin
     status_char:= '/4/off';
  end
  else
    begin
      status_char:= '/4/on';
    end;


  url := 'http://' + edit1.Text + status_char ;
  web_data := IdHTTP1.Get(url);
  UpLedStatus(web_data);
end;



 //LED3开关切换
procedure TForm1.Switch3Click(Sender: TObject);
var
  url:string;
  status_char:string;
  web_data:string;
begin

  //如果灯是打开,那么执行关闭
  if led3 then
  begin
     status_char:= '/3/off';
  end
  else
    begin
      status_char:= '/3/on';
    end;


  url := 'http://' + edit1.Text + status_char ;
  web_data := IdHTTP1.Get(url);
  UpLedStatus(web_data);
end;



//舵机2开关
procedure TForm1.Switch4Click(Sender: TObject);
var
  url:string;
  status_char:string;
  web_data:string;
begin

  //如果灯是打开,那么执行关闭
  if led2 then
  begin
     status_char:= '/2/off';
  end
  else
    begin
      status_char:= '/2/on';
    end;


  url := 'http://' + edit1.Text + status_char ;
  web_data := IdHTTP1.Get(url);
  UpLedStatus(web_data);
end;




procedure TForm1.UpLedStatus(web_data:string) ;
begin




  //如果网页代码包含/5/on 就表示5处于关闭状态
  if (Pos('href="/5/on"',web_data)>0) then
  begin
      led5:=false;
  end
    else
    begin
      led5:=true;
    end;


   Switch1.IsChecked:=  led5 ; //灯5开关状态




  //如果网页代码包含/4/on 就表示4处于关闭状态
  if (Pos('href="/4/on"',web_data)>0) then
  begin
      led4:=false;
  end
    else
    begin
      led4:=true;
    end;


   Switch2.IsChecked:=  led4 ; //灯4开关状态



  //如果网页代码包含/3/on 就表示3处于关闭状态
  if (Pos('href="/3/on"',web_data)>0) then
  begin
      led3:=false;
  end
    else
    begin
      led3:=true;
    end;


   Switch3.IsChecked:=  led3 ; //灯3开关状态


  //如果网页代码包含/2/on 就表示舵机处于关闭状态
  if (Pos('href="/2/on"',web_data)>0) then
  begin
      led2:=false;
  end
    else
    begin
      led2:=true;
    end;


   Switch4.IsChecked:=  led2 ; //舵机开关状态


end;

end.
