http://redstarofsleep.iteye.com/blog/2123752

ɾ���ļ��� rm -rf Ŀ¼����
�����ļ� 
sudo cp -f /home/rs/vmrs/nginx.conf /usr/local/nginx/conf/nginx.conf
sudo cp -f /home/rs/vmrs/nginx-mysql /etc/pam.d/nginx-mysql


�鿴�����˿�  netstat -ltn

����Ӧ�ó��� sudo /usr/local/nginx/sbin/nginx

ֹͣӦ�ó���     sudo pkill nginx

//-------------------------------------------
ϵͳ�汾
ubuntu 14.04LTS 64λ

sudo apt-get update
sudo apt-get install openssh-server

23.99.109.56  : 22
vmrs  Vmrs123456789




//-------------------------------------------
����SecureCRT ��SSHSecureShellClient ���ڵ�½�������ʹ����ļ�

����������װC++������ 
sudo apt-get update
sudo apt-get install g++
sudo apt-get install unzip
sudo apt-get install make

sudo apt-get install libpam0g-dev
sudo apt-get install libpam-mysql
sudo apt-get install mysql-server-5.5   //��װ��ʱ����������vmrs1234


����pcre��zlib��openssl��nginx ��nginx-rtmp-module



pcre  			http://www.pcre.org/    https://sourceforge.net/projects/pcre/files/
zlib  			http://www.zlib.net/
openssl 		http://sourceforge.net/projects/openssl/?source=directory
nginx 			http://nginx.org/en/download.html
nginx-rtmp-module 	https://github.com/arut/nginx-rtmp-module

����ngx_http_auth_pam_module

ngx_http_auth_pam_module http://web.iti.upv.es/~sto/nginx/          #Ȩ����֤ʹ��mysql

//-------------------------------------------
����pcre��zlib��openssl��nginx ��nginx-rtmp-module���������� /home/rs/vmrs��
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
//##############û�гɹ�������

nginx+pam+mysqlʵ�ֻ������ݿ���û���֤
����mysql
mysql�����Ҫ���õĲ��࣬��һ���⡢һ�������洢���룬Ȼ�����ú÷���mysql�ķ����˺ţ��Ϳ����ˣ�

sudo mysql -u root -p

exit

create database pam
use pam
create table user(userid varchar(16),passwd varchar(50),primary key (userid));      ///// type=innodb default charset=utf8;
GRANT SELECT ON pam.* TO pamuser@localhost IDENTIFIED BY '123456';


ִ�����������֮�󣬵õ���
���ÿ⣺pam
��user
�ֶΣ�userid, passwd
�����˺ţ�pamuser
�������룺123456

2)����pam-mysql

��/etc/pam.d/�½�һ���ļ�nginx-mysql

/etc/pam.d/nginx-mysql


���ݣ�

auth required /lib/security/pam_mysql.so user=pamuser passwd=123456 host=localhost db=pam table=user usercolumn=userid passwdcolumn=passwd crypt=2 
account required /lib/security/pam_mysql.so user=pamuser passwd=123456 host=localhost db=pam table=user usercolumn=userid passwdcolumn=passwd crypt=2


����ļ����ļ��������Լ�ȡ��ֻҪ�ͺ����nginx���ö�Ӧ�ϼ��ɡ������ļ����mysql�����ö�Ӧ����ȥ�����仰���˿�ͷ��һ������auth��account�⣬����һ���ġ�

�����е�crypt��
����0=plain: ����
1=Y: crypt()����
2=mysql: mysql��password()����
3=md5: mysql��md5()����

��ϸ���ÿɼ���

http://pam-mysql.sourceforge.net/Documentation/package-readme.php

3)����nginx

����server {
listen 80;
server_name pam.ws.netease.com;

location / {
auth_pam "mysql pam";
auth_pam_service_name "nginx-mysql";
root /data/html/;
}

}

auth_pam����ʾ��
auth_pam_service_name��/etc/pam.d/�¶�Ӧ�ļ�������

���ԣ�

��mysql����һ����¼��

insert into user values ('abc',password('12345'));


insert into user values ('abcd','12345');

//-------------------------------------------

��װ��ɺ�,��Nginx�������ļ�nginx.conf��������
//�����������Ķ˿� 1935��8080��nginxĬ�ϵ�http�����˿ڣ�

�������������rtmp������

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
Ȼ��,���hls,����Ҫ��http��������һ��location����
location /hls {
            types {
                application/vnd.apple.mpegurl m3u8;
                video/mp2t ts;
            }
            root /tmp;
            add_header Cache-Control no-cache;
}
����һ�����,�����������, rtmp����1935�˿�,�����hls�Ļ���hls on����hls,����Ϊhls����һ����ʱ�ļ�Ŀ¼hls_path /tmp/hls; �������߼������ÿ��Բο�nginx-rtmp-module��readme,�����бȽ���ϸ�Ľ�������������,���������ṩ��һ��ͨ��JWPlayer����ҳ�ϲ��ŵ�����.

 
//-------------------------------------------
cd /usr/local/nginx/sbin


sudo cp -f /home/rs/vmrs/nginx.conf /usr/local/nginx/conf/nginx.conf


sudo ./nginx




�����������ļ���,����nginx,ͨ��netstat -ltn������Կ���������һ��1935�˿ڵļ���.8080��nginxĬ�ϵ�http�����˿�.

Ȼ����ffmpeg������nginx:

��һ�����Ƶ����������õ�myapp��:



Java���� ���ƴ��� �ղش���
ffmpeg -re -i "D:\download\film\aqgy\02.mp4" -vcodec libx264 -vprofile baseline -acodec aac -ar 44100 -strict -2 -ac 1 -f flv -s 1280x720 -q 10 rtmp://krscloud.cloudapp.net:1935/  myapp/test1  


�ڶ������͵�hls��:



Java���� ���ƴ��� �ղش���
ffmpeg -re -i "D:\download\film\aqgy\02.mp4" -vcodec libx264 -vprofile baseline -acodec aac -ar 44100 -strict -2 -ac 1 -f flv -s 1280x720 -q 10 rtmp://ip:1935/hls/test2  


�������ǵ���ý�������������ʵʱ����,һ����rtmp��,��һ����hls��,����ý�岥��������һ��,��ý�岥����������vlcҲ������ffmpeg����ffplay.�ֻ�Ҳ�ǿ��Բ��ŵ�.

 

�������������ĵ�ַ�ֱ���:

��һ���������͵ĵ�ַ: rtmp://serverIp:1935/myapp/test1

�ڶ�����HTTP��ַ: http://serverIp:8080/hls/test2.m3u8




rtmp://192.168.7.148:1935/myapp/test1


