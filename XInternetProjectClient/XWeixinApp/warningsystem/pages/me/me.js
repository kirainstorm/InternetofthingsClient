const appjs = require('../../app.js')

function loginOut() {
  appjs.Fun_reLaunchLogin();
}

Page({
  /**
   * 页面的初始数据
   */
  data: {

  },
  // 退出登录
  logout: function () {
    loginOut()
  },
  //
  onLoad: function (options) {
    wx.setNavigationBarTitle({ title: '我的' })
  },
  onReady: function () {
  },
  onShow: function () {
  },
  onHide: function () {
  },
  onUnload: function () {
  },
  onPullDownRefresh: function () {
    // console.log('正在下拉刷新')
    // //执行网络请求工作，请求到数据后 wx.stopPullDownRefresh()
    // setTimeout(function () {
    //   console.log('延迟执行')
    //   wx.stopPullDownRefresh();
    // }, 2000)
  },
  onReachBottom: function () {
  },
  onShareAppMessage: function () {
  }
})