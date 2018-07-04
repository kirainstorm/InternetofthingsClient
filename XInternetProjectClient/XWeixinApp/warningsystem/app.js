//app.js
//
const jsonbuilderJs = require('./utils/jsonbuilder.js')
const cacheJs = require('./utils/cache.js')
const websocketJs = require('./utils/websocket.js')
var WxNotificationCenter = require('./utils/WxNotificationCenter.js')


//当前小程序是否被关掉了
var _wx_app_is_hide = false;
//网络状态
var _wx_NetworkIsConnected;
var _wx_NetworkType;// wifi/2g/3g/4g/unknown(Android下不常见的网络类型)/none(无网络)
//websocket

//用户名密码缓存
var _wx_username_cache = "";
var _wx_userpwd_cache = "";
//用户登陆状态
// var _wx_user_login_status = false;


App({
  onLaunch: function () {
    console.log('App - onLaunch');
    




    // // 展示本地存储能力
    // var logs = wx.getStorageSync('logs') || []
    // logs.unshift(Date.now())
    // wx.setStorageSync('logs', logs)

    // 登录
    wx.login({
      success: res => {
        // 发送 res.code 到后台换取 openId, sessionKey, unionId
      }
    })
    // 获取用户信息
    wx.getSetting({
      success: res => {
        if (res.authSetting['scope.userInfo']) {
          // 已经授权，可以直接调用 getUserInfo 获取头像昵称，不会弹框
          wx.getUserInfo({
            success: res => {
              // 可以将 res 发送给后台解码出 unionId
              this.globalData.userInfo = res.userInfo

              // 由于 getUserInfo 是网络请求，可能会在 Page.onLoad 之后才返回
              // 所以此处加入 callback 以防止这种情况
              if (this.userInfoReadyCallback) {
                this.userInfoReadyCallback(res)
              }
            }
          })
        }
      }
    })
  },
  onShow: function () {
    console.log('App - onShow');
    //
    _wx_app_is_hide = false;
//
    _wx_username_cache = cacheJs.Fun_Cache_getUserName();
    _wx_userpwd_cache = cacheJs.Fun_Cache_getUserPwd();

    //Fun_getUserNamePwd();
    //监测网络
    // wx.getNetworkType({
    //   success: function (res) {
    //     console.log("getNetworkType" + res.networkType);
    //     _wx_NetworkIsConnected = res.isConnected;
    //     _wx_NetworkType = res.networkType
    //   }
    // });
    // wx.onNetworkStatusChange(function (res) {
    //   console.log("onNetworkStatusChange" + res.isConnected);
    //   console.log("onNetworkStatusChange" + res.networkType);
    //   _wx_NetworkIsConnected = res.isConnected;
    //   _wx_NetworkType = res.networkType;
    // });
    //
    // if (null == _wx_timer_check_connction) {
    //   console.log('App - onShow - setInterval(Fun_checkConnection)');
    //   _wx_timer_check_connction = setInterval(Fun_checkConnection);
    // }
    //
    if (("" != _wx_username_cache) && ("" != _wx_userpwd_cache))
      websocketJs.Fun_Websocket_connectSever();
  },
  onHide: function () {
    console.log('App - onHide');
    //
    _wx_app_is_hide = true;
    //
    // if (null != _wx_timer_check_connction) {
    //   console.log('App - onHide - clearInterval(Fun_checkConnection)');
    //   clearInterval(_wx_timer_check_connction);
    //   _wx_timer_check_connction = null;
    // };
    //
    websocketJs.Fun_Websocket_disconnectSever();
  },
  globalData: {
    userInfo: null
  }
})


// function Fun_checkConnection() {
//   if ((false == _wx_socketOpen) && (null == _wx_socketTask)) {
//     _wx_socketTask = wx.connectSocket({ url: 'ws://webapp.warningsys.com/' });
//   }
// }





//注销
function Fun_reLaunchLogin()
{
  cacheJs.Fun_Cache_removeUserNamePwd();
  websocketJs.Fun_Websocket_disconnectSever();
  wx.reLaunch({
    url: '../../pages/index/index'
  })
}

//




// module.exports.Fun_setUserNamePwd = Fun_setUserNamePwd;
// module.exports.Fun_getUserName = Fun_getUserName;
// module.exports.Fun_getUserPwd = Fun_getUserPwd;
// //
// module.exports.Fun_WS_connectSever = Fun_WS_connectSever;

// //
// module.exports.Fun_getDevicesList = Fun_getDevicesList;
// //
module.exports.Fun_reLaunchLogin = Fun_reLaunchLogin;