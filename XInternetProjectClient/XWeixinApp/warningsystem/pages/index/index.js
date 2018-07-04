//index.js
//获取应用实例
const app = getApp()

const appjs = require('../../app.js')
const jsonbuilderJs = require('../../utils/jsonbuilder.js')
const cacheJs = require('../../utils/cache.js')
const websocketJs = require('../../utils/websocket.js')
//
//var _index_username;
//var _index_password;

//
//onLoad - onShow - onReady
//点击右上角/被其他页面挡住 onHide
//页面被其他页面挡住后显示 onShow


Page({
  data: {
    

    motto: 'Hello World',
    userInfo: {},
    hasUserInfo: false,
    // canIUse: wx.canIUse('button.open-type.getUserInfo')


    _index_username: '',
    _index_password: '',
    login: '登录',
    userinfoimageUrl: '../image/login_logo.png',
    headimageUrl: '../image/login_user_image.png',
    passwordimageUrl: '../image/login_pass_image.png',
    loading: false,
  },
  //事件处理函数
  // bindViewTap: function() {
  //   wx.navigateTo({
  //     url: '../logs/logs'
  //   })
  // },
  onLoad: function () {
    console.log('onLoad');





    //wx.showToast({ title: "" + appjs._wx_username_cache ,duration : 2000});
    //wx.hideLoading()//

    // if (app.globalData.userInfo) {
    //   this.setData({
    //     userInfo: app.globalData.userInfo,
    //     hasUserInfo: true
    //   })
    // } else if (this.data.canIUse){
    //   // 由于 getUserInfo 是网络请求，可能会在 Page.onLoad 之后才返回
    //   // 所以此处加入 callback 以防止这种情况
    //   app.userInfoReadyCallback = res => {
    //     this.setData({
    //       userInfo: res.userInfo,
    //       hasUserInfo: true
    //     })
    //   }
    // } else {
    //   // 在没有 open-type=getUserInfo 版本的兼容处理
    //   wx.getUserInfo({
    //     success: res => {
    //       app.globalData.userInfo = res.userInfo
    //       this.setData({
    //         userInfo: res.userInfo,
    //         hasUserInfo: true
    //       })
    //     }
    //   })
    // }
  },
  // getUserInfo: function(e) {
  //   console.log(e)
  //   app.globalData.userInfo = e.detail.userInfo
  //   this.setData({
  //     userInfo: e.detail.userInfo,
  //     hasUserInfo: true
  //   })
  // }
  onUnload: function () {
    console.log('onUnload');
  },
  onHide: function () {
    console.log('onHide');
  },
  onReady: function () {
    console.log('onReady');
  },
  onShow: function () {
    console.log('onShow');
    this.data._index_username = cacheJs.Fun_Cache_getUserName();
    this.data._index_password = cacheJs.Fun_Cache_getUserPwd();

    this.setData({
      inputLoginUser: this.data._index_username
    });
    this.setData({
      inputLoginPwd: this.data._index_password
    });
  },
  userNameInput: function (e) {
    this.data._index_username = e.detail.value
  },
  passWordInput: function (e) {
    this.data._index_password = e.detail.value
  },
  clickloginbtn: function () {
    //

    if ("" == this.data._index_username) {
      //console.log('请输入用户名')
      wx.showModal({
        title: '提示',
        content: '请输入用户名',
        showCancel: false,
        success: function (res) {
          if (res.confirm) {
            console.log('用户点击确定')
          } else {
            console.log('用户点击取消')
          }
        }
      })
      return
    }
    //
    if ("" == this.data._index_password) {
      //console.log('请输入密码')
      wx.showModal({
        title: '提示',
        content: '请输入密码',
        showCancel:false,
        success: function (res) {
          if (res.confirm) {
            console.log('用户点击确定')
          } else {
            console.log('用户点击取消')
          }

        }
      })
      return
    }


    cacheJs.Fun_Cache_setUserNamePwd(this.data._index_username, this.data._index_password);
    websocketJs.Fun_Websocket_connectSever();

    //
  }
})
