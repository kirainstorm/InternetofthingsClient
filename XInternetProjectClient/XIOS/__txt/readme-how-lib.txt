在build setting --- “Search Paths”的 header search paths / library  search paths栏目中分别为头文件和库文件设置路径。


///-------------------------------
添加OpenGL与GLKit相关frameworks
Build Phases栏，在“Link Binary With Libraries”中加入
GLKit
OpenGLES
QuartzCore
libz.tdb
libbz2.1.0.tdb
libiconv.2.4.0.tbd


#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>



使用静态库
设置 --- build phases ---- link binary with libraries  点击+号，可以看到同一个工作空间的其他静态库


ENABLE_BITCODE 错误

Build Settings 搜索 Enable Bitcode  改为NO,解决方案里的所有工程都改




在使用c++头文件的地方，将.m改为.mm，，包括包含该类的头文件的.m文件俺

//glkview页面需要使用presentViewController
弹出
dispatch_async(dispatch_get_main_queue(), ^{
        
PersonalPlayViewController * playview = ([[PersonalPlayViewController alloc] initWithNibName:@"PersonalPlayViewController" bundle:nil]);
        
playview.rowid=indexPath.row;
        
playview.view.backgroundColor=[UIColor whiteColor];
        
[self presentViewController:playview animated:YES completion:nil ];//glkview need:presentViewController
        
    
});