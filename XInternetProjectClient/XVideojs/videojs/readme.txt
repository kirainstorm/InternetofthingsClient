https://github.com/videojs/video.js/releases


https://github.com/videojs/videojs-contrib-hls/releases


ע��·������

video.js �������������ʱ��ͣ���߲���

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
        //this.play();//20170801����������
      } else {
        //this.pause();//20170801����������
      }
    }
  };