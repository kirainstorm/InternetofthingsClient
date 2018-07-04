package com.platform.util;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.GridView;


//com.warningsys.util.MyGridView
//
//ListView或者GridView嵌套在ScrollView中：
//嵌套使用时，会出现ListView/GridView有自己的滚动条，而ScrollView又有自己的滚动条。
//解决此问题的核心是：重写ListView/GridView的OnMesure方法。
//
//xml布局文件加入（假设MyGridView所在的包为com.example.scrollviewandgridview）：
//<com.example.scrollviewandgridview.MyGridView  
//      android:id="@+id/mygridview"  
//      android:layout_width="match_parent"  
//      android:layout_height="wrap_content"  
//      android:numColumns="2" >  
//</com.example.scrollviewandgridview.MyGridView>  

public class MyGridView extends GridView {

	public MyGridView(Context context) {
		super(context);
	}

	public MyGridView(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	public MyGridView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
	}

	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		int expandSpec = MeasureSpec.makeMeasureSpec( 
				Integer.MAX_VALUE >> 2, MeasureSpec.AT_MOST); 
		super.onMeasure(widthMeasureSpec, expandSpec); 
	}
}