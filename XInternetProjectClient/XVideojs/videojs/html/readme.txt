1��indexpage.html

��ҳ��Ч����ֻ����Ƶ��û��ȫ��


2��index.html

����ĳ�������Ч����ֻ��ȫ��





3���޸�video.js �������������ʱ��ͣ���߲���

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