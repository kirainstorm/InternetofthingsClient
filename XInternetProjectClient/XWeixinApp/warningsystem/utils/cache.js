//
//
var _loginuser_cache, _loginpwd_cache;
//
function Fun_Cache_setUserNamePwd(user, pwd) {
  try {
    wx.setStorageSync('loginuser', user);
    wx.setStorageSync('loginpwd', pwd);
    _loginuser_cache = user;
    _loginpwd_cache = pwd;
    //console.log('Fun_Cache_setUserNamePwd-' + _loginuser_cache);
    //console.log('Fun_Cache_setUserNamePwd-' + _loginpwd_cache);
  } catch (e) {
    // Do something when catch error
  }
}
function Fun_Cache_getUserNamePwd() {
  try {
    var value = wx.getStorageSync('loginuser')
    if (value) {
      _loginuser_cache = value;
    } else {
      _loginuser_cache = "";
    }
    //
    value = wx.getStorageSync('loginpwd')
    if (value) {
      _loginpwd_cache = value;
    } else {
      _loginpwd_cache = "";
    }
    //console.log('Fun_Cache_getUserNamePwd-' + _loginuser_cache);
    //console.log('Fun_Cache_getUserNamePwd' + _loginpwd_cache);
  } catch (e) {
    // Do something when catch error
  }
}
function Fun_Cache_removeUserNamePwd() {
  try {
    //wx.removeStorageSync('loginuser');
    wx.removeStorageSync('loginpwd');
    _loginpwd_cache = "";
  } catch (e) {
    // Do something when catch error
  }
}
function Fun_Cache_getUserName() {
  Fun_Cache_getUserNamePwd();
  return _loginuser_cache;
}
function Fun_Cache_getUserPwd() {
  Fun_Cache_getUserNamePwd();
  return _loginpwd_cache;
}
//-----------------------------------------------------
function Fun_Cache_setDevicesList(data) {
  try {
    wx.setStorageSync('deviceslist', data);
  } catch (e) {
    // Do something when catch error
  }
}
function Fun_Cache_getDevicesList() {
  var value = null;
  try {
    value = wx.getStorageSync('deviceslist')
  } catch (e) {
    // Do something when catch error
  }
  return value;
}
//-----------------------------------------------------
function Fun_Cache_setPlayInfo(data) {
  try {
    wx.setStorageSync('playinfo', data);
  } catch (e) {
    // Do something when catch error
  }
}
function Fun_Cache_getPlayInfo() {
  var value = null;
  try {
    value = wx.getStorageSync('playinfo')
  } catch (e) {
    // Do something when catch error
  }
  return value;
}



//
//
module.exports.Fun_Cache_setUserNamePwd = Fun_Cache_setUserNamePwd;
module.exports.Fun_Cache_getUserNamePwd = Fun_Cache_getUserNamePwd;
module.exports.Fun_Cache_removeUserNamePwd = Fun_Cache_removeUserNamePwd;
module.exports.Fun_Cache_getUserName = Fun_Cache_getUserName;
module.exports.Fun_Cache_getUserPwd = Fun_Cache_getUserPwd;
//
module.exports.Fun_Cache_setDevicesList = Fun_Cache_setDevicesList;
module.exports.Fun_Cache_getDevicesList = Fun_Cache_getDevicesList;
//
module.exports.Fun_Cache_setPlayInfo = Fun_Cache_setPlayInfo;
module.exports.Fun_Cache_getPlayInfo = Fun_Cache_getPlayInfo;
