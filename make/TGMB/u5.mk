#TO TEST:
#https://stackoverflow.com/questions/33159342/why-dont-environment-variables-work-in-shell-commands
#bugs: http://savannah.gnu.org/bugs/?func=detailitem&item_id=10593

export HELLO=Hello,world
# the env used by $(shell) is exactly the same as the env of make's parent process, ie the process which launched make,
# which has no $HELLO defined
$(info $(shell echo $$$$ $(HELLO):$$HELLO))
all:
	@echo $(HELLO)
	@echo $(shell echo $$HELLO)
	@echo $$HELLO