��build setting --- ��Search Paths���� header search paths / library  search paths��Ŀ�зֱ�Ϊͷ�ļ��Ϳ��ļ�����·����


///-------------------------------
���OpenGL��GLKit���frameworks
Build Phases�����ڡ�Link Binary With Libraries���м���
GLKit
OpenGLES
QuartzCore
libz.tdb
libbz2.1.0.tdb
libiconv.2.4.0.tbd


#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>



ʹ�þ�̬��
���� --- build phases ---- link binary with libraries  ���+�ţ����Կ���ͬһ�������ռ��������̬��


ENABLE_BITCODE ����

Build Settings ���� Enable Bitcode  ��ΪNO,�������������й��̶���




��ʹ��c++ͷ�ļ��ĵط�����.m��Ϊ.mm�����������������ͷ�ļ���.m�ļ���

//glkviewҳ����Ҫʹ��presentViewController
����
dispatch_async(dispatch_get_main_queue(), ^{
        
PersonalPlayViewController * playview = ([[PersonalPlayViewController alloc] initWithNibName:@"PersonalPlayViewController" bundle:nil]);
        
playview.rowid=indexPath.row;
        
playview.view.backgroundColor=[UIColor whiteColor];
        
[self presentViewController:playview animated:YES completion:nil ];//glkview need:presentViewController
        
    
});