//格式化 Javascript http://www.css88.com/tool/js_beautify/
// 转为unicode 编码--不转换字母、数字等,只转换中文
function JsEncodeUnicode(v) {
    // return s.replace(/([\u4E00-\u9FA5]|[\uFE30-\uFFA0])/g, function (work) {
    //     return "\\u" + work.charCodeAt(0).toString(16);
    // });
    
    var unicode = "";
    for (var i = 0; i < v.length; i++) {
        var code = Number(v[i].charCodeAt(0));
        if (code > 127) {
            var charAscii = code.toString(16);
            charAscii = new String("0000").substring(charAscii.length, 4) + charAscii;
            unicode += "\\u" + charAscii;
        } else {
            unicode += v[i];
        }
    }
    return unicode;
}
//to gbk 
function Fun_toGBK(str) {
    var s = $URL.encode(str);
    var s1 = s;//.replace(/%/g,"\\x");
    return s1;
}
function Fun_gbkToAscii(str) {
    var s = str;//.replace(/\\x/g,"\%");
    var s1 = $URL.decode(s);

    return s1;
}
function Fun_guid() {
    return 'xxxxxxxxxxxx4xxxyxxxxxxxxxxxxxxx'.replace(/[xy]/g, function (c) {
        var r = Math.random() * 16 | 0, v = c == 'x' ? r : (r & 0x3 | 0x8);
        return v.toString(16);
    });
}
//login
function Fun_createJsonLoginInfo(user, pwd) {
    var s = '{"header":{"command":"login","result":0,"deviceuuid":"","msguuid":""},"info":{"user":"' + user + '","pwd":"' + pwd + '"}}';
    return s;
}
//Heartbeat
function Fun_createJsonHeartbeatInfo() {
    return '{"header":{"command":"heartbeat","result":0,"deviceuuid":"","msguuid":""}}';
}
//getdevice
function Fun_createJsonGetDevicesList() {
    return '{"header":{"command":"getdevices","result":0,"deviceuuid":"","msguuid":""}}';
}
//
var msguuid_getparams = "";
var msguuid_setparams = "";
//获取参数
function Fun_createJsonGetDevicesInfo(devuuid) {
    msguuid_getparams = Fun_guid();
    return '{"header":{"command":"getparams","result":0,"deviceuuid":"' + devuuid + '","msguuid":"' + msguuid_getparams + '"}}';
}

//设置OSD

function Fun_createJsonSetOsd(devuuid, val) {
    msguuid_setparams = Fun_guid();
    var s = '{"header":{"command":"setparam","result":0,"deviceuuid":"' + devuuid + '","msguuid":"' + msguuid_setparams + '"},"info":{"command":"osdtitle","osdtitle":"' + JsEncodeUnicode(val) + '"}}';
    return s;
}
//设置主码流
function Fun_createJsonSetMainBitRate(devuuid, val) {
    msguuid_setparams = Fun_guid();
    var s = '{"header":{"command":"setparam","result":0,"deviceuuid":"' + devuuid + '","msguuid":"' + msguuid_setparams + '"},"info":{"command":"mainbitrate","mainbitrate":' + val + '}}';
    return s;
}
//设置主码流帧率
function Fun_createJsonSetMainFrameRate(devuuid, val) {
    msguuid_setparams = Fun_guid();
    var s = '{"header":{"command":"setparam","result":0,"deviceuuid":"' + devuuid + '","msguuid":"' + msguuid_setparams + '"},"info":{"command":"mainframerate","mainframerate":' + val + '}}';
    return s;
}
//设置主码流分辨率
function Fun_createJsonSetMainSize(devuuid, val) {
    msguuid_setparams = Fun_guid();
    var s = '{"header":{"command":"setparam","result":0,"deviceuuid":"' + devuuid + '","msguuid":"' + msguuid_setparams + '"},"info":{"command":"mainimagesize","mainimagesize":' + val + '}}';
    return s;
}
//设置子码流
function Fun_createJsonSetSubBitRate(devuuid, val) {
    msguuid_setparams = Fun_guid();
    var s = '{"header":{"command":"setparam","result":0,"deviceuuid":"' + devuuid + '","msguuid":"' + msguuid_setparams + '"},"info":{"command":"subbitrate","subbitrate":' + val + '}}';
    return s;
}
//设置子码流分辨率
function Fun_createJsonSetSubSize(devuuid, val) {
    msguuid_setparams = Fun_guid();
    var s = '{"header":{"command":"setparam","result":0,"deviceuuid":"' + devuuid + '","msguuid":"' + msguuid_setparams + '"},"info":{"command":"subimagesize","subimagesize":' + val + '}}';
    return s;
}
//设置亮度
function Fun_createJsonSetBrightness(devuuid, val) {
    msguuid_setparams = Fun_guid();
    var s = '{"header":{"command":"setparam","result":0,"deviceuuid":"' + devuuid + '","msguuid":"' + msguuid_setparams + '"},"info":{"command":"brightness","brightness":' + val + '}}';
    return s;
}
//设置对比度
function Fun_createJsonSetContrast(devuuid, val) {
    msguuid_setparams = Fun_guid();
    var s = '{"header":{"command":"setparam","result":0,"deviceuuid":"' + devuuid + '","msguuid":"' + msguuid_setparams + '"},"info":{"command":"contrast","contrast":' + val + '}}';
    return s;
}
//设置饱和度
function Fun_createJsonSetStaturtion(devuuid, val) {
    //staturtion  单词错了，应该是 saturation ，先这样用
    msguuid_setparams = Fun_guid();
    var s = '{"header":{"command":"setparam","result":0,"deviceuuid":"' + devuuid + '","msguuid":"' + msguuid_setparams + '"},"info":{"command":"staturtion","staturtion":' + val + '}}';
    return s;
}
//设置锐度
function Fun_createJsonSetSharpness(devuuid, val) {
    msguuid_setparams = Fun_guid();
    var s = '{"header":{"command":"setparam","result":0,"deviceuuid":"' + devuuid + '","msguuid":"' + msguuid_setparams + '"},"info":{"command":"sharpness","sharpness":' + val + '}}';
    return s;
}
//设置翻转
function Fun_createJsonSetFlip(devuuid, val) {
    msguuid_setparams = Fun_guid();
    var s = '{"header":{"command":"setparam","result":0,"deviceuuid":"' + devuuid + '","msguuid":"' + msguuid_setparams + '"},"info":{"command":"flip","flip":' + val + '}}';
    return s;
}
//设置有线网络
function Fun_createJsonSetWired(devuuid, dhcp, wiredip) {
    msguuid_setparams = Fun_guid();
    var s = '{"header":{"command":"setparam","result":0,"deviceuuid":"' + devuuid + '","msguuid":"' + msguuid_setparams + '"},"info":{"command":"wirednet","dhcp":' + dhcp + ',"wiredip":"' + wiredip + '"}}';
    return s;
}
//设置无线参数
function Fun_createJsonSetWifi(devuuid, ssid, pwd) {
    msguuid_setparams = Fun_guid();
    var s = '{"header":{"command":"setparam","result":0,"deviceuuid":"' + devuuid + '","msguuid":"' + msguuid_setparams + '"},"info":{"command":"wifi","ssid":"' + ssid + '","wifipwd":"' + pwd + '"}}';
    return s;
}
//设置时区
function Fun_createJsonSetTimeZone(devuuid, val) {
    msguuid_setparams = Fun_guid();
    var s = '{"header":{"command":"setparam","result":0,"deviceuuid":"' + devuuid + '","msguuid":"' + msguuid_setparams + '"},"info":{"command":"timezone","timezone":' + val + '}}';
    return s;
}
//设置ntp server
function Fun_createJsonSetNtpServer(devuuid, ntp) {
    msguuid_setparams = Fun_guid();
    var s = '{"header":{"command":"setparam","result":0,"deviceuuid":"' + devuuid + '","msguuid":"' + msguuid_setparams + '"},"info":{"command":"ntpserver","ntpserver":"' + ntp + '"}}';
    return s;
}
//ptz

//function Fun_createJsonPtz(devuuid,ptz,ptzvalue)
//ptz 为字符串 stop/left/right/up/down/leftup/leftdown/rightup/rightdown/positionset/postioncall
function Fun_createJsonPtz(devuuid, ptz) {
    msguuid_setparams = Fun_guid();
    var s = '{"header":{"command":"ptz","result":0,"deviceuuid":"' + devuuid + '","msguuid":"' + msguuid_setparams + '"},"info":{"ptz":"' + ptz + '","ptzvalue":' + 0 + '}}';
    return s;
}































