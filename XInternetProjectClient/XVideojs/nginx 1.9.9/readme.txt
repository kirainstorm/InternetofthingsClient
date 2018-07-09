http://redstarofsleep.iteye.com/blog/2123752

删除文件夹 rm -rf 目录名字
拷贝文件 
sudo cp -f /home/rs/vmrs/nginx.conf /usr/local/nginx/conf/nginx.conf
sudo cp -f /home/rs/vmrs/nginx-mysql /etc/pam.d/nginx-mysql


查看监听端口  netstat -ltn

启动应用程序 sudo /usr/local/nginx/sbin/nginx

停止应用程序     sudo pkill nginx

//-------------------------------------------
系统版本
ubuntu 14.04LTS 64位

sudo apt-get update
sudo apt-get install openssh-server

23.99.109.56  : 22
vmrs  Vmrs123456789




//-------------------------------------------
下载SecureCRT ，SSHSecureShellClient 用于登陆服务器和传输文件

给服务器安装C++编译器 
sudo apt-get update
sudo apt-get install g++
sudo apt-get install unzip
sudo apt-get install make

sudo apt-get install libpam0g-dev
sudo apt-get install libpam-mysql
sudo apt-get install mysql-server-5.5   //安装的时候配置密码vmrs1234


下载pcre，zlib，openssl，nginx ，nginx-rtmp-module



pcre  			http://www.pcre.org/    https://sourceforge.net/projects/pcre/files/
zlib  			http://www.zlib.net/
openssl 		http://sourceforge.net/projects/openssl/?source=directory
nginx 			http://nginx.org/en/download.html
nginx-rtmp-module 	https://github.com/arut/nginx-rtmp-module

下载ngx_http_auth_pam_module

ngx_http_auth_pam_module http://web.iti.upv.es/~sto/nginx/          #权限验证使用mysql

//-------------------------------------------
传输pcre，zlib，openssl，nginx ，nginx-rtmp-module到服务器的 /home/rs/vmrs下
ls

tar -xzvf nginx-1.9.9.tar.gz 
tar -xzvf zlib-1.2.8.tar.gz
tar -xzvf openssl-1.0.1h.tar.gz
tar -xzvf pcre-8.37.tar.gz
tar -xzvf ngx_http_auth_pam_module-1.4.tar.gz


unzip pcre2-10.20.zip
unzip nginx-rtmp-module-master.zip

//-------------------------------------------
cd nginx-1.9.9
make

./configure --prefix=/usr/local/nginx --with-pcre=/home/rs/vmrs/pcre-8.37 --with-zlib=/home/rs/vmrs/zlib-1.2.8 --with-openssl=/home/rs/vmrs/openssl-1.0.1h --add-module='/home/rs/vmrs/ngx_http_auth_pam_module-1.4 /home/rs/vmrs/nginx-rtmp-module-master'

./configure --prefix=/usr/local/nginx --with-pcre=/home/rs/pcre-8.37 --with-zlib=/home/rs/zlib-1.2.8 --with-openssl=/home/rs/openssl-1.0.1h --add-module='/home/rs/nginx-rtmp-module-master'


make

sudo make install




//-------------------------------------------
//##############没有成功。。。

nginx+pam+mysql实现基于数据库的用户认证
配置mysql
mysql这块需要配置的不多，建一个库、一个表来存储密码，然后配置好访问mysql的访问账号，就可以了：

sudo mysql -u root -p

exit

create database pam
use pam
create table user(userid varchar(16),passwd varchar(50),primary key (userid));      ///// type=innodb default charset=utf8;
GRANT SELECT ON pam.* TO pamuser@localhost IDENTIFIED BY '123456';


执行完上面语句之后，得到：
引用库：pam
表：user
字段：userid, passwd
访问账号：pamuser
访问密码：123456

2)配置pam-mysql

在/etc/pam.d/下建一个文件nginx-mysql

/etc/pam.d/nginx-mysql


内容：

auth required /lib/security/pam_mysql.so user=pamuser passwd=123456 host=localhost db=pam table=user usercolumn=userid passwdcolumn=passwd crypt=2 
account required /lib/security/pam_mysql.so user=pamuser passwd=123456 host=localhost db=pam table=user usercolumn=userid passwdcolumn=passwd crypt=2


这个文件的文件名可以自己取，只要和后面的nginx配置对应上即可。配置文件里把mysql的配置对应拷进去，两句话除了开头的一个单词auth和account外，都是一样的。

配置中的crypt：
引用0=plain: 明码
1=Y: crypt()函数
2=mysql: mysql的password()函数
3=md5: mysql的md5()函数

详细配置可见：

http://pam-mysql.sourceforge.net/Documentation/package-readme.php

3)配置nginx

引用server {
listen 80;
server_name pam.ws.netease.com;

location / {
auth_pam "mysql pam";
auth_pam_service_name "nginx-mysql";
root /data/html/;
}

}

auth_pam：提示语
auth_pam_service_name：/etc/pam.d/下对应文件的名字

测试：

在mysql插入一条记录：

insert into user values ('abc',password('12345'));


insert into user values ('abcd','12345');

//-------------------------------------------

安装完成后,打开Nginx的配置文件nginx.conf进行配置
//开启服务器的端口 1935，8080（nginx默认的http监听端口）

首先在里面加入rtmp的配置

rtmp {
    server {
        listen 1935;
  
        application myapp {
            live on;
        }
       application hls {
            live on;
           hls on;
            hls_path /tmp/hls;
        }
    }
}
然后,针对hls,还需要在http里面增加一个location配置
location /hls {
            types {
                application/vnd.apple.mpegurl m3u8;
                video/mp2t ts;
            }
            root /tmp;
            add_header Cache-Control no-cache;
}
这是一个最简单,最基础的配置, rtmp监听1935端口,如果是hls的话用hls on开启hls,并且为hls设置一个临时文件目录hls_path /tmp/hls; 其它更高级的配置可以参看nginx-rtmp-module的readme,里面有比较详细的介绍其它的配置,并且它还提供了一个通过JWPlayer在网页上播放的例子.

 
//-------------------------------------------
cd /usr/local/nginx/sbin


sudo cp -f /home/rs/vmrs/nginx.conf /usr/local/nginx/conf/nginx.conf


sudo ./nginx




保存完配置文件后,启动nginx,通过netstat -ltn命令可以看到增加了一个1935端口的监听.8080是nginx默认的http监听端口.

然后用ffmpeg推流到nginx:

第一个是推到了上面配置的myapp上:



Java代码 复制代码 收藏代码
ffmpeg -re -i "D:\download\film\aqgy\02.mp4" -vcodec libx264 -vprofile baseline -acodec aac -ar 44100 -strict -2 -ac 1 -f flv -s 1280x720 -q 10 rtmp://krscloud.cloudapp.net:1935/  myapp/test1  


第二个推送到hls上:



Java代码 复制代码 收藏代码
ffmpeg -re -i "D:\download\film\aqgy\02.mp4" -vcodec libx264 -vprofile baseline -acodec aac -ar 44100 -strict -2 -ac 1 -f flv -s 1280x720 -q 10 rtmp://ip:1935/hls/test2  


现在我们的流媒体服务器有两个实时流了,一个是rtmp的,另一个是hls的,用流媒体播放器播放一下,流媒体播放器可以用vlc也可以用ffmpeg带的ffplay.手机也是可以播放的.

 

上面这两个流的地址分别是:

第一个就是推送的地址: rtmp://serverIp:1935/myapp/test1

第二个是HTTP地址: http://serverIp:8080/hls/test2.m3u8




rtmp://192.168.7.148:1935/myapp/test1


