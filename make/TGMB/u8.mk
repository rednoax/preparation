#How to test this .mk: set -x;echo -e "$$:$-\n###";make -f u8.mk
.PHONY:all
$(info $(shell echo $$$$:$$-))#$- in this make's process != parent process that has `set -x`
all:
#each line is a child process itself, the following 2 lines are 2 child processes respectively \
so the `pwd` of each is not different since they're in different child process
	@cd /tmp;echo "$$$$:`pwd`"
	@echo "$$$$:`pwd`"
#`set -x` will only effect the child process running the following one line
	@set -x;echo 1
# and this line will not be affected by `set -x` for it is in another child process.
	@echo -ne "$$$$:$$-:\t";echo 2