package com.sricamlive.platform;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.annotation.TargetApi;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.IBinder;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.text.TextUtils;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.platform.nativecaller.DeviceListItem;
import com.platform.nativecaller.NativeCaller;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.net.InetAddress;
import java.net.UnknownHostException;

public class LoginPlatActivity extends AppCompatActivity {
    private UserLoginTask mAuthTask = null;

    // Values for email and password at the time of the login attempt.

    // UI references.
    private EditText mEmailView;
    private EditText mPasswordView;
    private EditText mSvrIPView;
    private View mLoginFormView;
    //private View mLoginStatusView;
    //private TextView mLoginStatusMessageView;
    private Button mBtn;
    //声明进度条对话框
    ProgressDialog m_pDialog;


    public String read_name() {
        try {
            FileInputStream inStream = this.openFileInput("name.txt");
            byte[] buffer = new byte[1024];
            int hasRead = 0;
            StringBuilder sb = new StringBuilder();
            while ((hasRead = inStream.read(buffer)) != -1) {
                sb.append(new String(buffer, 0, hasRead));
            }
            inStream.close();
            return sb.toString();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "";
    }

    public void write_name(String msg) {
        if (msg == null) return;
        try {
            FileOutputStream fos = openFileOutput("name.txt", MODE_PRIVATE);
            fos.write(msg.getBytes());
            fos.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public String read_pwd() {
        try {
            FileInputStream inStream = this.openFileInput("pwd.txt");
            byte[] buffer = new byte[1024];
            int hasRead = 0;
            StringBuilder sb = new StringBuilder();
            while ((hasRead = inStream.read(buffer)) != -1) {
                sb.append(new String(buffer, 0, hasRead));
            }

            inStream.close();
            return sb.toString();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "";
    }

    public void write_pwd(String msg) {
        if (msg == null) return;
        try {
            FileOutputStream fos = openFileOutput("pwd.txt", MODE_PRIVATE);
            fos.write(msg.getBytes());
            fos.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public String read_ip() {
        try {
            FileInputStream inStream = this.openFileInput("ip.txt");
            byte[] buffer = new byte[1024];
            int hasRead = 0;
            StringBuilder sb = new StringBuilder();
            while ((hasRead = inStream.read(buffer)) != -1) {
                sb.append(new String(buffer, 0, hasRead));
            }

            inStream.close();
            return sb.toString();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "";
    }

    public void write_ip(String msg) {
        if (msg == null) return;
        try {
            FileOutputStream fos = openFileOutput("ip.txt", MODE_PRIVATE);
            fos.write(msg.getBytes());
            fos.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login_plat);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        toolbar.setTitle(getResources().getString(R.string.app_login));//标题
        setSupportActionBar(toolbar);



        //创建ProgressDialog对象
        m_pDialog = new ProgressDialog(this);
        // 设置进度条风格，风格为圆形，旋转的
        m_pDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
        // 设置ProgressDialog 提示信息
        m_pDialog.setMessage(getResources().getString(R.string.title_activity_plat_login_3));
        // 设置ProgressDialog 的进度条是否不明确
        m_pDialog.setIndeterminate(false);
        // 设置ProgressDialog 是否可以按退回按键取消
        m_pDialog.setCancelable(false);



        SysApp.Ins().onInitNative();

        mEmailView = (EditText) findViewById(R.id.platuser);
        mEmailView.setText(read_name());

        mPasswordView = (EditText) findViewById(R.id.platpwd);
        mPasswordView.setText(read_pwd());

        mSvrIPView = (EditText) findViewById(R.id.platip);
        mSvrIPView.setText(read_ip());

        mPasswordView
                .setOnEditorActionListener(new TextView.OnEditorActionListener() {

                    @Override
                    public boolean onEditorAction(TextView textView, int id,
                                                  KeyEvent keyEvent) {
                        // if (id == R.id.login || id == EditorInfo.IME_NULL) {
                        if (id == EditorInfo.IME_NULL) {
                            attemptLogin();
                            return true;
                        }
                        return false;
                    }
                });

        mLoginFormView = findViewById(R.id.plat_login_form);
        //mLoginStatusView = findViewById(R.id.plat_login_status);
        //mLoginStatusMessageView = (TextView) findViewById(R.id.plat_login_status_message);

        mBtn = (Button) findViewById(R.id.sign_in_button);
        mBtn.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        attemptLogin();
                    }
                });
    }

    /**
     * Attempts to sign in or register the account specified by the login form.
     * If there are form errors (invalid email, missing fields, etc.), the
     * errors are presented and no actual login attempt is made.
     */
    public static boolean isNetworkConnected(Context context) {
        if (context != null) {
            ConnectivityManager mConnectivityManager = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
            NetworkInfo mNetworkInfo = mConnectivityManager.getActiveNetworkInfo();
            if (mNetworkInfo != null) {
                return true;//有网
            }
        }
        return false;//没有网
    }

    public static String GetInetAddress(String host) {
        String IPAddress = "";
        try {
            InetAddress ReturnStr1 = java.net.InetAddress.getByName(host);
            IPAddress = ReturnStr1.getHostAddress();
        } catch (UnknownHostException e) {
            e.printStackTrace();
            return IPAddress;
        }
        return IPAddress;
    }

    public void attemptLogin() {
        if (mAuthTask != null) {
            return;
        }

        //判断网络状况
        if (false == isNetworkConnected(this)) {
            ErrorAlertDialogShow(R.string.title_activity_plat_login_6);
            return;
        }


        InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.hideSoftInputFromWindow(mEmailView.getWindowToken(), 0);
        imm.hideSoftInputFromWindow(mPasswordView.getWindowToken(), 0);
        imm.hideSoftInputFromWindow(mSvrIPView.getWindowToken(), 0);

        //mEmailView.clearFocus();
        //mPasswordView.clearFocus();
        // Reset errors.
        mEmailView.setError(null);
        mPasswordView.setError(null);
        mSvrIPView.setError(null);

        // Store values at the time of the login attempt.
        SysApp.Ins().mUser = mEmailView.getText().toString();
        SysApp.Ins().mPassword = mPasswordView.getText().toString();
        SysApp.Ins().mIp = mSvrIPView.getText().toString();

        boolean cancel = false;
        View focusView = null;

        // Check for a valid password.
        if (TextUtils.isEmpty(SysApp.Ins().mPassword)) {
            mPasswordView
                    .setError(getString(R.string.title_activity_plat_login_1));
            focusView = mPasswordView;
            cancel = true;
        }
        // else if (mPassword.length() < 4) {
        // mPasswordView.setError(getString(R.string.error_invalid_password));
        // focusView = mPasswordView;
        // cancel = true;
        // }

        // Check for a valid email address.
        if (TextUtils.isEmpty(SysApp.Ins().mUser)) {
            mEmailView
                    .setError(getString(R.string.title_activity_plat_login_2));
            focusView = mEmailView;
            cancel = true;
        }

        if (TextUtils.isEmpty(SysApp.Ins().mIp)) {
            mSvrIPView
                    .setError(getString(R.string.title_activity_plat_login_7));
            focusView = mSvrIPView;
            cancel = true;
        }

        write_ip(SysApp.Ins().mIp);

        // else if (!mEmail.contains("@")) {
        // mEmailView.setError(getString(R.string.error_invalid_email));
        // focusView = mEmailView;
        // cancel = true;
        // }

        if (cancel) {
            // There was an error; don't attempt login and focus the first
            // form field with an error.
            focusView.requestFocus();
        } else {
            // Show a progress spinner, and kick off a background task to
            // perform the user login attempt.
//            mLoginStatusMessageView
//                    .setText(R.string.title_activity_plat_login_3);
            showProgress(true);
            mAuthTask = new UserLoginTask();
            mAuthTask.execute((Void) null);
        }
    }

    public void ErrorAlertDialogShow(int messageId) {
        Toast.makeText(this, messageId, Toast.LENGTH_SHORT).show();
    }

    /**
     * Shows the progress UI and hides the login form.
     */
    @TargetApi(Build.VERSION_CODES.HONEYCOMB_MR2)
    private void showProgress(final boolean show) {
        // On Honeycomb MR2 we have the ViewPropertyAnimator APIs, which allow
        // for very easy animations. If available, use these APIs to fade-in
        // the progress spinner.
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB_MR2) {
            int shortAnimTime = getResources().getInteger(
                    android.R.integer.config_shortAnimTime);

//            mLoginStatusView.setVisibility(View.VISIBLE);
//            mLoginStatusView.animate().setDuration(shortAnimTime)
//                    .alpha(show ? 1 : 0)
//                    .setListener(new AnimatorListenerAdapter() {
//                        @Override
//                        public void onAnimationEnd(Animator animation) {
//                            mLoginStatusView.setVisibility(show ? View.VISIBLE
//                                    : View.GONE);
//                        }
//                    });

            mLoginFormView.setVisibility(View.VISIBLE);
            mLoginFormView.animate().setDuration(shortAnimTime)
                    .alpha(show ? 0 : 1)
                    .setListener(new AnimatorListenerAdapter() {
                        @Override
                        public void onAnimationEnd(Animator animation) {
                            mLoginFormView.setVisibility(show ? View.GONE
                                    : View.VISIBLE);
                        }
                    });
        } else {
            // The ViewPropertyAnimator APIs are not available, so simply show
            // and hide the relevant UI components.
            //mLoginStatusView.setVisibility(show ? View.VISIBLE : View.GONE);
            mLoginFormView.setVisibility(show ? View.GONE : View.VISIBLE);
        }

        if (show)
            m_pDialog.show();
        else
            m_pDialog.hide();
    }

    /**
     * Represents an asynchronous login/registration task used to authenticate
     * the user.
     */
    public class UserLoginTask extends AsyncTask<Void, Void, Boolean> {
        @Override
        protected Boolean doInBackground(Void... params) {
            // TODO: attempt authentication against a network service.
            // ------------------------------------------------------
            //放在AsyncTask中，，主线程不能访问网络
            SysApp.Ins().mIp = GetInetAddress("xplat.srikam.com");
            if ("" == SysApp.Ins().mIp) {
                ErrorAlertDialogShow(R.string.title_activity_plat_login_6);
                return false;
            }

            DeviceListItem[] list = NativeCaller.GetDeviceList(SysApp.Ins().mIp,
                    SysApp.Ins().mPort, SysApp.Ins().mUser, SysApp.Ins().mPassword);
            if (list == null) {
                return false;
            }

            for (int i = 0; i < list.length; i++) {
                SysApp.Ins().InsertDeviceListItem(list[i]);
            }

            write_name(SysApp.Ins().mUser);
            write_pwd(SysApp.Ins().mPassword);
            //write_ip(SysApp.Ins().mIp);
            return true;
        }

        @Override
        protected void onPostExecute(final Boolean success) {
            mAuthTask = null;
            // showProgress(false);

            if (success) {

                ErrorAlertDialogShow(R.string.title_activity_plat_login_4);
                Intent intent = new Intent();
                intent.setClass(LoginPlatActivity.this,
                        AppMainActivity.class);
                startActivity(intent);
                overridePendingTransition(android.R.anim.fade_in,
                        android.R.anim.fade_out);
                LoginPlatActivity.this.finish();
            } else {
                ErrorAlertDialogShow(R.string.title_activity_plat_login_5);
                showProgress(false);
                // mPasswordView.setError(getString(R.string.error_incorrect_password));
                // mPasswordView.requestFocus();
            }
        }

        @Override
        protected void onCancelled() {
            mAuthTask = null;
            showProgress(false);
        }
    }


    // 获取点击事件
    @Override
    public boolean dispatchTouchEvent(MotionEvent ev) {
// TODO Auto-generated method stub
        if (ev.getAction() == MotionEvent.ACTION_DOWN) {
            View view = getCurrentFocus();
            //if (isHideInput(view, ev))
            {
                HideSoftInput(view.getWindowToken());
            }
        }
        return super.dispatchTouchEvent(ev);
    }

    // 判定是否需要隐藏
//    private boolean isHideInput(View v, MotionEvent ev) {
//        if (v != null && (v instanceof EditText)) {
//            int[] l = {0, 0};
//            v.getLocationInWindow(l);
//            intleft = l[0], top = l[1], bottom = top + v.getHeight(), right = left + v.getWidth();
//            if (ev.getX() > left && ev.getX() < right && ev.getY() > top && ev.getY() < bottom) {
//                returnfalse;
//            } else {
//                return true;
//            }
//        }
//        return false;
//    }

    // 隐藏软键盘
    private void HideSoftInput(IBinder token) {
        if (token != null) {
            InputMethodManager manager = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
            manager.hideSoftInputFromWindow(token,
                    InputMethodManager.HIDE_NOT_ALWAYS);
        }
    }


}
