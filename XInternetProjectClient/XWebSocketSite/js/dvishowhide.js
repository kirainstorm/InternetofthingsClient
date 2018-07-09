// Write your JavaScript code.
function Fun_showModal(msg) {
	$("#myModalMsg").text(msg);
	$('#myModal').modal({backdrop:'static',keyboard:false}); 
}
function Fun_showPreviewModal() {
	$('#myModalPreview').modal({backdrop:'static',keyboard:false}); 
}

var __bModalParamsOpen = false;//参数设置对话框是否处于打开状态
function Fun_showParamsModalTitle(name){

	//设置标题
	$('#myModalParams_title').text("参数设置:"+name);
}
function Fun_showParamsModal(name) {
	if(false == __bModalParamsOpen)
	{
		$('#ParamTab a:first').tab('show');//显示第一个选项卡
		$('#myModalParams').modal({backdrop:'static',keyboard:false});
	}
}
$(function () { $('#myModalParams').on('hide.bs.modal', function () {
      __bModalParamsOpen = false;
})});
$(function () { $('#myModalParams').on('show.bs.modal', function () {
      __bModalParamsOpen = true;
})});
function Fun_hideAllDiv() {
	$("#divlogin").hide();
	$("#divloading").hide();
	$("#divtitle").hide();
	$("#divdevices").hide();
	$("#divmap").hide();
	$("#divlog").hide();
	$("#divsysset").hide();
	$("#divhelp").hide();
	//
	$("#nav-item-a-1").removeClass("active");
	$("#nav-item-a-2").removeClass("active");
	$("#nav-item-a-3").removeClass("active");
	$("#nav-item-a-4").removeClass("active");
	$("#nav-item-a-5").removeClass("active");
	//
}

function Fun_ShowDiv(_index) {
	Fun_hideAllDiv();
	if (0 == _index) {
		$("#divlogin").show();
	} else if (1 == _index) {
		$("#loadingmsg").text(_msg_connecting);
		$("#divloading").show();
	} else if (2 == _index) {
		$("#nav-item-a-1").addClass("active");
		$("#divtitle").show();
		$("#divdevices").show();
	} else if (3 == _index) {
		$("#nav-item-a-2").addClass("active");
		$("#divtitle").show();
		$("#divmap").show();
	} else if (4 == _index) {
		$("#nav-item-a-3").addClass("active");
		$("#divtitle").show();
		$("#divlog").show();
	} else if (5 == _index) {
		$("#nav-item-a-4").addClass("active");
		$("#divtitle").show();
		$("#divsysset").show();
	} else if (6 == _index) {
		$("#nav-item-a-5").addClass("active");
		$("#divtitle").show();
		$("#divhelp").show();
	}
}
function getNowFormatDate() {
    var date = new Date();
    var seperator1 = "-";
    var seperator2 = ":";
    var month = date.getMonth() + 1;
    var strDate = date.getDate();
    if (month >= 1 && month <= 9) {
        month = "0" + month;
    }
    if (strDate >= 0 && strDate <= 9) {
        strDate = "0" + strDate;
    }
    var currentdate = date.getFullYear() + seperator1 + month + seperator1 + strDate
            + " " + date.getHours() + seperator2 + date.getMinutes()
            + seperator2 + date.getSeconds();
    return currentdate;
} 
var _logcount = 0;
var _logObj;
function Fun_writeLogToScreen(message) {
	
	var output = document.getElementById("logoutput"); 
	var pre = document.createElement("p");
	pre.style.wordWrap = "break-word";
	var msg = getNowFormatDate();
	msg += ": ";
	msg += message;
	pre.innerHTML = msg;
	if(0 == _logcount){
		_logcount = 1;
		output.appendChild(pre);
		_logObj = pre;
	}
	else
	{
		output.insertBefore(pre,_logObj);
		_logObj = pre;
	}
}