1：indexpage.html

首页的效果，只放视频，没有全屏


2：index.html

进入某个房间的效果，只有全屏





3：修改video.js 屏蔽鼠标左键点击时暂停或者播放

  Player.prototype.handleTechClick_ = function handleTechClick_(event) {
    // We're using mousedown to detect clicks thanks to Flash, but mousedown
    // will also be triggered with right-clicks, so we need to prevent that
    if (event.button !== 0) {
      return;
    }

    // When controls are disabled a click should not toggle playback because
    // the click is considered a control
    if (this.controls()) {
      if (this.paused()) {
        //this.play();//20170801，屏蔽这里
      } else {
        //this.pause();//20170801，屏蔽这里
      }
    }
  };