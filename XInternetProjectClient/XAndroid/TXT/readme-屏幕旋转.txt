
����������������ǿ�ƿ�����Ļ��תЧ��

����û����ֻ�û�п���������Ӧ��������AndroidManifest.xml��������android:screenOrientation��Ĭ������£���Activity������Ӧ��Ļ��ת�¼����������������£���Ȼϣ��Activity����Ӧ��Ļ��ת����������´��룺

// activity�� onCreate ������ 

this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR);


������������������Ļ��תʱ����ϣ��activity������



���ϣ��������Ļ��ת�¼������Ҳ�ϣ��activity �����٣��������£�

��1����AndroidManifest.xml��Ӧ��activity�����У���ӣ�

android:configChanges="keyboardHidden|orientation|screenSize"

��2���ڶ�Ӧ��activity�У����غ���onConfigurationChanged

@Override
public voidonConfigurationChanged(Configuration newConfig) { 
    super.onConfigurationChanged(newConfig); 
}


���ַ�����һ��Bug����ֻ��һ����ת90�ȣ������ͻȻһ������ת180�ȣ�onConfigurationChanged�������ᱻ���á�



��android:targetSdkVersion���ڵ���13ʱ�� Ҫ��onConfigurationChanged()���������ã�mainfest�м���
            android:screenOrientation="sensor" >

