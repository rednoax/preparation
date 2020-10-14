.PHONY:all
all:main.out long_string.out fgets.out
main.out:blob.c utils.c blobmsg.c main.c
	gcc -m32 -Wall -g $^ -o $@
long_string.out:blob.c utils.c blobmsg.c long_string.c
	gcc -m32 -Wall -g $^ -o $@
fgets.out:fgets.c
	gcc -m32 -Wall -g $^ -o $@
