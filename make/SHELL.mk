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
########################################################
__MAKECMDGOALS=msm8952 $$1 $$2
target:=$(firstword $(__MAKECMDGOALS))
$(warning $(target) from "$(__MAKECMDGOALS)")
#an unexist var's firstword is empty
$(warning $(firstword $(__unexist_var)))
#ifeq ($(FOO),), FOO can be undefined or empty
ifeq ($(__unexist),)
$(warning $(wildcard *.mk))
endif
ifndef __unexist
$(warning non-defined "__unexist" conform to ifndef)
endif
__unexist2:=
ifeq ($(__unexist2),)
$(warning "__unexist2" empty, '$(firstword $(__unexist2))')
endif
ifndef __unexist2
$(warning empty "__unexist2" conforms to ifndef)
endif
#ifneq is similar
ifneq ($(__unexist),)
else
$(warning not defined:$(__unexist))
endif
ifneq ($(__unexist2),)
else
$(warning empty:$(__unexist2))
endif
###########################
#use the following command line to test!
#make -f SHELL.mk msm8952 DEBUG=0 BOOTLOADER_OUT=out/target/product/A10_32/obj/EMMC_BOOTLOADER_OBJ
.PHONY:all
project-name:=$(firstword $(MAKECMDGOALS))
ifneq ($(project-name),)
ifneq ($(wildcard project/$(project-name).mk),)
do-nothing:=1
$(MAKECMDGOALS) all: make-make make-make-make
make-make:
	PRJECT=$(project-name) $(MAKE) -C project -f $(project-name).mk
make-make-make:
	PRJECT=$(project-name) $(MAKE) -f SHELL.mk emmc
endif
endif

ifeq ($(do-nothing),)
VARS=DEBUG BOOTLOADER_OUT
$(call iterate, $(VARS))
endif

emmc_clean:
	@echo $@
emmc_obj:
	@echo $@
#$^ is different from "a|b" and "a b", but the sequence seems no different!
emmc:emmc_clean | emmc_obj
	@echo $@ [$^]
emmc_2:emmc_clean emmc_obj
	@echo $@ [$^]
#######################
undefined_with_prefix:=$(addprefix --second ,$(__undefined))
empty_with_prefix:=$(addprefix --second, $(__unexist2))
$(call iterate, undefined_with_prefix empty_with_prefix)
file1:=/opt/o:p
file2:=/opt
$(warning $(subst :,[,$(file1)))
$(warning $(subst :,[,$(file2)))
