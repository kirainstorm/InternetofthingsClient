https://github.com/videojs/video.js/releases


https://github.com/videojs/videojs-contrib-hls/releases


注意路径问题

video.js 屏蔽鼠标左键点击时暂停或者播放

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