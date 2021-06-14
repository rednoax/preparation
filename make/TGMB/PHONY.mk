# sub/ exists and its timestamp is newer than dummy.mk. However, once sub becomes .PHONY, its recipe \
AL runs even its prerequisite is older than the target.\
$ ll -ld sub dummy.mk \
-rw-rw-r-- 1 rednoah rednoah    0 Jun 15 07:00 dummy.mk \
drwxrwxr-x 2 rednoah rednoah 4096 Jun 15 07:03 sub/

.PHONY:sub
sub:dummy.mk
	@echo $@