//引入通知
var WxNotificationCenter = require('../../utils/WxNotificationCenter.js')
// var devjsondata;
// var devlist = [];
// var grouplist = [];
// var groupid;

var _lists_refresh_timer = null;

var deviceslist = [];

const appjs = require('../../app.js')
const cacheJs = require('../../utils/cache.js')
const websocketJs = require('../../utils/websocket.js')
const jsonbuilderJs = require('../../utils/jsonbuilder.js')

Page({
  /**
   * 页面的初始数据
   */
  data: {

    // deviceslist: [],
    // group: [],
    onlineUrl: '../image/online.png',
    offlineUrl: '../image/offline.png',
    hasUserinfo: false,

  },

  //设备信息点击事件
  clickinfobtn: function (e) {
    // wx.showModal({
    //   title: '提示',
    //   content: '点击了设备信息',
    //   success: function (res) {
    //     if (res.confirm) {
    //       console.log('用户点击确定',e.id)
    //     } else {
    //       console.log('用户点击取消')
    //     }

    //   }
    // })
  },


  //选择组
  selectGroupAction: function (e) {
    //分组id

    groupid = e.currentTarget.id;
    // var that=this;
    //  getUserDevices(that)

    wx.navigateTo({
      url: '../groupList/groupList?groupid=' + groupid,
    })
  },

  //点击行事件
  clickrow: function (e) {
    // var info;
    // var hid;
    // var ipurl;
    // var status;
    // var array;


    var info = e.currentTarget.id;
    cacheJs.Fun_Cache_setPlayInfo(info);
    console.log(info);
    var array = info.split(";")
    var _uuid = array[0];
    var _status = array[1];
    var _rtmpip = array[2];
    var _rtmpuuid = array[3];


    // status = array[1];
    // //hid = array[0];
    // //ipurl = array[0].split("//")[1].split(":")[0]
    // getApp().globalData.ipurl = ipurl
    // getApp().globalData.hid = hid
    // console.log("info=" + info)
    // console.log("status=" + status)
    // console.log("hid =" + hid)
    // console.log("ipurl =" + ipurl)


    if (_status == 1) {
      wx.navigateTo({
        url: '../player/player',
      })
    } else {
      wx.showModal({
        title: '提示',
        content: '当前设备不在线',
      })
    }


    // wx.navigateTo({
    //   url: '../player/player',
    // })
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    console.log(options)
    wx.setNavigationBarTitle({ title: '列表' })

    // if (options.groupid){
    // wx.setNavigationBarTitle({ title: '4GCMS' })
    // groupid = options.id;
    // var that = this;
    // getUserDevices(that)
    // }

    //注册通知
    var that = this;
    WxNotificationCenter.addNotification('getdeviceResult', that.didGetdeviceNotification, that);


  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {
    console.log('1')
  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {
    Fun_Lists_Show_Devices_List(this);



    // var list = yet.data.list;
    // list.splice(0, list.length);//清空数组 
    // devlist.splice(0, devlist.length);//清空数组
    // if (devjsondata.device_info != null) {

    //   for (var i = 0; i < devjsondata.device_info.length; i++) {

    //     list.push(devjsondata.device_info[i]);
    //     devlist.push(devjsondata.device_info[i]);

    //   }

    //   yet.setData({
    //     list: list
    //   })

    // }


    // console.log('name=' + getApp().globalData.username)
    // //如果没有登录过 就先登录
    // if (getApp().globalData.username === '' || getApp().globalData.username === null) {
    //   wx.navigateTo({
    //     url: '../loginPage/login',
    //   })
    //   return;
    // }
  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {
    //移除通知
    var that = this
    WxNotificationCenter.removeNotification('getdeviceResult', that);
    //
    if (null != _lists_refresh_timer)
    {
      clearTimeout(_lists_refresh_timer);
      _lists_refresh_timer = null;
    }

  },
  //通知处理
  didGetdeviceNotification: function (info) {

    if (null != _lists_refresh_timer) {
      clearTimeout(_lists_refresh_timer);
      _lists_refresh_timer = null;
    }
    wx.hideLoading();
    Fun_Lists_Show_Devices_List(this);

    // console.log('info=' + info);
    // //请求列表
    // var that = this;

    // //默认分组为0
    // groupid = 0;
    // getUserDevices(that)

    // wx.hideNavigationBarLoading()

  },



  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {
    console.log('正在下拉刷新');
    //
     wx.stopPullDownRefresh();
     //
     websocketJs.Fun_Websocket_sendMessage(jsonbuilderJs.Fun_createJsonGetDevicesList());
     //
     setTimeout(Fun_Lists_didRefreshTimeout, 3000);
// //
//     appjs.
//     //
    wx.showLoading({
      title: '正在刷新',
      mask: true,
    });



    


    //请求列表
    // groupid = 0;
    // var that = this;
    // getUserDevices(that)

    //执行网络请求工作，请求到数据后 wx.stopPullDownRefresh()
    // setTimeout(function () {
    //   console.log('延迟执行')
    //   wx.stopPullDownRefresh()
    // }, 2000)
  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  }
})
function Fun_Lists_didRefreshTimeout()
{
  wx.hideLoading();
  _lists_refresh_timer = null;
}


function Fun_Lists_Show_Devices_List(_this) {
  //
  deviceslist.splice(0, deviceslist.length);
  _this.setData({
    list: deviceslist
  })
  //
  var data = cacheJs.Fun_Cache_getDevicesList();
  if (null == data)
    return;

  //console.log("cameralist- " + data);

  var obj = JSON.parse(data);
  //alert(obj.info.length);
  //alert(obj.info[0].name);
  for (var i = 0; i < obj.info.length; i++) {
    deviceslist.push(obj.info[i]);

  }
  _this.setData({
    list: deviceslist
  })
}





function playvideo(_hid, _ipurl) {
  var _src = "/home/play?username=" + getUrlParam('username') + "&access_token=" + getUrlParam('access_token') + "&state=" + getUrlParam('state') + "&hid=" + _hid + "&ipurl=" + _ipurl;

  window.open(_src);
}
function parseMediaIPFromUrl(sspurl) {
  var ipstr = null;
  var strarray = new Array();
  strarray = sspurl.split(';');
  for (i = 0; i < strarray.length; i++) {
    if (strarray[i].indexOf("ssp://") != -1) {
      var iEnd = strarray[i].lastIndexOf(":");
      ipstr = strarray[i].substring(6, iEnd);
      break;
    }
  }

  return ipstr;
}



