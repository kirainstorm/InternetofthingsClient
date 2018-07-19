bootstrap-ios.sh拷贝到目录下直接运行即可


# 库文件最终放在./stage/lib/下




集成到Xcode

做iOS的同学还是习惯完全用Xcode的吧。所以可以把上面的脚本集成到xcode里。在某个工程中，Xcode菜单File->New->Target…->Other->Aggregate->取名boost，Finish->在这个target的属性中选择Build Phases->点击左边的+号->New Run Script Phase->填入bash ./build_boost.sh->自己再填好库文件搜索路径和链接各个boost库。主target还要设置依赖boost target，免得boost编译前就链接以致错误。