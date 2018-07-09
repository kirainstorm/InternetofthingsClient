
》》》》》》》》强制开启屏幕旋转效果

如果用户的手机没有开启重力感应器或者在AndroidManifest.xml中设置了android:screenOrientation，默认情况下，该Activity不会响应屏幕旋转事件。如果在这种情况下，依然希望Activity能响应屏幕旋转，则添加如下代码：

// activity的 onCreate 函数中 

this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR);


》》》》》》》》屏幕旋转时，不希望activity被销毁



如果希望捕获屏幕旋转事件，并且不希望activity 被销毁，方法如下：

（1）在AndroidManifest.xml对应的activity属性中，添加：

android:configChanges="keyboardHidden|orientation|screenSize"

（2）在对应的activity中，重载函数onConfigurationChanged

@Override
public voidonConfigurationChanged(Configuration newConfig) { 
    super.onConfigurationChanged(newConfig); 
}


这种方法有一个Bug，它只能一次旋转90度，如果你突然一下子旋转180度，onConfigurationChanged函数不会被调用。



当android:targetSdkVersion大于等于13时， 要想onConfigurationChanged()方法被调用，mainfest中加上
            android:screenOrientation="sensor" >

