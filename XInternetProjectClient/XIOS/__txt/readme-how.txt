Xcode ͷ�ļ�·��

///////
ʹ��������ԣ�ģ�����಻������������������




��build setting --- ��Search Paths���� header search paths / library  search paths��Ŀ�зֱ�Ϊͷ�ļ��Ϳ��ļ�����·����


//---------------------------------
����xib

file--new--file-- ios(source) -cocoa touch class-next---subclass of ѡ��UIViewController / UITableViewController �ȵȣ���ѡ�� also create xib file��


//---------------------------------
�����ؼ��ʹ���
.h��
@interface ViewController : UIViewController  
{  
    UILabel *titleLabel;  
}  
  
@property(nonatomic,retain)IBOutlet UILabel *titleLabel;  ;  
  
-(IBAction)Edit:(id)sender; //��ť����Edit


.m��
@synthesize titleLabel;

-(IBAction)Edit:(id)sender
{
}

��xib
�Ҽ�file's owner
��קСԲȦ����ͬ�ؼ���
//------------------------------
XCode ΪiPhone��������ͼ��ķ���
�����һ��PNGΪ��׺��ͼ��, ����������Ϊ icon.png �Ϳ�����.  �ٴ����� IPhone ģ����. �ͳɹ���.
��ʱ�ߴ粻������˹淶,Ҳ�ǿ�����ʾ������,������Ч����.
ΪiPhone������������ֻ��Ҫ��ͼƬ��������Ϊ:Default.png ����.
icon�ߴ�: 57*57  ����  114*114
Default�ߴ�: 320*480 ���� 640:960


//---------------------------------
����auto layout��size classes
iphone�ĺ������������Ե������ڣ�
ipad���У�����so��������ʱֻ��iphone���������iphone������������������

���� ��any any ��ͼ����ק�ø����ؼ�������Ų��ֺã��� compact regular�е�����������any compact�е�������


//-----------------------------
xcode��xib�Ŀؼ��Ĳ��
��ϵ
�ڱ༭���е���ÿ��view��˳���ʼ����������
�磺��ʾview��
          label1��
          label2.
Ҫ����label1���ϲ㣬��label1�ϵ�label2����Ϳ�����

///-------------------------------
���OpenGL��GLKit���frameworks
Build Phases�����ڡ�Link Binary With Libraries���м���GLKit, OpenGLES,QuartzCore������framework
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

//-----------------------------
������̿�����ӵ�һ�������ռ�workspace��

//-----------------------------
ʹ�þ�̬��
���� --- build phases ---- link binary with libraries  ���+�ţ����Կ���ͬһ�������ռ��������̬��


//-------------
boost�⣿���� ����build setting ���� bitcode �� yes��Ϊno



XCode���ÿ�ݼ���ʹ��
һ���������е���

1�����У�ֹͣ�����ڹ�������Product�

Command + R  ���С�

Command + .  ֹͣ

2��F6�������ԡ�F7���룬F8������ ��Eclipse,VS����

