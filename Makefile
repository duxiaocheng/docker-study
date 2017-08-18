all : docker_demo

docker_demo : docker_demo.c
	gcc -g -o $@ $<


