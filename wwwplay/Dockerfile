###############################################################################
# WWWPlayer Dockerfile
# Usage:
# docker build -t wwwplay .
# docker run -d -v /home/chasond/wwwplay/:/wwwplay -v /home/chasond/opt/:/opt -p 50000:80 wwwplay
# Then we can access the wwwplay: http://10.13.0.187:50000/wwwplay.html
#
# Make sure that the host has wwwplay(4G size) and opt/ti(8G size)folder
###############################################################################

# Due to wine, and there is no i686 version RPM package for centos7
FROM centos:6

MAINTAINER chason.du@nokia-sbell.com

RUN mkdir -p /wwwplay/

ADD runproxy /root/runproxy
ADD run.sh /usr/bin/run.sh

WORKDIR /root/
RUN source /root/runproxy && yum -y install httpd perl perl-CGI perl-TermReadKey which
RUN source /root/runproxy && yum -y install epel-release
# for /lib/ld-linux.so.2 and libgcc_s.so.1 (yum provides "libgcc_s.so.1")
#RUN source /root/runproxy && yum -y install glibc-2.17-157.el7_3.5.i686 libgcc-4.8.5-11.el7.i686
RUN source /root/runproxy && yum -y install wine

COPY httpd.conf /etc/httpd/conf/httpd.conf

RUN mkdir -p /home/apache
RUN chown apache /home/apache

VOLUME /wwwplay/
VOLUME /opt/
EXPOSE 80

CMD ["/usr/bin/run.sh"]


