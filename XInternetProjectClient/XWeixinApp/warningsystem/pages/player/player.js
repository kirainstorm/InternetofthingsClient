//
const appjs = require('../../app.js')
const cacheJs = require('../../utils/cache.js')
const websocketJs = require('../../utils/websocket.js')
const jsonbuilderJs = require('../../utils/jsonbuilder.js')
//
var Interval;
var timeOut;
var hiddentime;
var player;
var isfull;
//
Page({

  data: {
    text: '',
    control1Url: '../image/control1.jpg',
    flipvurl: '../image/flip_v.png',
    fliphurl: '../image/flip_h.png',
    alarmonurl: '../image/alarm_on.png',
    alarmoffurl: '../image/alarm_off.png',
    avqonurl: '../image/avq_mid.png',
    avqoffurl: '../image/avq_high.png',
    intercomonurl: '../image/player_intercom_on.png',
    intercomoffurl: '../image/player_intercom_off.png',
    videorecordurl: '../image/video_record_normal.png',
    videophotourl: '../image/video_photo_normal.png',
    miconurl: '../image/mic_on.png',
    micoffurl: '../image/mic_off.png',
    videotopurl: '../image/top.png',
    videolefturl: '../image/left.png',
    videorighturl: '../image/right.png',
    videoboomurl: '../image/boom.png',
    fullscreenurl: '../image/full.png',
    audiourl: '../mp3/click.wav',
    playurl: '',
    playingurl: '../image/play.png',
    pauseurl: '../image/pause.png',
    placeholderurl: '../image/placeholder.png',
    backurl: '../image/back.png',
    isalarmon: false,
    isavqon: false,
    isintercomon: false,
    ismicon: false,
    isComplete: false,
    autoplay: false,
    ishorizontal: false,
    playing: false,
    ishidden: true,
    top: '50%',
  },
  statechange(e) {
    console.log('live-player code:', e.detail.code);
    //wx.showToast({ title: "" + e.detail.code, duration: 2000 });
  },
  error(e) {
    console.error('live-player error:', e.detail.errMsg);
    wx.showToast({ title: "" + e.detail.errMsg, duration: 2000 });
  },
  onLoad: function (options) {
    var that = this
    isfull = false;
    hiddentime = 1;
  },
  onReady: function () {
    // 使用 wx.createAudioContext 获取 audio 上下文 context
  },
  onShow: function () {
    //
    wx.setNavigationBarTitle({ title: '实时视频' });
    //
    this.audioCtx = wx.createAudioContext('myaudio')
    player = wx.createLivePlayerContext('liveplayer');
    //
    var info = cacheJs.Fun_Cache_getPlayInfo(info);
    //console.log(info);
    var array = info.split(";")
    var _uuid = array[0];
    var _status = array[1];
    var _rtmpip = array[2];
    var _rtmpuuid = array[3];
    //
    var playurl = "rtmp://" + _rtmpip + "/live/" + _rtmpuuid;
    console.log(playurl);
    //微信小程序调用函数
    this.setData({
      playurl: playurl,
      autoplay: true,
      playing: true,
      ishidden: false
    })
  },
  onHide: function () {
  },
  onUnload: function () {
  },
  onPullDownRefresh: function () {
  },
  onPullDownRefresh: function () {
    console.log()
  },
  onReachBottom: function () {

  },
  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  },

  clickbtnAction: function (e) {
    // this.audioCtx.play()

    // console.log(e.currentTarget.id)

    // hiddentime = hiddentime + 1000;
    // if (e.currentTarget.id == 0) {
    //   sendCommand(16);
    //   this.setData({
    //     text: '翻转'
    //   })
    // } else if (e.currentTarget.id == 1) {

    //   this.setData({
    //     isalarmon: !this.data.isalarmon,
    //     text: this.data.isalarmon ? '报警关闭' : '报警开启'
    //   })
    // } else if (e.currentTarget.id == 2) {

    //   this.setData({
    //     isintercomon: !this.data.isintercomon,
    //     text: this.data.isintercomon ? '声音开启' : '声音关闭'
    //   })
    // } else if (e.currentTarget.id == 3) {

    //   this.setData({

    //     text: '开始录像'
    //   })
    // } else if (e.currentTarget.id == 4) {
    //   sendCommand(103);
    //   this.setData({
    //     isavqon: !this.data.isavqon,
    //     text: this.data.isavqon ? '画质：高清' : '画质：超清'
    //   })
    // } else if (e.currentTarget.id == 5) {
    //   sendCommand(15);
    //   this.setData({

    //     text: '镜像'
    //   })
    // } else if (e.currentTarget.id == 6) {

    //   this.setData({

    //     text: '拍照'
    //   })
    // } else if (e.currentTarget.id == 7) {

    //   this.setData({
    //     ismicon: !this.data.ismicon,
    //     text: this.data.ismicon ? '对讲关闭' : '对讲开启'
    //   })
    // } else if (e.currentTarget.id == 8) {
    //   sendCommand(3);
    //   this.setData({
    //     text: '云台向上'
    //   })
    // } else if (e.currentTarget.id == 9) {
    //   sendCommand(1);

    //   this.setData({
    //     text: '云台向左'
    //   })
    // } else if (e.currentTarget.id == 10) {
    //   sendCommand(2);
    //   this.setData({
    //     text: '云台向右'
    //   })
    // } else if (e.currentTarget.id == 11) {
    //   sendCommand(4);
    //   this.setData({
    //     text: '云台向下'
    //   })

    // }
    // //  console.log(this.data.text)
    // console.log('hiddentime=' + hiddentime + '毫秒')

    // if (hiddentime > 1000) {
    //   clearTimeout(timeOut)
    //   console.log('关闭定时器')
    // }

    // var that = this
    // timeOut = setTimeout(function () {

    //   console.log('延迟执行')
    //   hiddentime = 0;
    //   that.setData({
    //     text: '',
    //   })

    // }, 2000)



  },


  hiddenaction() {
    var that = this;
    if (isfull === false) {

      that.setData({
        ishidden: !that.data.ishidden
      })
    } else {

    }
  },

  play() {

    var that = this;
    that.setData({
      ishidden: true
    })
    if (this.data.playing === true) {
      player.pause()
      console.log("暂停");
    } else {
      player.resume()
      console.log("恢复播放");
    }
    that.setData({
      playing: !that.data.playing
    })
  },

  fullscren() {

    var that = this;
    if (isfull === false) {
      player.requestFullScreen({});
      console.log('点击全屏')
      that.setData({
        top: '65%'
      })
    } else {
      player.exitFullScreen({});
      console.log('退出全屏')
      that.setData({
        top: '50%',
        ishidden: true
      })
    }
    isfull = !isfull;

    that.setData({
      ishorizontal: !that.data.ishorizontal
    })
  },

})

