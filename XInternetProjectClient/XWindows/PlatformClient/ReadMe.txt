1：使用私有协议直接连设备
2：使用私有协议链接平台
3：使用国标链接平台






1:

//本地添加
CREATE TABLE tb_device(
id integer PRIMARY KEY, 
_p2p integer default 0,
_name varchar(64), 
_param varchar(128), 
_ver varchar(64),
_ip varchar(64), 
_port integer, 
_user varchar(64), 
_pwd varchar(64),
_serial varchar(128));

//从服务器获取
CREATE TABLE tb_device_s(
id integer PRIMARY KEY, 
devid_s integer,
oemid_s integer, 
areaid_s integer, 
serverid_s integer, 
accountid_s integer,
media_ip varchar(64), 
media_port integer, 
channel_count integer, 
_name varchar(64), 
_param varchar(128), 
_ver varchar(64),
_ip varchar(64), 
_port integer, 
_user varchar(64), 
_pwd varchar(64), 
_serial varchar(128));

CREATE TABLE tb_user(_id INTEGER PRIMARY KEY, _user varchar(64), _pwd varchar(64) ,_param varchar(255));

CREATE TABLE tb_config(_id INTEGER PRIMARY KEY, _key varchar(64)  , _val INT);
INSERT INTO tb_config(_key,_val) VALUES("LANGUAGE",0);
INSERT INTO tb_config(_key,_val) VALUES("SERVER_ENABLE",0);
INSERT INTO tb_config(_key,_val) VALUES("SPLIT_SCREEN",2);

//标题栏背景色
INSERT INTO tb_config(_key,_val) VALUES("BG_CORLOR_TR",30);
INSERT INTO tb_config(_key,_val) VALUES("BG_CORLOR_TG",30);
INSERT INTO tb_config(_key,_val) VALUES("BG_CORLOR_TB",30);
//窗体背景色
INSERT INTO tb_config(_key,_val) VALUES("BG_CORLOR_R",30);
INSERT INTO tb_config(_key,_val) VALUES("BG_CORLOR_G",30);
INSERT INTO tb_config(_key,_val) VALUES("BG_CORLOR_B",30);
//视频窗口标题背景色
INSERT INTO tb_config(_key,_val) VALUES("BG_T_CORLOR_R",62);
INSERT INTO tb_config(_key,_val) VALUES("BG_T_CORLOR_G",62);
INSERT INTO tb_config(_key,_val) VALUES("BG_T_CORLOR_B",62);
//视频窗口标题字体颜色
INSERT INTO tb_config(_key,_val) VALUES("BG_TC_CORLOR_R",235);
INSERT INTO tb_config(_key,_val) VALUES("BG_TC_CORLOR_G",235);
INSERT INTO tb_config(_key,_val) VALUES("BG_TC_CORLOR_B",235);



SPLIT_SCREEN：
case 0:m_nShow = 1;break;
case 1:m_nShow = 4;break;
case 2:m_nShow = 9;break;
case 3:m_nShow = 16;break;
case 4:m_nShow = 25;break;
case 5:m_nShow = 36;break;
case 6:m_nShow = 49;break;
case 7:m_nShow = 64;break;


2: