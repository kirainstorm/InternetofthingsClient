package com.sricamlive.platform;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;

import com.nostra13.universalimageloader.core.ImageLoader;

import uk.co.senab.photoview.PhotoView;
import uk.co.senab.photoview.PhotoViewAttacher;

public class AlbumViewActivity extends AppCompatActivity {
    Toolbar toolbar;
    private PhotoView img;
    private PhotoViewAttacher attacher;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_album_view);

        //
        String currentFileName=null;
        if(getIntent().getExtras()!=null)
            currentFileName=getIntent().getExtras().getString("file");


        //
//        toolbar = (Toolbar) findViewById(R.id.toolbar);
//        toolbar.setTitle(getResources().getString(R.string.app_look_pic));//标题
//        File tempFile =new File(currentFileName);
//        toolbar.setSubtitle(tempFile.getName());//副标题
//        setSupportActionBar(toolbar);
//        toolbar.setNavigationIcon(R.drawable.ic_action_back);//设置Navigation 图标
//        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//                finish();
//            }
//        });


        img = (PhotoView) findViewById(R.id.imageView);
        ImageLoader.getInstance().displayImage("file:///"+ currentFileName, img);
        attacher = new PhotoViewAttacher(img);
        attacher.update();
        attacher.setScaleType(ImageView.ScaleType.FIT_CENTER);

//        int a = getWindow().getAttributes().width;
//        int b = getWindow().getAttributes().height;
//        float deltaX = 0, deltaY = 0;
//
//
//        Matrix matrix = new Matrix();
//        matrix.postTranslate(120, 120);
//        img.setImageMatrix(matrix);
    }
//
//    private void center(boolean horizontal, boolean vertical)
//    {
//        Matrix m = new Matrix();
//        //m.set(matrix);
//        RectF rect = new RectF(0, 0, bitmap.getWidth(), bitmap.getHeight());
//        m.mapRect(rect);
//        float height = rect.height();
//        float width = rect.width();
//        float deltaX = 0, deltaY = 0;
//        if (vertical)
//        {
//            //int screenHeight = dm.heightPixels;  //手机屏幕分辨率的高度
//            int screenHeight = 400;
//            if (height < screenHeight)
//            {
//                deltaY = (screenHeight - height)/2 - rect.top;
//            }else if (rect.top > 0)
//            {
//                deltaY = -rect.top;
//            }else if (rect.bottom < screenHeight)
//            {
//                deltaY = view.getHeight() - rect.bottom;
//            }
//        }
//
//        if (horizontal)
//        {
//            //int screenWidth = dm.widthPixels;  //手机屏幕分辨率的宽度
//            int screenWidth = 400;
//            if (width < screenWidth)
//            {
//                deltaX = (screenWidth - width)/2 - rect.left;
//            }else if (rect.left > 0)
//            {
//                deltaX = -rect.left;
//            }else if (rect.right < screenWidth)
//            {
//                deltaX = screenWidth - rect.right;
//            }
//        }
//        matrix.postTranslate(deltaX, deltaY);
//    }

}
