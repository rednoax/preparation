ifndef CROSS_COMPILE
CROSS_COMPILE=arm-linux-
endif
obj=start.o main.o
OUT=final
.PHONY:all
all:$(OUT)
$(OUT):$(obj)
	$(CROSS_COMPILE)ld -T test.lds $(obj) -o $@
%.o:%.S
	$(CROSS_COMPILE)gcc -c -Wall $< -o $@
%.o:%.c
	$(CROSS_COMPILE)gcc -c -Wall $< -o $@

clean:
	rm -rf $(obj) $(OUT)
