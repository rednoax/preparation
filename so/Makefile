#{static0,static1}.out are to check if there's any difference in xx.out linked with different archive files\
that derives from a single .o by `ld -r -o` and multiple .o respectively
#md5sum result: static0.a != static1.a; static0.out == static1.out
objs = a.o b.o
ars=static0.a static1.a
outs=$(patsubst %.a,%.out,$(ars))

all: $(outs)

%.out:main.o %.a
	gcc -o $@ $^
	md5sum $@

$(word 1,$(ars)):$(objs)
	ld -r -o $@.o $^
	ar rcs $@ $@.o; md5sum $@

$(word 2,$(ars)):$(objs)
	ar rcs $@ $^; md5sum $@

%.o: %.c
	gcc -Wall -c $< -o $@
clean:
	rm -rf *.o *.lst *.a