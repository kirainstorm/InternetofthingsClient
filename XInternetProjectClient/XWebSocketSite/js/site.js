// Write your JavaScript code.
document.write("<script language=javascript src='https://cdn.bootcss.com/jquery/3.3.1/jquery.min.js'></script >");
document.write("<script language=javascript src='https://cdn.bootcss.com/jquery-loading-overlay/2.1.3/loadingoverlay.min.js'></script >");
document.write("<script language=javascript src='https://cdn.bootcss.com/popper.js/1.14.1/umd/popper.min.js'></script >");
document.write("<script language=javascript src='https://cdn.bootcss.com/bootstrap/3.3.7/js/bootstrap.js'></script >");
document.write("<script language=javascript src='https://cdn.bootcss.com/bootstrap-select/1.13.1/js/bootstrap-select.min.js'></script >");
document.write("<script language=javascript src='https://cdn.bootcss.com/bootstrap-switch/3.3.4/js/bootstrap-switch.min.js'></script >");
document.write("<script language=javascript src='https://cdn.bootcss.com/bootstrap-slider/10.0.0/bootstrap-slider.min.js'></script >");
document.write("<script language=javascript src='https://cdn.bootcss.com/magnific-popup.js/1.1.0/jquery.magnific-popup.min.js'></script>");
document.write("<script language=javascript src='https://cdn.bootcss.com/jquery-validate/1.17.0/jquery.validate.min.js'></script >");
document.write("<script language=javascript src='https://cdn.bootcss.com/jquery-validate/1.17.0/localization/messages_zh.min.js'></script >");
document.write("<script language=javascript src='https://cdn.bootcss.com/jquery-validation-unobtrusive/3.2.6/jquery.validate.unobtrusive.min.js'></script >");


document.write("<script language=javascript src='https://cdnjs.cloudflare.com/ajax/libs/jschardet/1.4.1/jschardet.min.js'></script >");


document.write("<script language=javascript src='js/gbk.js'></script >");
document.write("<script language=javascript src='js/utf8.js'></script >");
document.write("<script language=javascript src='js/language.js'></script >");
document.write("<script language=javascript src='js/jsonbuilder.js'></script >");
document.write("<script language=javascript src='js/websocket.js'></script >");
document.write("<script language=javascript src='js/validate.js'></script >");
document.write("<script language=javascript src='js/dvishowhide.js'></script >");
document.write("<script language=javascript src='js/swfobject.js'></script >");
//
var _cur_device_uuid,_cur_device_name;

//
var _checklogintimeloop;
var _checkloginnterval;
function Fun_checkLogin() {
	_checklogintimeloop++;
	if (1 == Fun_getWebsocketConnctionStatus()) {
		clearInterval(_checkloginnterval);
		Fun_ShowDiv(2);
		doSend(Fun_createJsonGetDevicesList());
		return;
	}

	if (_checklogintimeloop > 100) {
		Fun_ShowDiv(0);
		Fun_showModal(_msg_connect_failed);
		clearInterval(_checkloginnterval);
	}

}
function fun_wslogin() {


	// 			var s = Fun_createJsonHeartbeatInfo();
	// 	console.log(s);


	// 	var text = Fun_createJsonHeartbeatInfo();

	// obj = JSON.parse(text);
	// alert(obj.header[0].cmd);

	// //document.getElementById("demo").innerHTML = obj.sites[1].name + " " + obj.sites[1].url;



	// 	var s = Fun_createJsonHeartbeatInfo();
	// 	alert(s);
	// 	var json = JSON.parse(s);
	// 	alert(json.length);
	// 	for(var i=0,l=json.length;i<l;i++){
	// 		for(var key in json[i]){
	// 			alert(key+':'+json[i][key]);
	// 		}
	// 	}
	// return;

	// //
	// //if(0 == fun_getiotip())
	// {
	// //	return;
	// }

	// var i = 0;
	// for(i=0; i < 5; i++)
	// {
	// 	Fun_writeLogToScreen('<span style="color: red;">ERROR:</span> '+ i); 
	// }




	Fun_ShowDiv(1);
	Fun_createWebsocket();
	_checklogintimeloop = 0;
	_checkloginnterval = setInterval(Fun_checkLogin, 100);
}
function fun_wslogout() {
}
function Fun_sendmMessage() {

}



function Fun_GetDevices_callback(data) {
	$("#divdeviceslistdiv ul").remove();
	
	//
	var obj = JSON.parse(data);
	//alert(obj.info.length);
	//alert(obj.info[0].name);

	for (var i = 0; i < obj.info.length; i++) {
		/*
		ulll += '<ul class="list-unstyled">';
		//
		ulll += '<li class="media"><img class="mr-3" src="images/login/group.png" alt="Generic image">';
		ulll += '<div class="media-body">';
		if(0 == obj.info[i].online)
		{
			ulll += '<h5 class="mt-0 mb-1">'+ obj.info[i].name + '</h5>';
		}
		else
		{
			ulll += '<h5 class="mt-0 mb-1"  style="color:chartreuse">' + obj.info[i].name + '</h5>';
			ulll += '<a href="#" onclick="';
			ulll += "Fun_OpenPreview('" + obj.info[i].rtmpip + "','" + obj.info[i].rtmpuuid + "')";
			ulll += '">观看视频</a>  ';
			//
			ulll += '<a href="#" onclick="';
			ulll += "Fun_GetParam('" + obj.info[i].uuid + "')";
			ulll += '">设备参数</a>';
		}

		ulll += '</div>';
		ulll += '</li>';
		//
		ulll += '</ul>';
		*/
		
		var ulll = "";
		
		ulll += '<div class="media">';
 		ulll += '<div class="media-left media-middle">';
  		ulll += '<img class="media-object" src="images/login/group.png" alt="Generic image">';
  		ulll += '</div>';
  		ulll += '<div class="media-body">';
    	ulll += '<h4 class="media-heading">'+ obj.info[i].name + '</h4>';
		if(0 != obj.info[i].online)
		{
			ulll += '<a href="#" onclick="';
			ulll += "Fun_OpenPreview('" + obj.info[i].rtmpip + "','" + obj.info[i].rtmpuuid + "')";
			ulll += '">观看视频</a>  ';
			//
			ulll += '<a href="#" onclick="';
			ulll += "Fun_GetParam('" + obj.info[i].uuid + "','" + obj.info[i].name + "')";
			ulll += '">设备参数</a>';
		}
		ulll += '</div>';
		ulll += '</div>';
		$("#divdeviceslistdiv").append(ulll);
	}
	//alert(ulll);
	

	/*
	<li class="media"> <img class="mr-3" src="..." alt="Generic placeholder image">
            <div class="media-body">
              <h5 class="mt-0 mb-1">List-based media object</h5>
              观看视频 设备参数 图片抓拍 日志信息 修改设备 删除设备
              Cras sit amet nibh libero, in gravida nulla. Nulla vel metus scelerisque ante sollicitudin. Cras purus odio, vestibulum in vulputate at, tempus viverra turpis. Fusce condimentum nunc ac nisi vulputate fringilla. Donec lacinia congue felis in faucibus. </div>
          </li>
	*/
}



function Fun_OpenPreview(rtmpip, rtmpuuid) {
	var openurl = "preview.html?rtmp="+ encodeURIComponent("rtmp://" + rtmpip + "/live/" + rtmpuuid);
	window.open(openurl);
	
	/*
	
	//alert(rtmpip);
	//alert(rtmpuuid);

	var _video_width = 760;
	var _video_height = 399;
	var swfVersionStr = "10.3.0";//<!-- For version detection, set to min. required Flash Player version, or 0 (or 0.0.0), for no version detection. -->
	var xiSwfUrlStr = "swfs/playerProductInstall.swf";//<!-- To use express install, set to playerProductInstall.swf, otherwise the empty string. -->
	var soFlashVars = {
		//rtmp://47.96.249.142/live/5a3048d93e00494ebe629b583a8002bc
		src: "rtmp://" + rtmpip + "/live/" + rtmpuuid,
		//src: "rtmp://live.hkstv.hk.lxdns.com/live/hks",
		streamType: "live",
		autoPlay: "true",
		controlBarAutoHide: "true",
		controlBarPosition: "bottom"
	};
	var params = {
		quality: "high",
		bgcolor: "#000000",
		allowscriptaccess: "sameDomain",
		allowfullscreen: "true"
	};
	var attributes= {
		id: "StrobeMediaPlayback",
		name: "StrobeMediaPlayback",
		align: "middle"
	};
	swfobject.embedSWF("swfs/StrobeMediaPlayback.swf", "flashContent",
		_video_width, _video_height, swfVersionStr, xiSwfUrlStr, soFlashVars, params, attributes);
	swfobject.createCSS("#flashContent", "display:block;text-align:center;");//<!-- JavaScript enabled so display the flashContent div in case it is not replaced with a swf object. -->

	Fun_showPreviewModal();

	//swfobject.stopplay
	*/
}
var _timer_get_set_params;
function Fun_GetSetParam_Error()
{
	$('#param_modal_body').LoadingOverlay("hide");
	alert("操作失败，设备可能离线。");
}
function Fun_GetParam_Ex()
{
	Fun_GetParam(_cur_device_uuid,_cur_device_name);
}
function Fun_GetParam(devuuid,devname) {
	
	$('#param_modal_body').LoadingOverlay("show");
	_timer_get_set_params = setTimeout('Fun_GetSetParam_Error()', 3000)
	_cur_device_uuid = devuuid;
	_cur_device_name = devname;
	Fun_showParamsModalTitle(devname);
	doSend(Fun_createJsonGetDevicesInfo(devuuid));
	
	
	//Fun_showParamsModal();
/*
{
	"header" : 
	{
		"command" : "getparams",
		"deviceuuid" : "3",
		"msguuid" : "eefc6286e2434a1095d878a707d808ed",
		"result" : 0
	},
	"info" : 
	{
		"brightness" : 50,
		"contrast" : 25,
		"dhcp" : 1,
		"flip" : 0,
		"hue" : 15,
		"mainbitrate" : 512,
		"mainframerate" : 25,
		"mainimagesize" : 1,
		"ntpserver" : "203.135.184.123",
		"osdtitle" : "IPC",
		"platid" : "5778",
		"platip" : "47.96.249.142",
		"platport" : 6501,
		"sharpness" : 50,
		"ssid" : "TP-LINK_baoye",
		"staturtion" : 25,
		"subbitrate" : 256,
		"subimagesize" : 0,
		"timezone" : 28800,
		"wifipwd" : "20202024",
		"wiredip" : "192.168.1.200"
	}
}
*/
}
//var mySlider = $("input.slider").slider();
/*
$('#ex1').slider({
}).on('slide', function (slideEvt) {
//当滚动时触发
//console.info(slideEvt);
//获取当前滚动的值，可能有重复
// console.info(slideEvt.value);
}).on('change', function (e) {
	//当值发生改变的时候触发
	//console.info(e);
	//获取旧值和新值
	console.info(e.value.oldValue + '--' + e.value.newValue);
});
lide
当slider被拖动时触发
新的slider值
slideStart
当slider开始拖动时触发
新的slider值
slideStop
当slider停止拖动或slider被点击时触发
新的slider值
change
slider的值改变时触发
带有两个属性的对象："oldValue"和"newValue"
slideEnabled
当设置slider为可用时触发
N/A
slideDisabled
当设置slider为不可用时触发
N/A
*/
//var value = mySlider.slider('getValue');
//mySlider.slider('setValue', 5)
var slider_brightness; 
var slider_contrast;
var slider_saturation;
var slider_sharpness;
function Fun_GetParam_callback(data){
	//
	clearInterval(_timer_get_set_params);//清除计时器
	//
	var obj = JSON.parse(data);
	$('#d_name').val((obj.info.osdtitle));
	//
	$('#d_enc_main_bitrate').val(obj.info.mainbitrate);
	$('#d_enc_main_bitrate').selectpicker('refresh');
	$('#d_enc_main_framerate').val(obj.info.mainframerate);
	$('#d_enc_main_framerate').selectpicker('refresh');
	$('#d_enc_main_size').val(obj.info.mainimagesize);
	$('#d_enc_main_size').selectpicker('refresh');
	$('#d_enc_sub_bitrate').val(obj.info.subbitrate);
	$('#d_enc_sub_bitrate').selectpicker('refresh');
	$('#d_enc_sub_size').val(obj.info.subimagesize);
	$('#d_enc_sub_size').selectpicker('refresh');
	//
	slider_brightness.slider('setValue',obj.info.brightness);
	slider_contrast.slider('setValue',obj.info.contrast);
	slider_saturation.slider('setValue',obj.info.staturtion);
	slider_sharpness.slider('setValue',obj.info.sharpness);
	$('#d_img_flip').val(obj.info.flip);
	$('#d_img_flip').selectpicker('refresh');
	//
	$('#d_wired_dhcp').prop("checked",obj.info.dhcp?true:false);
	//if($('#d_wired_dhcp').prop('checked'))
	$('#d_wired_ip').val(obj.info.wiredip);
	//
	$('#d_wifi_ssid').val(obj.info.ssid);
	$('#d_wifi_pwd').val(obj.info.wifipwd);
	//
	$('#d_time_zone').val(obj.info.timezone);
	$('#d_time_zone').selectpicker('refresh');
	$('#d_time_ntp').val(obj.info.ntpserver);
	
	
	
	Fun_showParamsModal();
	$('#param_modal_body').LoadingOverlay("hide");
}
//
function Fun_SetParam_callback_ok()
{
	clearInterval(_timer_get_set_params);//清除计时器
	$('#param_modal_body').LoadingOverlay("hide");
}
function Fun_SetParam_callback_error()
{
	clearInterval(_timer_get_set_params);//清除计时器
	$('#param_modal_body').LoadingOverlay("hide");
	alert("操作失败，请重试");
}
//
function Fun_SetParam_Osd()
{
	$('#param_modal_body').LoadingOverlay("show");
	_timer_get_set_params = setTimeout('Fun_GetSetParam_Error()', 3000)
	doSend(Fun_createJsonSetOsd(_cur_device_uuid,$('#d_name').val()));
}
//
//
//
function Fun_SetParam_MainBitrate()
{
	$('#param_modal_body').LoadingOverlay("show");
	_timer_get_set_params = setTimeout('Fun_GetSetParam_Error()', 3000)
	doSend(Fun_createJsonSetMainBitRate(_cur_device_uuid,$('#d_enc_main_bitrate').val()));
}
function Fun_SetParam_MainFrameRate()
{
	$('#param_modal_body').LoadingOverlay("show");
	_timer_get_set_params = setTimeout('Fun_GetSetParam_Error()', 3000)
	doSend(Fun_createJsonSetMainFrameRate(_cur_device_uuid,$('#d_enc_main_framerate').val()));
}
function Fun_SetParam_MainSize()
{
	$('#param_modal_body').LoadingOverlay("show");
	_timer_get_set_params = setTimeout('Fun_GetSetParam_Error()', 3000)
	doSend(Fun_createJsonSetMainSize(_cur_device_uuid,$('#d_enc_main_size').val()));
}
function Fun_SetParam_SubBitrate()
{
	$('#param_modal_body').LoadingOverlay("show");
	_timer_get_set_params = setTimeout('Fun_GetSetParam_Error()', 3000)
	doSend(Fun_createJsonSetSubBitRate(_cur_device_uuid,$('#d_enc_sub_bitrate').val()));
}
function Fun_SetParam_SubSize()
{
	$('#param_modal_body').LoadingOverlay("show");
	_timer_get_set_params = setTimeout('Fun_GetSetParam_Error()', 3000)
	doSend(Fun_createJsonSetSubSize(_cur_device_uuid,$('#d_enc_sub_size').val()));
}
//
//
//
function Fun_SetParam_Img_Brightness(val)
{
	console.log(val);//
	$('#param_modal_body').LoadingOverlay("show");
	_timer_get_set_params = setTimeout('Fun_GetSetParam_Error()', 3000)
	doSend(Fun_createJsonSetBrightness(_cur_device_uuid,val));
}
function Fun_SetParam_Img_Contrast(val)
{
	$('#param_modal_body').LoadingOverlay("show");
	_timer_get_set_params = setTimeout('Fun_GetSetParam_Error()', 3000)
	doSend(Fun_createJsonSetContrast(_cur_device_uuid,val));
}
function Fun_SetParam_Img_Staturtion(val)
{
	$('#param_modal_body').LoadingOverlay("show");
	_timer_get_set_params = setTimeout('Fun_GetSetParam_Error()', 3000)
	doSend(Fun_createJsonSetStaturtion(_cur_device_uuid,val));
}
function Fun_SetParam_Img_Sharpness(val)
{
	$('#param_modal_body').LoadingOverlay("show");
	_timer_get_set_params = setTimeout('Fun_GetSetParam_Error()', 3000)
	doSend(Fun_createJsonSetSharpness(_cur_device_uuid,val));
}
function Fun_SetParam_Img_Flip()
{
	$('#param_modal_body').LoadingOverlay("show");
	_timer_get_set_params = setTimeout('Fun_GetSetParam_Error()', 3000)
	doSend(Fun_createJsonSetFlip(_cur_device_uuid,$('#d_img_flip').val()));
}
//
//
//
function Fun_SetParam_Wired()
{
	$('#param_modal_body').LoadingOverlay("show");
	_timer_get_set_params = setTimeout('Fun_GetSetParam_Error()', 3000)
	doSend(Fun_createJsonSetWired(_cur_device_uuid,($('#d_wired_dhcp').prop('checked'))?1:0,$('#d_wired_ip').val()));
}
//
//
//
function Fun_SetParam_Wifi()
{
	$('#param_modal_body').LoadingOverlay("show");
	_timer_get_set_params = setTimeout('Fun_GetSetParam_Error()', 3000)
	doSend(Fun_createJsonSetWifi(_cur_device_uuid,$('#d_wifi_ssid').val(),$('#d_wifi_pwd').val()));
}
//
//
//
function Fun_SetParam_Timezone()
{
	$('#param_modal_body').LoadingOverlay("show");
	_timer_get_set_params = setTimeout('Fun_GetSetParam_Error()', 3000)
	doSend(Fun_createJsonSetTimeZone(_cur_device_uuid,$('#d_time_zone').val()));
}
function Fun_SetParam_NtpServer()
{
	$('#param_modal_body').LoadingOverlay("show");
	_timer_get_set_params = setTimeout('Fun_GetSetParam_Error()', 3000)
	doSend(Fun_createJsonSetNtpServer(_cur_device_uuid,$('#d_time_ntp').val()));
}














