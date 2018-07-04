Xcode 头文件路径

///////
使用真机调试，模拟器编不过。。。。。。。。




在build setting --- “Search Paths”的 header search paths / library  search paths栏目中分别为头文件和库文件设置路径。


//---------------------------------
创建xib

file--new--file-- ios(source) -cocoa touch class-next---subclass of 选择UIViewController / UITableViewController 等等，，选中 also create xib file、


//---------------------------------
关联控件和代码
.h中
@interface ViewController : UIViewController  
{  
    UILabel *titleLabel;  
}  
  
@property(nonatomic,retain)IBOutlet UILabel *titleLabel;  ;  
  
-(IBAction)Edit:(id)sender; //按钮操作Edit


.m中
@synthesize titleLabel;

-(IBAction)Edit:(id)sender
{
}

打开xib
右键file's owner
拖拽小圆圈到不同控件上
//------------------------------
XCode 为iPhone程序设置图标的方法
随便找一个PNG为后缀的图标, 把它重命名为 icon.png 就可以了.  再次启动 IPhone 模拟器. 就成功了.
及时尺寸不符合审核规范,也是可以显示出来的,供测试效果吧.
为iPhone设置启动画面只需要将图片名称命名为:Default.png 即可.
icon尺寸: 57*57  高清  114*114
Default尺寸: 320*480 高清 640:960


//---------------------------------
对于auto layout和size classes
iphone的横屏和竖屏可以单独调节，
ipad不行，，，so。。。暂时只做iphone，以下针对iphone！！！！！！！！！

方法 在any any 视图中拖拽好各个控件，并大概布局好，在 compact regular中调整竖屏，在any compact中调整横屏


//-----------------------------
xcode的xib的控件的层次
关系
在编辑器中调整每隔view的顺序，最开始的在最下面
如：显示view，
          label1，
          label2.
要想让label1在上层，把label1拖到label2下面就可以了

///-------------------------------
添加OpenGL与GLKit相关frameworks
Build Phases栏，在“Link Binary With Libraries”中加入GLKit, OpenGLES,QuartzCore这三个framework
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

//-----------------------------
多个工程可以添加到一个工作空间workspace中

//-----------------------------
使用静态库
设置 --- build phases ---- link binary with libraries  点击+号，可以看到同一个工作空间的其他静态库


//-------------
boost库？？？ 导致build setting 搜索 bitcode 将 yes改为no



XCode常用快捷键的使用
一、关于运行调试

1、运行，停止，都在工具栏的Product里。

Command + R  运行。

Command + .  停止

2、F6单步调试、F7跳入，F8继续， 和Eclipse,VS类似

