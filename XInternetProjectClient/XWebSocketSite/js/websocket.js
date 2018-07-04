// Write your JavaScript code.
var _user_status = 0; //0:管理员权限  1：访问者权限
var _websocket;
var _wsStatus = 0; //0:未登陆 1：成功登陆
var _wsRedirectServer = "ws://192.168.1.159:6000/";
var _wsHeartbeatInterval;
function Fun_getWebsocketConnctionStatus() {
	return _wsStatus;
}
//用于ip重定向
function Fun_getIotIp(id) {
	var _ret = 0;
	$.ajaxSettings.async = false; //同步执行
	//('http://47.96.249.142:81/api/userapi/' + $.base64.btoa(id))
	$.getJSON('http://47.96.249.142:81/api/userapi/aGhla2xAcXEuY29t')
		.done(function (data) {
			console.log(">> getIOTServerIp >>  Get the data successfully");
			//{"iotip":"47.96.249.142"}
			$.each(data, function (name, value) {
				$.cookie(name, value)
			});
			//setTimeout(getRedirectIpSuccess,30);
			_ret = 1;
		})
		.fail(function (jqXHR, textStatus, err) {
			console.log(">> getIOTServerIp >>  Failed to get data" + err);
			//setTimeout(getRedirectIpFailed,30);
		});
	$.ajaxSettings.async = true;
	return _ret;
}
function Fun_websocketHeartbeatInterval() {
	var msg = Fun_createJsonHeartbeatInfo();
	doSend(msg);
}
function Fun_createWebsocket() {
	//_websocket = new WebSocket("ws://127.0.0.1:6502/");
	_websocket = new WebSocket("ws://47.96.249.142:6502/");
	_websocket.onopen = function (evt) {
		onOpen(evt)
	};
	_websocket.onclose = function (evt) {
		onClose(evt)
	};
	_websocket.onmessage = function (evt) {
		onMessage(evt)
	};
	_websocket.onerror = function (evt) {
		onError(evt)
	};
}

function onOpen(evt) {
	//writeToScreen("CONNECTED"); 
	//doSend("WebSocket rocks"); 
	//doSend();
	//setInterval(doSend,100);

	Fun_writeLogToScreen("websocket connected!");
	var a = $("#usernametext").val();
	var b = $("#pwdtext").val();
	var msg = Fun_createJsonLoginInfo(a, b);
	doSend(msg);
}

function onClose(evt) {
	//writeToScreen("DISCONNECTED"); 
	_wsStatus = 0;
	clearInterval(_wsHeartbeatInterval);
	Fun_writeLogToScreen("websocket closed!");
	//
	Fun_showModal("websocket断开，请写好自动重连逻辑，或者要求用户再次登陆");
}

function onMessage(evt) {
	//writeToScreen('<span style="color: blue;">RESPONSE: '+ evt.data+'</span>');
	console.log(evt.data);

	var obj = JSON.parse(evt.data);
	var cmd = obj.header.command;
	if ("login" == cmd) {
		//alert(cmd);
		_wsStatus = 1; //收到login回复才算真正登陆成功
		_wsHeartbeatInterval = setInterval(Fun_websocketHeartbeatInterval, 5000);
	}
	if ("heartbeat" == cmd) {
		return;
	}
	if ("getdevices" == cmd) {
		Fun_GetDevices_callback(evt.data);
	}
	if (("getparams" == cmd) && (obj.header.msguuid == msguuid_getparams)) {
		if (0 == obj.header.result)
			Fun_GetParam_callback(evt.data);
	}
	if (("setparam" == cmd) && (obj.header.msguuid == msguuid_setparams)) {
		if (0 == obj.header.result)
			Fun_SetParam_callback_ok();
		else
			Fun_SetParam_callback_error()
	}

	var msg = '<span style="color: red;">RESPONSE</span>' + evt.data;
	Fun_writeLogToScreen(msg);
	/*
	var text = '{ "sites" : [' +
    '{ "name":"Runoob" , "url":"www.runoob.com" },' +
    '{ "name":"Google" , "url":"www.google.com" },' +
    '{ "name":"Taobao" , "url":"www.taobao.com" } ]}';
	*/
	//obj.sites[1].name	//obj.节点名[索引].属性
	//obj.header.cmd 	//obj.节点名.属性
}

function onError(evt) {
	//writeToScreen('<span style="color: red;">ERROR:</span> '+ evt +evt.data); 
	_websocket.close();
	//location.reload();
}

function doSend(message) {
	//var message = "WebSocket rocks";
	//var utf8jsonmessage = eval('(' + message + ')');
	//var s = JSON.stringify(utf8jsonmessage);
	var obj = JSON.parse(message);
	var cmd = obj.header.command;
	if ("heartbeat" != cmd) {
		var msg = '<span style="color: blue;">SEND:</span>' + message;
		Fun_writeLogToScreen(msg);
	}
	_websocket.send(message);
}
function Fun_websocketSendJsonTest() {
	var msg = $("#logtextareatest").val();
	//alert(msg);
	doSend(msg);
}