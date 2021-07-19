all:start.o
start.o:start.S
	gcc -c $< -o $@
include .depend

.depend:start.S
	rm -rf $@
	g=$(@:.S=.o);gcc -M $< -MQ $$g $< >>$@

$(warning Makefile finish)