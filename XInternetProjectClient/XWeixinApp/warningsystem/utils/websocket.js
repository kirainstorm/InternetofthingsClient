//
const cacheJs = require('./cache.js')
const jsonbuilderJs = require('./jsonbuilder.js')
var WxNotificationCenter = require('./WxNotificationCenter.js')
//
var _wx_socketTask = null;
var _wx_socketOpen = false;//
var _wx_socketHearbeat_timer = null;
var _wx_user_login_status = false;
//
function Fun_Websocket_connectSever() {
  // wx.connectSocket({
  //   url: 'wss://wapp.hddvs.com:6502'
  // })
  // wx.onSocketOpen(function (res) {
  //   console.log('WebSocket连接已打开！')
  // })
  // wx.onSocketError(function (res) {
  //   console.log('WebSocket连接打开失败，请检查！')
  // })

  Fun_Websocket_disconnectSever();

  wx.showLoading({
    title: '正在连接服务器',
    mask: true,
  });

  console.log("Fun_Websocket_connectSever - _wx_socketOpen :" + _wx_socketOpen);
  console.log("Fun_Websocket_connectSever - _wx_socketTask :" + _wx_socketTask);
  console.log("Fun_Websocket_connectSever - _wx_user_login_status :" + _wx_user_login_status);

  if ((false == _wx_socketOpen) && (null == _wx_socketTask)) {

    _wx_socketTask = wx.connectSocket({
      url: 'wss://wapp.hddvs.com:6502'
    });

    wx.onSocketOpen(function (res) {
      _wx_socketOpen = true;
      //
      var s = jsonbuilderJs.Fun_createJsonLoginInfo(cacheJs.Fun_Cache_getUserName(), cacheJs.Fun_Cache_getUserPwd());
      console.log(s);
      Fun_Websocket_sendMessage(s);
      //
      console.log('WebSocket - onSocketOpen');
    });
    wx.onSocketError(function (res) {
      _wx_socketOpen = false;
      _wx_user_login_status = false;
      console.log('WebSocket - onSocketError');
      // if (_wx_app_is_hide) {
      //   ;//用户关闭了小程序
      // }
      // else {
      //   ;//需要重连
      // }
    });
    wx.onSocketMessage(function (res) {
      console.log('收到服务器内容：' + res.data);
      // if (_wx_app_is_hide) {
      //   ;//用户关闭了小程序
      // }
      // else {
      //   Fun_WS_onMessage(res.data);
      // }
      Fun_Websocket_onMessage(res.data);
    });
    wx.onSocketClose(function (res) {
      if (false == _wx_user_login_status) {
        wx.showToast({
          title: "错误：请检查网络或者用户名密码！",
          icon: "none",
          mask: true,
          duration: 2000
        });
      }

      _wx_user_login_status = false;
      console.log('WebSocket - onSocketClose');
      // if (_wx_app_is_hide) {
      //   ;//用户关闭了小程序
      // }
      // else {
      //   ;//需要重连
      // }
    });

  }
};
function Fun_Websocket_disconnectSever() {
  if ((true == _wx_socketOpen) && (null != _wx_socketTask)) {
    _wx_socketTask.close();
    _wx_socketOpen = false;
    _wx_socketTask = null;
  }
  if (null != _wx_socketHearbeat_timer) {
    clearInterval(_wx_socketHearbeat_timer);
    _wx_socketHearbeat_timer = null;
  }

  //_wx_user_login_status = false;
};
function Fun_Websocket_onMessage(data) {

  var obj = JSON.parse(data);
  var cmd = obj.header.command;
  if ("login" == cmd) {
    //alert(cmd);
    _wx_user_login_status = true; //收到login回复才算真正登陆成功
    if (null == _wx_socketHearbeat_timer) {
      _wx_socketHearbeat_timer = setInterval(Fun_Websocket_HeartbeatInterval, 5000);
    }
    wx.hideLoading();
    Fun_Websocket_sendMessage(jsonbuilderJs.Fun_createJsonGetDevicesList());
    //
    setTimeout(Fun_switchTab, 1000);
  }
  //
  if (false == _wx_user_login_status) {
    return;
  }
  //
  if ("heartbeat" == cmd) {
    return;
  }
  //
  if ("getdevices" == cmd) {
    wx.hideLoading();
    cacheJs.Fun_Cache_setDevicesList(data);
    //
    WxNotificationCenter.postNotificationName('getdeviceResult',null);
  }


  //
  // if (("getparams" == cmd) && (obj.header.msguuid == msguuid_getparams)) {
  //   if (0 == obj.header.result)
  //     Fun_GetParam_callback(evt.data);
  // }
  // if (("setparam" == cmd) && (obj.header.msguuid == msguuid_setparams)) {
  //   if (0 == obj.header.result)
  //     Fun_SetParam_callback_ok();
  //   else
  //     Fun_SetParam_callback_error()
  // }


  if (null == _wx_socketHearbeat_timer) {
    _wx_socketHearbeat_timer = setInterval(Fun_WS_HeartbeatInterval, 5000);
  }
  ;
}
function Fun_Websocket_sendMessage(msg) {
  if ((true == _wx_socketOpen) && (null != _wx_socketTask)) {
    wx.sendSocketMessage({
      data: msg
    });
    return true;
  }
  return false;
};
function Fun_Websocket_HeartbeatInterval() {
  Fun_Websocket_sendMessage(jsonbuilderJs.Fun_createJsonHeartbeatInfo());
}
function Fun_switchTab() {
  wx.switchTab({
    url: "../lists/lists"
  })
}
//
module.exports.Fun_Websocket_connectSever = Fun_Websocket_connectSever;
module.exports.Fun_Websocket_disconnectSever = Fun_Websocket_disconnectSever;
module.exports.Fun_Websocket_sendMessage = Fun_Websocket_sendMessage;