//var apiurl = "http://localhost:8089/api";
var apiurl = "http://live.sricam.com/api";
var HOME_CURR_STATUS = "home";
var HOME_CURR_TYPE =null;
var HOME_CURR_DATAID = null;

var CURR_LIVE_ID = null;

var CURR_LivePlayer = null;

var HOME_INIT =  false;

var refresh_status = false;

var CURR_search_kw = null;
function imgError($this){
    return $this.src='imgs/notfund.gif';
}

function strlen(str){
    var len = 0;
    for (var i=0; i<str.length; i++) {
        var c = str.charCodeAt(i);
        //单字节加1
        if ((c >= 0x0001 && c <= 0x007e) || (0xff60<=c && c<=0xff9f)) {
            len++;
        }
        else {
            len+=2;
        }
    }
    return len;
}

function GetQueryString(name)
{
     var reg = new RegExp("(^|&)"+ name +"=([^&]*)(&|$)");
     var r = window.location.search.substr(1).match(reg);
     if(r!=null)return  unescape(r[2]); return null;
}

function loadLiveView(roomid){
    $.ajax({
        type:"get",
        url:apiurl+"/view?roomid="+roomid,
        dataType:"jsonp",
        jsonpCallback:"sri_liveview",
        success:function(resp) {
            var resultdata =resp.data;
            var liveRoom = resultdata.liveRoom;
            var liveRoomInfo = resultdata.liveRoomInfo;
            if(liveRoom != null){
                $("#page-view").find(".bar-nav .title").text(liveRoom.roomName);
                $("#view_room_no").text(liveRoom.roomNo);
                $("#view_room_intro").text(liveRoom.roomIntro);
                if(liveRoomInfo){
                    $("#view_room_view_num").text(liveRoomInfo.viewNum);
                }
                if(!CURR_LivePlayer){
                    var video_html = '';
                    video_html +='<video id="live-video-player" webkit-playsinline="true"  playsinline x5-video-player-type="h5" style="width: 100%;height: 100%;" lang="zh-cn"  class="video-js vjs-default-skin  vjs-big-play-centered" controls ';
                    video_html +='poster = "'+liveRoom.roomCoverImgUrl+'"';
                    video_html +='';
                    video_html +='>';
                    video_html +='<source src="'+liveRoom.liveUrl+'" type="application/x-mpegURL" webkit-playsinline="true" playsinline="true">';
                    video_html +='</video>';
                    $("#live-video-block").html(video_html);

                    CURR_LivePlayer = videojs('live-video-player');
                    CURR_LivePlayer.ready(function(){
                        CURR_LivePlayer.play();
                    });
                }
                CURR_LivePlayer.src({
                    type:"application/x-mpegURL" ,
                    src: liveRoom.liveUrl
                });
                $("#live-video-block").show();
                $("#liveload-block").hide();
		$(".vjs-control-bar").css('display','none');//去掉底部的状态条
                CURR_LivePlayer.play();
            }
        },
        error:function(){
        }
    });
}

function loadComments(roomid){
    var url = apiurl+"/view/getcomments";
    $.ajax({
        'url':url,
        'type':'post',
        'data':{roomid:roomid},
        dataType:"jsonp",
        jsonpCallback:"sri_getcomments",
        'success':function(resp){
            console.log(resp);
            var list_html = ''
            for(var i=0;i<resp.records.length;i++){
                var msg = resp.records[i];
                var item = "";
                item +='<li class="chat-item">';
                item +='<div class="message">';
                var nickname = "";
                if(!msg.commentUser){
                    nickname = "游客";
                }else{
                    if(!msg.commentUser.nickname){
                        nickname = msg.commentUser.username
                    }else{
                        nickname = msg.commentUser.nickname;
                    }

                }
                item +='<span class="chat-user-name">'+nickname+':';
                item +='</span>';
                item +='<span class="chat-content">'+msg.content+'</span>';
                item +='</div>';
                item +='</li>';
                list_html +=item;
            }
            $("#chat-block").html(list_html);
            //$('#chat-tab').slimScroll({ scrollBy: '30px' });
            setTimeout(function(){
                var cHeight = document.body.clientHeight;
                var cardHeight = $('.card').outerHeight();
                var tabHeight = $('#view-chat-tab').outerHeight();
                var footerHeight = $('.bar-footer').outerHeight();
                var tab_content_height =  cHeight-cardHeight-tabHeight-footerHeight;
                $j('#chat-tab').slimScroll({
                    start:"bottom",
                    wheelStep:30,
                    touchScrollStep:300,
                    height: tab_content_height+'px'
                });
            },500);
        },'error':function(){
            //$.alert('系统处理异常或超时！');
        }
    });
}

function send(){
    var roomid = _ROOM_ID;
    var message = $("#msginput").val();
    if(!message){
        $.alert("输入留言内容");
        return ;
    }
    var url = "/view/comment"
    $.ajax({
        'url':apiurl+url,
        'type':'post',
        'data':{roomid:roomid,message:message},
        dataType:"jsonp",
        jsonpCallback:"sri_comment",
        'success':function(resp){
            if(resp.success){
                var msg = resp.data;
                $("#msginput").val("");
                var item = "";
                item +='<li class="chat-item">';
                item +='<div class="message">';
                var nickname = "";
                if(!msg.commentUser){
                    nickname = "游客";
                }else{
                    if(!msg.commentUser.nickname){
                        nickname = msg.commentUser.username
                    }else{
                        nickname = msg.commentUser.nickname;
                    }
                }
                item +='<span class="chat-user-name">'+nickname+':';
                item +='</span>';
                item +='<span class="chat-content">'+msg.content+'</span>';
                item +='</div>';
                item +='</li>';
                $("#chat-block").append(item);
            }

        },'error':function(){
            //layer.alert('系统处理异常或超时！',{icon: 2});
        }
    });
}

var $j = jQuery.noConflict();

var _ROOM_ID = null;
$(function () {
    var roomid = GetQueryString("id");
    _ROOM_ID = roomid;
    loadLiveView(roomid);
    loadComments(roomid);
    $(".sendMsg").on("click",function () {
        send();
    });
    setInterval(function(){
        loadComments(roomid);
    },2000);
    $.init();
});
