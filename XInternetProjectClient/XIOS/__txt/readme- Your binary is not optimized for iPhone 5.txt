http://blog.csdn.net/wsjshx/article/details/49558627


xcode7����Щ�ӣ���Your binary is not optimized for iPhone 5�� (ITMS-90096) when submitting 
��ǩ�� xcodeapp storeappios���� 
2015-11-01 15:49 7295���Ķ� ����(4) �ղ� �ٱ� 
��Ȩ����������Ϊ����ԭ�����£�δ������������ת�ء�
��Xcode7��App Store�ύ�������ļ��ǣ���ʾERROR ITMS-90096��"You binary is not?optimized for iPhone 5....".
�ڹ������Images.xcassets��Ӳ�����LaunchImage�Խ��ERROR ITMS-90096�����������κ����ã���Ҫ����������iPhone 5������ͼƬ���ؼ��������1��ͼƬ�����Ʊ����Default-568.png��2�������ͼƬ���ڹ��̵ĸ�Ŀ¼�¡�3��ͼƬ�ߴ������320*568��
Ȼ����plist�����
<key>UILaunchImages</key>
<array>
    <dict>
        <key>UILaunchImageName</key>
        <string>Default-568</string>
        <key>UILaunchImageSize</key>
        <string>{320, 568}</string>
    </dict>
</array>
��ͼ��

���������ϴ��������ļ��ˡ�