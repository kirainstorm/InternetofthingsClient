对于debug/release选择 菜单栏-Product-Scheme-Edit Scheme 



将__IOSInc   __IOSLib   monitor拷贝到目标文件夹


monitor需要修改的地方：
1:

build settting---header search path

../__IOSInc ../__IOSInc/boost_1_56_0


2:
build settting---library search path

debug:
../__IOSLib/debug ../__IOSLib

release:
../__IOSLib/release ../__IOSLib


3:
general---linked frameworks and libraries

将相关的.a添加进来








