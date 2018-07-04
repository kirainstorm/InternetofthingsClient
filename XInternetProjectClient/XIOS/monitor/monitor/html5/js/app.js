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

var $page = null;
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

function setupWebViewJavascriptBridge(callback) {
    if (window.WebViewJavascriptBridge) { return callback(WebViewJavascriptBridge); }
    if (window.WVJBCallbacks) { return window.WVJBCallbacks.push(callback); }
    window.WVJBCallbacks = [callback];
    var WVJBIframe = document.createElement('iframe');
    WVJBIframe.style.display = 'none';
    WVJBIframe.src = 'wvjbscheme://__BRIDGE_LOADED__';
    document.documentElement.appendChild(WVJBIframe);
    setTimeout(function() { document.documentElement.removeChild(WVJBIframe) }, 0)
}

setupWebViewJavascriptBridge(function(bridge) {
     bridge.registerHandler('testJavascriptHandler', function(data, responseCallback) {
        //alert('JS方法被调用:'+data);
        //responseCallback('js执行过了');
     })
})

function routerLoad($this){
    var roomid = $($this).attr("data-id");
    CURR_LIVE_ID = roomid;
    //$.router.load("#page-view");  //加载内联页面

//window.WebViewJavascriptBridge.callHandler('showWebPreview', {'url': '+ CURR_LIVE_ID +', 'events':'1'})
window.WebViewJavascriptBridge.callHandler('showWebPreview',CURR_LIVE_ID );

    //AndroidFunction.showWebPreview(CURR_LIVE_ID);//这个给android使用

    //window.open("file:///E:/NewPlatform/PlatformForKindergarten/Android/PlatformMonitor/app/src/main/assets/preview.html?id="+CURR_LIVE_ID);
}

function LoadLunboUrl(_url)
{
    //alert(_url);
window.WebViewJavascriptBridge.callHandler('showWebAD',_url);
    //AndroidFunction.showWebAD(_url);
    //window.open(_url);
}

// function GetQueryString(name)
// {
//      var reg = new RegExp("(^|&)"+ name +"=([^&]*)(&|$)");
//      var r = window.location.search.substr(1).match(reg);
//      if(r!=null)return  unescape(r[2]); return null;
// }

// function PreviewViewOnload()//播放视频
// {
//     //获取preview.html?id=
//     alert(GetQueryString(id));
//     loadLiveView(GetQueryString(id));
// }


function loadIndexLiveData(type,$content){
    if(!type){
        $.showPreloader('直播加载中');
    }

    $.ajax({
        type:"get",
        url:apiurl+"/homeHotLiveList",/*url写异域的请求地址*/
        dataType:"jsonp",/*加上datatype*/
        jsonpCallback:"sri_homeHotLiveList",/*设置一个回调函数，名字随便取，和下面的函数里的名字相同就行*/
        success:function(resultdata){
            if(resultdata != null && resultdata.length > 0){
                var list_all_html = '';
                for(var i = 0; i < resultdata.length; i++){
                    var hotlist_html = '';
                    hotlist_html += '<div class="home-live-list-title">';
                    hotlist_html += '<h1>';
                    hotlist_html += '<span class="name">'+resultdata[i].title+'</span>';
                    hotlist_html += '</h1>';
                    hotlist_html += '</div>';

                    hotlist_html += '<div class="row no-gutter">';
                    var livelist = resultdata[i].liveRoomList;
                    if(livelist != null && livelist.length > 0){
                        for(var j = 0; j < livelist.length; j++){
                            var liveItem = "";
                            var roomid = livelist[j].id;
                            liveItem += '<div class="col-50">';
                            liveItem += '<div class="card">';
                            liveItem += '<div class="card-content">';
                            liveItem += '<div class="card-content-inner" style="padding:0.25rem;">';
                            liveItem += '<a  data-id="'+roomid+'" onclick="routerLoad(this)">';
                            liveItem += '<div class="cover">';
                            liveItem += '<div class="cate">台号：'+livelist[j].roomNo+'</div>';
                            liveItem += '<img style="display: block;width: 100%;height: 100%;"  onerror="imgError(this)" data-original="'+livelist[j].roomCoverImgUrl+'" src="'+livelist[j].roomCoverImgUrl+'" alt="'+livelist[j].roomName+'" class="lazy">';
                            liveItem += '</div>';
                            liveItem += '<div class="info">';
                            liveItem += '<div class="name">'+livelist[j].roomName+'</div>';
                            liveItem += '<div class="person_num">'+livelist[j].liveRoomInfo.viewNum+'</div>';
                            liveItem += '</div>';
                            liveItem += '</a>';
                            liveItem += '</div>';
                            liveItem += '</div>';
                            liveItem += '</div>';
                            liveItem += '</div>';
                            hotlist_html += liveItem;
                        }
                    }
                    hotlist_html += '</div>';
                    list_all_html += hotlist_html
                }
                $("#hotlivelist").html(list_all_html);
                if(!type){
                    $.hidePreloader();
                }else if(type == "refresh"){
                    $(window).scrollTop(0);
                    $.pullToRefreshDone($content);
                }
                refresh_status = false;
            }
        },
        error:function(){
            refresh_status = false;
            $.hidePreloader();
            $.alert('请检查网络');
        }
    });
}

function loadNavLiveData(_TYPE,_DATAID,optType,$content,callbackFun){
    if(optType == null){
        $.showIndicator();
    }
    var data = {};
    if(!_TYPE){
        data.type = HOME_CURR_TYPE;
    }else{
        data.type = _TYPE;
    }
    if(!_DATAID){
        data.dataid = HOME_CURR_DATAID;
    }else{
        data.dataid = _DATAID;
    }
    //var page = 1||$page;
    console.log(data);
    var orderby = "view_num";
    //mark
    $("#hotlivelist").html("");
    flow.load({
        elem: '#hotlivelist',
        scrollElem: '.content ',
        isAuto:true,
        done: function(page, next){
            setTimeout(function(){
                $.ajax({
                    type:"post",
                    url:apiurl+"/navLiveList?current="+page+"&orderby="+orderby,/*url写异域的请求地址*/
                    data:data,
                    dataType:"jsonp",/*加上datatype*/
                    jsonpCallback:"sri_navLiveList",/*设置一个回调函数，名字随便取，和下面的函数里的名字相同就行*/
                    success:function(resultdata){
                        console.log(resultdata);
                        /*
                        if(resultdata != null && resultdata.records != null &&  resultdata.records.length > 0){

                        }else {
                            var nodatahmtl = "";
                            nodatahmtl += "<div class='content-block text-center'>";
                            nodatahmtl += "没有查询到符合条件的数据";
                            nodatahmtl += "</div>";
                            $("#hotlivelist").html(nodatahmtl);
                        }
                        */
                        var liveItem = "";
                        var records = resultdata.records;
                        liveItem += '<div class="row no-gutter">';
                        for(var j = 0; j < records.length; j++){
                            var roomid = records[j].id;
                            liveItem += '<div class="col-50">';
                            liveItem += '<div class="card">';
                            liveItem += '<div class="card-content">';
                            liveItem += '<div class="card-content-inner" style="padding:0.25rem;">';
                            liveItem += '<a data-id="'+roomid+'" onclick="routerLoad(this)">';
                            liveItem += '<div class="cover">';
                            liveItem += '<div class="cate">台号：'+records[j].roomNo+'</div>';
                            liveItem += '<img style="display: block;width: 100%;height: 100%;"  onerror="imgError(this)" data-original="'+records[j].roomCoverImgUrl+'" src="'+records[j].roomCoverImgUrl+'" alt="'+records[j].roomName+'" class="lazy">';
                            liveItem += '</div>';
                            liveItem += '<div class="info">';
                            liveItem += '<div class="name">'+records[j].roomName+'</div>';
                            liveItem += '<div class="person_num">'+records[j].liveRoomInfo.viewNum+'</div>';
                            liveItem += '</div>';
                            liveItem += '</a>';
                            liveItem += '</div>';
                            liveItem += '</div>';
                            liveItem += '</div>';
                            liveItem += '</div>';
                        }
                        liveItem += "</div>";
                        next(liveItem, page < resultdata.pages);


                        if(optType != null){
                            if(optType == "refresh"){
                                $(window).scrollTop(0);
                                $.pullToRefreshDone($content);
                            }
                        }else{
                            $.hideIndicator();
                        }
                        if(callbackFun){
                            callbackFun.call();
                        }
                        refresh_status = false;
                    },
                    error:function(){
                        refresh_status = false;
                        $.hideIndicator();
                        $.alert('请检查网络');
                    }
                });
            },500);

        }
    });
}

// function loadLiveView(roomid){
//     $.ajax({
//         type:"get",
//         url:apiurl+"/view?roomid="+roomid,/*url写异域的请求地址*/
//         dataType:"jsonp",/*加上datatype*/
//         jsonpCallback:"sri_liveview",/*设置一个回调函数，名字随便取，和下面的函数里的名字相同就行*/
//         success:function(resultdata) {
//             if(resultdata != null){
//                 $("#page-view").find(".bar-nav .title").text(resultdata.roomName);
//                 $("#view_room_no").text(resultdata.roomNo);
//                 $("#view_room_view_num").text(8888);
//                 $("#view_room_intro").text(resultdata.roomIntro);

//                 if(!CURR_LivePlayer){
//                     var video_html = '';
//                     video_html +='<video id="live-video-player" webkit-playsinline="true" style="width: 100%;height: 100%;" lang="zh-cn"  class="video-js vjs-default-skin" controls ';
//                     video_html +='poster = "'+resultdata.roomCoverImgUrl+'"';
//                     video_html +='';
//                     video_html +='>';
//                     video_html +='<source src="'+resultdata.liveUrl+'" type="application/x-mpegURL">';
//                     video_html +='</video>';
//                     $("#live-video-block").html(video_html);

//                     CURR_LivePlayer = videojs('live-video-player');
//                     CURR_LivePlayer.ready(function(){
//                         CURR_LivePlayer.play();
//                     });
//                 }
//                 CURR_LivePlayer.src({
//                     type:"application/x-mpegURL" ,
//                     src: resultdata.liveUrl
//                 });
//                 $("#live-video-block").show();
//                 $("#liveload-block").hide();
//                 CURR_LivePlayer.play();
//             }
//         },
//         error:function(){
//         }
//     });
// }

function loadLunboList(){
    $.ajax({
        type:"get",
        url:apiurl+"/lunbo",/*url写异域的请求地址*/
        dataType:"jsonp",/*加上datatype*/
        jsonpCallback:"sri_lunboList",/*设置一个回调函数，名字随便取，和下面的函数里的名字相同就行*/
        success:function(resultdata) {
            //alert(resultdata);
            if(resultdata != null && resultdata.length > 0){
                var lunbo_html = '';
                lunbo_html += '<div id="slides" class="swiper-container-horizontal">';
                lunbo_html += '<div class="swiper-wrapper">';
                for(var i=0; i < resultdata.length; i++){
                    lunbo_html += '<div class="swiper-slide" >';
                    lunbo_html += '<a href="#" ><img src="'+resultdata[i].lunboImgUrl+'"  class="lazy" onclick=LoadLunboUrl("'+resultdata[i].url+'") ></a>';
                    lunbo_html += '</div>';
                }
                lunbo_html += '</div>';
                lunbo_html += '<div class="pagination swiper-pagination-bullets">';
                lunbo_html += '</div>';
                lunbo_html += '</div>';
                //alert(lunbo_html);
                $("#lunbolist").html(lunbo_html);
                var lunboSwiper = new Swiper('#slides', {
                    direction: 'horizontal',
                    loop: true,
                    // 如果需要分页器
                    pagination: '#slides .pagination'
                });
                /*
                $("img.lazy").show().lazyload({
                    effect : "fadeIn"
                });
                */
            }
        },
        error:function(){
        }
    });
}

function indexInit(){
    $.ajax({
        type:"get",
        url:apiurl+"/loadNavList",/*url写异域的请求地址*/
        dataType:"jsonp",/*加上datatype*/
        jsonpCallback:"sri_loadNavList",/*设置一个回调函数，名字随便取，和下面的函数里的名字相同就行*/
        success:function(resultdata) {
            if(resultdata != null && resultdata.length > 0) {
                var nav_list_html = '';
                nav_list_html += '<div class="swiper-slide active"  data-type="0"><span>首页</span></div>';
                for (var i = 0; i < resultdata.length; i++) {
                    var swiper_slide_width ="width:3rem";
                    if(strlen(resultdata[i].title)<=6){
                        swiper_slide_width ="width:3rem";
                    }else if(strlen(resultdata[i].title)<=8){
                        swiper_slide_width ="width:4.5rem";
                    }else if(strlen(resultdata[i].title)<=12){
                        swiper_slide_width ="width:5.5rem";
                    }
                    nav_list_html += '<div class="swiper-slide" style="'+swiper_slide_width+'" data-type="'+resultdata[i].type+'" data-val="'+resultdata[i].linkData+'"><span>'+resultdata[i].title+'</span></div>';
                }
            }
            $("#topNav .swiper-wrapper").html(nav_list_html);

            //----------下滚动加载 end----------
            var mySwiper = new Swiper('#topNav', {
                freeMode: true,
                freeModeMomentumRatio: 0.5,
                slidesPerView: 'auto',
                onTouchStart: function(swiper,even) {
                    var type = $(even.srcElement).parent().attr("data-type");
                    var val = $(even.srcElement).parent().attr("data-val");
                    //console.log(val);
                    if(type == 0){
                        HOME_CURR_STATUS = "home";
                        HOME_CURR_TYPE = null;
                        HOME_CURR_DATAID = null;
                        loadLunboList();
                        loadIndexLiveData();
                    }else if(type == 1){
                        HOME_CURR_STATUS = "home";
                        HOME_CURR_TYPE = null;
                        HOME_CURR_DATAID = null;
                    }else if(type == 2){
                        $("#lunbolist").html("");
                        HOME_CURR_STATUS = "nav";
                        HOME_CURR_TYPE ="hot";
                        HOME_CURR_DATAID = val;
                        loadNavLiveData("hot",val);
                    }else if(type == 3){
                        $("#lunbolist").html("");
                        HOME_CURR_STATUS = "nav";
                        HOME_CURR_TYPE ="category";
                        HOME_CURR_DATAID = val;

                        loadNavLiveData("category",val);
                    }
                }
            });

            var swiperWidth = mySwiper.container[0].clientWidth;
            var maxTranslate = mySwiper.maxTranslate();
            var maxWidth = -maxTranslate + swiperWidth / 2;

            mySwiper.on('tap', function (swiper, e) {
                //e.preventDefault()
                var slide = swiper.slides[swiper.clickedIndex];
                var slideLeft = slide.offsetLeft;
                var slideWidth = slide.clientWidth;
                var slideCenter = slideLeft + slideWidth / 2;
                // 被点击slide的中心点
                mySwiper.setWrapperTransition(300);
                if (slideCenter < swiperWidth / 2) {
                    mySwiper.setWrapperTranslate(0);
                } else if (slideCenter > maxWidth) {
                    mySwiper.setWrapperTranslate(maxTranslate);
                } else {
                    var nowTlanslate = slideCenter - swiperWidth / 2;
                    mySwiper.setWrapperTranslate(-nowTlanslate);
                }
                $("#topNav  .active").removeClass('active');
                $("#topNav .swiper-slide").eq(swiper.clickedIndex).addClass('active');
            });
        },
        error:function(){
            $.alert('请检查网络');
        }
    });

}

var flow = null;
layui.use('flow', function(){
    flow = layui.flow;
});

$(function () {
    indexInit();

    //----------首页--js--开始----------
    $(document).on("pageInit", "#page-home", function(e, id, page) {
        if(CURR_LivePlayer){
            CURR_LivePlayer.pause();
            $("#live-video-block").hide();
            $("#liveload-block").show();
        }
        CURR_LIVE_ID = null ;
        if(HOME_CURR_STATUS=="home"){
            if(!HOME_INIT){
                loadLunboList();
                loadIndexLiveData();
                HOME_INIT = true;
            }
        }else{
            $("#lunbolist").html("");
            loadNavLiveData();
            HOME_INIT = false;
        }
        //----------下拉刷新 start----------

        var $content = $(page).find(".content").on('refresh', function(e){
            if(refresh_status){
                return;
            }
            refresh_status = true;
            if(HOME_CURR_STATUS == "home"){
                loadIndexLiveData("refresh",$content);
            }else if(HOME_CURR_STATUS == "search"){
                loadNavLiveData("kw",CURR_search_kw,"refresh",$content);
            }else{
                loadNavLiveData(null,null,"refresh",$content);
            }

        });
        //----------下拉刷新 end----------

    });

    $(document).on('click','.open-search-modal', function () {
        $.modal({
            title:  '搜索直播房间',
            text: '<div class="searchbar"><div class="search-input"><label class="icon icon-search" for="search"></label><input type="search" id="searchKw" placeholder="输入关键字..."/></div></div>',
            buttons: [
                {
                    text: '取消',
                    onClick: function() {}
                },
                {
                    text: '搜索',
                    onClick: function() {
                        var dataid = $("#searchKw").val();
                        $("#lunbolist").html("");
                        HOME_CURR_STATUS = "search";
                        CURR_search_kw = dataid;
                        $("#topNav  .active").removeClass('active');
                       loadNavLiveData("kw",dataid);

                    }
                }
            ]
        })
    });
    //----------首页--js--结束----------


    //----------直播页--js--开始----------
    $(document).on("pageInit", "#page-view", function(e, id, page) {
        loadLiveView(CURR_LIVE_ID);
    });
    //----------直播页--js--结束----------

    $.init();
});

