http://yuedu.baidu.com/ebook/31beb61a9b6648d7c1c746e8?pn=3&pa=1

配置JAVA环境变量

右击计算机 —> 属性 —> 高级系统设置 —> 环境变量 

新建系统变量JAVA_HOME —> 变量值为刚才安装的JDK的路径
在系统变量path中添加 %JAVA_HOME%\bin
新建系统变量CLASS_PATH —> 添加变量值 %JAVA_HOME%\lib\dt.jar;%JAVA_HOME%\lib\tools.jar;

到此为止环境变量配置完毕,下面来验证下是否配置成功.

CMD终端输入javac,如果显示帮助信息就证明配置正确了




配置ANDROID的环境变量
右击计算机 —> 属性 —> 高级系统设置 —> 环境变量 —> 新建系统变量ANDROID_HOME —> 变量值为刚才安装的SDK的路径C:\Users\kiki\AppData\Local\Android\sdk
接下来在path中添加Android sdk和Android工具的路径
%ANDROID_HOME%;%ANDROID_HOME%\tools;%ANDROID_HOME%\build-tools;%ANDROID_HOME%\platform-tools;

CMD终端输入adb device ,只要不报adb不是内部命令就是配置成功啦.




管理Android SDK
欢迎界面 —> Configure —> SDK Manager  选择要使用的sdk版本，否则就下载




Android Studio中的两个重要的概念:  项目(Project)和模块(Module) .
模块(Module):
Module是一个可以单独的运行和调试的application或公共库,它相当于Eclipse当中的Project.

每个module中都有自己的build.gradle文件用来配置模块的构建任务.
在build.gradle文件中我们可以配置SDK版本、构建工具版本、应用程序版本、打包参数、模块的依赖等等.

打包参数、模块的依赖等等.
项目(Project):
Project可以理解为一个完整的APP项目,它由application module和一些依赖的module组成, 相当于Eclipse当中的workspace.
一个Project中有多个module.



新建项目向导
 
欢迎界面: Start a new Android Studio project

如果你是第一次使用Android Studio创建项目, 可能会比较慢,因为Android Studio第一次会先下载Gradle.
project的选项卡有下拉列表，选择project显示所有文件。android类似eclipse目录结构


创建应用程序模块
操作步骤:
第1步:菜单栏: File —> New —> New Module
然后弹出创建模块界面:

为应用程序添加模块依赖:
操作步骤:
 打开Project Structure —> 选择你要操作的应用程序 —> 添加已存在项目中的模块作为依赖


删除模块
操作步骤:
 打开Project Structure —> 选中要删除的module --> 点击左上角的"-"

确定后,Gradle会进行同步.在项目的settings.gradle中会去掉我们删除的模块,本例中是appimport.
此时模块并不是直接被删除了,而是需要我们再执行一次删除.
在项目中找到appimport,右击后调出操作菜单,点击"Delete..."




在build.gradle中把miniSdk攺为



添加so文件
如何加载so文件？

假设你的so文件为:libBaiduMapSDK_v350_1.so

这样加载:
String libName = "BaiduMapSDK_v350_1"; // 注意:库名libName, 没有前缀lib和后缀.so  

System.loadLibrary( libName ); //在使用前一定要先加载

so文件应该放在哪里？

放到moduleName/src/main/jniLibs目录下，如果没有jinLibs目录就新建一个.armeabi、armeabi-v7a、mips、x86这四个文件夹是干嘛的呢?
表示四种不同的cpu类型，不同的cpu的特性不一样,在使用so文件时注意区分.
可以直接复制so文件粘贴到对应的文件夹下:ok之后so文件就进来啦.



新建类文件:
菜单栏 —> File —> New —> Java Class —> 输入类名,选择类模板
默认的类模板:
普通类:
接口类:枚举类:注解类:单例类

新建其它文件
我们可以直接新建各种格式的文件,有一些Android Studio支持的会直接新建成功,如果不支持的，需要先指定默认打开文件类型,只需要一次设置,以后就默认使用这个格式了.
实例演示：
第一次新建markdown文件 README.md.
➤ 菜单栏 —> File —> New —> File —> 输入文件名和格式,如下图:
OK —> 因为Android Studio无法识别markdown文件,需要指定一个默认的打开格式,我们在这里指定为Text.点击OK设置成功，以后再新建markdown文件都为默认使用Text格式打开.
如果你安装了markdown插件,那么默认就会使用markdown插件打开。



创建Activity

新建的Activity是否作为启动的Activity,如果是请勾选,如果不是就不要勾选.

Hierarchical Parent : 父层次

我们可以选择已存的在Activity作为父层次.

如果配置了父层次,在新建的Activity上点击回退键会回到这里配置的父层次.
当然也可以为空,我们这里就为空.

因为我们在新建Activity时将NavigationDrawerActivity设置为Launcher Activity,所以在AndroidManifest.xml文件中添加了<category android:name="android.intent.category.LAUNCHER" />.

但是可能有多个Activity声明为Launcher Activity,我们需要指定一下.

操作步骤:
➤ 打开运行和调试配置界面 运行的图标左面的下拉里面的edit configuration —> app —> General —> Launch Options
  
Launch使用默认的Activity作为Launcher Activity.

➤ 选择Specified Activity —> 选择Activity —> OK






 创建Service文件
操作步骤:
➤ 菜单栏: File —> New —> Service
然后弹出【New Android Component】界面:

创建Intent Service文件
操作步骤:
➤ 菜单栏: File —> New —>  Service —> Service(IntentService)
然后弹出【New Android Component】界面:



创建自定义组件
有时Android提供的组件无法满足我们的需求,因此需要自定义组件.
创建自定义组件的一般步骤:
1.新建类文件,要继承View或View的子类.
2.覆写父类的一些方法.
3.使用自定义组件类.
Android Studio会通过模板帮我们自动生成文件,并覆写方法. 我们只需要根据
自已的实际需求修修攺改就好了.
操作步骤:
➤ 菜单栏: File —> New —>UI Component —> Custom View
然后弹出【New Android Component】界面:



创建App Widget

App Widget是应用程序的窗口小部件,它可以被嵌入到其它应用程序中(如桌面)并接收周期性的更新.

创建Widget的一般步骤:
第1步:在res/layout目录下创建一个Widget布局文件.

第2步:创建一个类继承AppWidgetProvider.

第3步:在res/xml目录下创建一个XML文件,用来定义Widget的特性.

第4步:在AndroidManifest.xml中声明Widget.

使用Android Studio的模板功能,可以帮我们自动完成上面这些步骤.
操作步骤:
➤ 菜单栏: File —> New —> Widget —> App Widget

Class Name: 类名,继承AppWidgetProvider.
Placement: Widget 放在哪儿.  

1.Home-screen and Keyguard: 在主屏幕和锁键上.

2.Home-screen only: 仅在主屏幕上.

3.Keyboard only(API 17+): 仅在锁键上(只支持Android4.2及以上版本).

Resizable(API 12+ ): Widget是否可调整大小,只支持Android 3.1及以上版本.

1.Horizontally and vertically: 水平和垂直显示时可调整.

2.Only Horizontally: 仅水平时可调整.

3.Only vertically: 仅垂直时可调整.

4.Not resizable: 不可调整.

Minimum Width: 最小宽度,参照左边预览窗口的单元格.
Minimum Height: 最小高度,参照左边预览窗口的单元格.
Configuration Screen:勾选后会生成widgets配置activity.

使用默认配置,点击【Finish】后创建成功







资源限定符
在新建资源文件时可以选择资源限定符.
在Available qualifiers列表中列出了适配时常用的一些限定符,选择限定符,再进行相应的配置,就可以在res目录下生成相应的资源文件夹.

Country Code:
移动设备国家代码:唯一识别移动用户所属的国家,共3位,中国为460;　

Network Code:
移动设备网络代码:是与Country Code相结合，以用来表示唯一一个的移动设备的网络运营商.

Locale：
国际化(多语言):如果用户改变了系统中的语言设置，那么在应用程序的运行期间也能够改变为对应的语言.

Layout Direction：
布局方向.
LTR: 从左到右    RTL: 从右到左

Smallest Screen Width:
最小屏幕宽度:这个值是布局支持的最小宽度，而不管屏幕的当前方向.

Screen Width:
最小的可用屏幕宽度，当方向在横向和纵向之间改变时，这个配置值会跟当前的实际的宽度相匹配.

Screen Height:
最小的可用屏幕高度，当方向在横向和纵向之间改变时，这个配置值会跟当前的实际的高度相匹配.

Size:
屏幕尺寸.=====Small: 小屏幕尺寸,类似低分辨率的QVGA屏幕,大约是320x426dp;=====Normal: 中等屏幕尺寸,类似中等分辨率的HVGA屏幕,大约是320x470dp;====Large: 大屏幕尺寸,类似中等分辨率的VGA屏幕,大约是480x640dp；====X-large: 超大屏幕尺寸,比HVGA屏幕大,大约是720x960dp;

Ratio:
宽高比率：指的是实际的物理尺寸宽高比率
====long: 长屏幕    ====nolong: 非长屏幕

Orientation:
限制横竖屏切换.
portrait竖屏、landscape横屏、square正方形

UI Mode:
UI模式.
Car Dock:车座、Desk Dock:桌座、Television:电视上、Appliance:装置、Watch:手表
当用户将手机放在不同的Dock上或插入不同的地方,应用程序在运行期间就能够改变这个限定.

Night Mode:
夜间模式.
Not Ntight: 白天
Night: 夜间

Density:
密度
指定不同密度的手机使用不同的资源,如果没有提供与当前设备配置匹配的可选资源，那么系统会使用最接近的资源.

Touch Screen:
触屏类型:
No Touch: 非触屏
Stylus: 触控笔
Finger: 手指(触屏设备)

Keyboard:
键盘类型.
Exposed: 设备有可用的键盘(硬或软)
Hidden: 设备有可用的硬键盘（被隐藏,且无可用的软键盘）
Soft: 设备有可用的软键盘(不管否可见)

Text Input:
文本输入法
No keys:  设备没有用于文本输入的硬键盘
Qwerty:  设备有标准的硬键盘,不管用户是否可见
12 Key:  设备有12个键的硬键盘,不管用户是否可见.

Navigation State:
导航键的状态.
Exposed: 导航键可用 
Hidden: 导航键不可用
如果用户能够看到导航键，那么在应用程序运行时就能够改变这个限定.

Navigation Method:
导航方法:None:除了使用触屏以外,设备没有其他导航设施、 D-pad:设备有用于导航的定向板、 Trackball:设备有用于导航的轨迹球、 Wheel:设备有用于导航的定向滚轮

Dimension:
自定义屏幕尺寸

Version:
版本.





创建不同分辨率的图标
此功能可以快速方便的把现有的图片、剪贴画或文本生成不同分辨率的应用图标.
操作步骤:
 ➤ 菜单栏: File  | 右击Module —> New —>Image Asset —> 弹出Asset Studio对话框.
通过Icon Type切换不同的图标类型.
2.13.1 
启动图标
Icon Type 选择【 Launcher Icons】
使用本地图片
Assert Type选择Image,然后我们可以选择一个本地的图片,自动生成不同分辨率的图标.

使用剪贴画
Assert Type选择Clip Art,会有一些剪贴画提供给你选择.

使用文本
Assert Type选择Text

活动栏和选项卡图标
Icon Type 选择 【Action Bar and Tab Icons】
 ----- 略。。。
通知图标
Icon Type 选择【 Notification Icons】

 ----- 略。。。



创建矢量图
在Android5.0(API Level 21)中,我们可以使用矢量图. 矢量图的特点是它不会因为图像的缩放而失真.

操作步骤:
➤ 右击module —> New —> Vector Asset
然后打开Configure Vector Asset界面:


使用定义好的素材图标
选中【Material Icon】—> 点击Icon —> 选择定义好的素材图标 —> 设置大小和透明度 —> 查看预览效果
，，点击 Next —> 默认输出目录为res/drawable
使用本地的SVG文件
选中【Local SVG file】—> 在Path中选择本地的svg文件



创建AIDL文件
Android 接口定义语言 AIDL（Android Interface Definition Language）是一种接口描述语言.
AIDL的IPC的机制是基于接口的，该接口描述了与一个远程对象进行通信的抽象协议.

 ----- 略。。。



新建Android文件夹
在Android Studio中提供了新建文件夹的功能,在这里可以新建一些缺省的文件夹.



创建Resource Bundle文件
ResourceBundle(资源绑定)最大的好处是可以使你的程序国际化，在bundle文件名后加上国家、语言关键字，那系统就会自动载入相应的bundle文件.
properties文件一般的命名规范是： 自定义名_语言代码_国别代码.properties

例如:

success.properties 缺省

success_zh.properties 中文

success_ja.properties 日文

如果你的系统是日文,就会自动加载success_ja.properties.
如果你的系统是中文,success.properties和success_zh.properties都存在,则会优先使用success_zh.properties文件.当success_zh.properties文件不存在时会使用success.properties.

新建Resource Bundle文件
操作步骤:
➤  右击Module —> New —> Resource Bundle —> 输入文件名“success” —>添加语言代码

双击 properties文件 —> 切换到Resource Bundle —> 点击 + 按钮 —> 输入key
➤  双击key —> 编辑不同语言的value


















