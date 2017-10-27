iterate=$(foreach v, $1, $(warning $v='$($v)'))

#SHELL default value is /bin/sh, not the environment's SHELL=/bin/bash 
$(warning $(SHELL))
SHELL:=/bin/bash
#then it will become /bin/bash
$(warning $(SHELL))

#findstring usage
VAR0=$(findstring a, a b)
VAR1=$(findstring a,ab)
VAR2=$(findstring part, partial end)
VAR3=$(findstring partial, part end)
vars=VAR0 VAR1 VAR2 VAR3
$(call iterate, $(vars))

#MAKE_VERSION is 4.2.1
$(warning MAKE_VERSION='$(MAKE_VERSION)')

#########################################
#[root@lucia make]# expr 4.2.1>=3.81
#[root@lucia make]# expr 4.2.1>= 3.81
#expr: syntax error
#[root@lucia make]# expr 4.2.1 >= 3.81
#expr: syntax error
#[root@lucia make]# expr 4.2.1 \>= 3.81
#1
#########################################
#bash's "expr 4.2.1 \>=3.81" emit 1, please note there should be a space after \>=
result=$(shell expr $(MAKE_VERSION) \>= 3.81)
$(warning expr $(MAKE_VERSION)>=3.81 show "$(result)")

#$(filter/filter-out pattern, text) both pattern and text can contains spaces
pattern:=clean clobber dataclean
text:=menuconfig all clean distclean
$(warning $(filter $(pattern),$(text)))
$(warning $(filter-out $(pattern),$(text)))

#words will be 1
path1=/root/linux_android_board_support_package_vla.br.1.2.7-01010-8x16.0-4/APQ8016_410C_LA.BR.1.2.7-01010-8x16.0_6.0.1_Marsh_Mallo_P2
#$words will be 4
path2=/d/Program Files (x86)/FileZilla Server

$(warning $(words $(path1)))
$(warning $(words $(path2)))

java_version_str:=$(shell java -version 2>&1)
javac_version_str:=$(shell javac -version 2>&1)
java_version:=$(shell echo '$(java_version_str)'|grep '^java .*[ "]1\.7[\. "$$]')
javac_version:=$(shell echo '$(javac_version_str)'|grep '[ "]1\.7[\. "$$]')
$(warning $(java_version) $(javac_version))

.PHONY:all
all:;
