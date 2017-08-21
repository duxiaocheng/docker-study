###############################################################################
# Dockerfile to build docker_demo container images
# Based on CentOS
# Use "docker history <image>" can get this operation
# "docker build -t chasond/demo ."
#
# https://docs.docker.com/engine/reference/builder/#maintainer-deprecated
###############################################################################

# Pull base image: FROM <image>:<tag>
FROM centos:7

# MAINTAINER <name> 
MAINTAINER chason.du@nokia-sbell.com

# RUN <command> (the command is run in a shell - `/bin/sh -c`)  
# RUN ["executable", "param1", "param2" ... ]  (exec form)
# Create the default data directory
RUN mkdir -p /data/

# VOLUME ["<mountpoint>"]
# VOLUME <mountpoint1> <mountpoint2>
# Use "docker inspect <container-id>" to get the mount info(src-host/dst-container)
# The default host directory is /var/lib/docker/volumes/xxxxxxxxx/_data
# We can also use "docker run -v host-dir:docker-dir" to specify this mount
VOLUME /data/

# ENV <key> <value>
# ENV JAVA_HOME /path/to/java/dirent
ENV MY_DATA_DIR /data

# Copy file
ADD docker_demo /usr/bin/

# EXPOSE <port> [<port>...]
#
# The EXPOSE instruction informs Docker that the container listens on the specified network ports at runtime.
# EXPOSE does not make the ports of the container accessible to the host.
# To do that, you must use either the -p flag to publish a range of ports or the -P flag to publish all of the exposed ports.
# You can expose one port number and publish it externally under another number.
#
# To set up port redirection on the host system, see using the -P flag.
# The Docker network feature supports creating networks without the need to expose ports within the network,
# for detailed information see the overview of this feature).
#
# Refer to "docker run -p ip:hostPort:containerPort[/tcp] <image>"
# EXPOSE 55555

# ENTRYPOINT ["executable", "param1", "param2"] (like an exec, the preferred form)  
# ENTRYPOINT command param1 param2 (as a shell) 

# CMD ["executable","param1","param2"] (like an exec, this is the preferred form)  
# CMD command param1 param2 (as a shell)
# CMD ["param1","param2"] (as default parameters to ENTRYPOINT)
CMD ["/usr/bin/docker_demo"]


