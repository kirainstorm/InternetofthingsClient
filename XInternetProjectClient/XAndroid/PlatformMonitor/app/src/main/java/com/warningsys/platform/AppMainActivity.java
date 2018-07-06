package com.warningsys.platform;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Toast;

//import com.lzy.widget.AlphaIndicator;

import java.util.ArrayList;
import java.util.List;

public class AppMainActivity extends AppCompatActivity {



    private com.yinglan.alphatabs.AlphaTabsIndicator alphaTabsIndicator;
    public FragmentDevice FFragmDevice= new FragmentDevice();
    public FragmentLive FFragmLive= new FragmentLive();
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_aapp_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        toolbar.setTitle(getResources().getString(R.string.app_name));//标题
        setSupportActionBar(toolbar);


        SysApp.Ins().MainAct = this;
        SysApp.Ins().SetContext(AppMainActivity.this);

        ViewPager viewPager = (ViewPager) findViewById(R.id.viewPager);
        viewPager.setAdapter(new MainAdapter(getSupportFragmentManager()));
        alphaTabsIndicator = (com.yinglan.alphatabs.AlphaTabsIndicator) findViewById(R.id.alphaIndicator);
        alphaTabsIndicator.setViewPager(viewPager);
        alphaTabsIndicator.setTabCurrenItem(0);

//        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
//        fab.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
//                        .setAction("Action", null).show();
//            }
//        });







    }

    private class MainAdapter extends FragmentPagerAdapter implements ViewPager.OnPageChangeListener {

        private List<Fragment> fragments = new ArrayList<>();

        public MainAdapter(FragmentManager fm) {

            super(fm);

            if(fragments.isEmpty())
            {
                fragments.add(FFragmDevice);
                fragments.add(new FragmentHtml5());
                //fragments.add(FFragmLive);
                fragments.add(new FragmentAbout());
            }
        }



        @Override

        public Fragment getItem(int position) {

            return fragments.get(position);

        }



        @Override

        public int getCount() {

            return fragments.size();

        }



        @Override

        public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {



        }



        @Override

        public void onPageSelected(int position) {

            if (0 == position) {

                //alphaTabsIndicator.getTabView(0).showNumber(alphaTabsIndicator.getTabView(0).getBadgeNumber() - 1);

            } else if (1 == position) {

               // alphaTabsIndicator.getCurrentItemView().removeShow();

            } else if (2 == position) {

                //alphaTabsIndicator.removeAllBadge();

            }

        }



        @Override

        public void onPageScrollStateChanged(int state) {



        }

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        //getMenuInflater().inflate(R.menu.menu_main_device, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

//        if (item.getItemId() == R.id.platform_id_menu_live) {
//            Intent ints = new Intent();
//            ints.setClass(AppMainActivity.this, MainLiveActivity.class);
//            startActivity(ints);
//            return true;
//        }
//        // 资讯------------------------------------------------------------
//        if (item.getItemId() == R.id.platform_id_menu_community) {
//
//            Intent ints = new Intent();
//            ints.setClass(AppMainActivity.this, CommunityActivity.class);
//            startActivity(ints);
//            return true;
//        }
        // 关于我们------------------------------------------------------------
        if (item.getItemId() == R.id.platform_id_menu_about) {

            Intent ints = new Intent();
            ints.setClass(AppMainActivity.this, AboutActivity.class);
            startActivity(ints);
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    private long mExitTime = 0; // 退出时间

    @Override
    public void onBackPressed() {
        Intent setIntent = new Intent(Intent.ACTION_MAIN);
        setIntent.addCategory(Intent.CATEGORY_HOME);
        setIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        startActivity(setIntent);
    }

    @Override
    public void onDestroy() {
        // TODO Auto-generated method stub
        SysApp.Ins().onDestroyNative();
        android.os.Process.killProcess(android.os.Process.myPid());// Android程序退出彻底关闭进程的方法
        super.onDestroy();
    }

    @Override
    public void onPause() {
        // TODO Auto-generated method stub
        super.onPause();
    }

    @Override
    public void onResume() {
        // TODO Auto-generated method stub
        super.onResume();
    }

    @Override
    public void onStart() {
        // TODO Auto-generated method stub
        super.onStart();
    }

    @Override
    public void onStop() {
        // TODO Auto-generated method stub
        super.onStop();
    }


}
