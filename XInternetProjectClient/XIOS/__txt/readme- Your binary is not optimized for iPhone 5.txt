http://blog.csdn.net/wsjshx/article/details/49558627


xcode7的那些坑－“Your binary is not optimized for iPhone 5” (ITMS-90096) when submitting 
标签： xcodeapp storeappios开发 
2015-11-01 15:49 7295人阅读 评论(4) 收藏 举报 
版权声明：本文为博主原创文章，未经博主允许不得转载。
用Xcode7向App Store提交二进制文件是，提示ERROR ITMS-90096："You binary is not?optimized for iPhone 5....".
在工程里的Images.xcassets添加并设置LaunchImage对解决ERROR ITMS-90096根本不会起到任何作用，需要单独添加针对iPhone 5的载入图片。关键点有三项：1、图片的名称必须叫Default-568.png。2、必须把图片放在工程的根目录下。3、图片尺寸必须是320*568。
然后在plist中添加
<key>UILaunchImages</key>
<array>
    <dict>
        <key>UILaunchImageName</key>
        <string>Default-568</string>
        <key>UILaunchImageSize</key>
        <string>{320, 568}</string>
    </dict>
</array>
如图：

这样即可上传二进制文件了。