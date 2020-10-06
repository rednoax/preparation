.PHONY:all
all:main.out long_string.out
main.out:blob.c utils.c blobmsg.c main.c
	gcc -m32 -Wall -g $^ -o $@
long_string.out:blob.c utils.c blobmsg.c long_string.c
	gcc -m32 -Wall -g $^ -o $@
