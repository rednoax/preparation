#How to test this .mk: set -x;echo -e "$$:$-\n###";make -f u8.mk
.PHONY:all
$(info $(shell echo $$$$:$$-))#1/4.$- in this make's process != parent process that has `set -x`.Besides, child process running `bash`\
 also doesn't derive parent's `set -x`;but child process lauched by (..) will inherit parent's set option!
all:
#2/4.each line is a child process itself, the following 2 lines are 2 child processes respectively \
so the `pwd` of each is not different since they're in different child process
	@cd /tmp;echo "$$$$:`pwd`"
	@echo "$$$$:`pwd`"
#3/4.`set -x` will only effect the child process running the following one line, including child process in (...);
	@set -x;echo "$$$$:$$-";(echo 1.5);(ls)
# and this line will not be affected by `set -x` for it is in another child process.
	@echo -ne "$$$$:$$-:\t";echo 2
#4/4 echo 3 in the 1st line will run even one line script has encounter error \
 while echo 4 in the 2nd line script will NOT run due to `set -e` means script exits after any error; \
 the 3rd line will not run as the middle recipes has failed and make process will stop running the recipes following the failing line.
	@ls unexist;echo 3
	@set -e;ls unexist;echo 4;
	@echo 5
