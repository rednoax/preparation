#How to test this .mk: set -x;echo -e "$$:$-\n###";make -f u8.mk
.PHONY:all
$(info $(shell echo $$$$:$$-))#1/3.$- in this make's process != parent process that has `set -x`.Besides, child process running `bash`\
 also doesn't derive parent's `set -x`;but child process lauched by (..) will inherit parent's set option!
all:
#2/3.each line is a child process itself, the following 2 lines are 2 child processes respectively \
so the `pwd` of each is not different since they're in different child process
	@cd /tmp;echo "$$$$:`pwd`"
	@echo "$$$$:`pwd`"
#3/3.`set -x` will only effect the child process running the following one line, including child process in (...)
	@set -x;echo "$$$$:$$-";(echo 1.5)
# and this line will not be affected by `set -x` for it is in another child process.
	@echo -ne "$$$$:$$-:\t";echo 2