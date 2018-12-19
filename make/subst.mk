iterator=$(foreach v,$1,$(warning $v='$($v)'))

var0=$(subst a,b, aab)
var1=$(subst a,b,aab)
var2=$(subst a, b,aab)
var3=$(subst c,d,aab)
$(call iterator,var0 var1 var2 var3 MAKEFILE_LIST)

define showargs
$(warning $$1=[$1] $$2=[$2])
endef
define rwildcard
$(warning [$(subst *,%,%$2)])
endef
$(call showargs, project/, *.mk)
$(call rwildcard, project/,*.mk)
$(call rwildcard,project/, *.mk)
$(call rwildcard,,$(strip $(MAKEFILE_LIST)))
$(warning $(wildcard sub0/*))
#* means 0 or more chars
$(warning $(wildcard sub0/*c))
platform=plat/rockchip/rk3368/platform.mk \
plat/rockchip/rk3399/platform.mk \
plat/rockchip/rk3328/platform.mk \
plat/mediatek/mt8173/platform.mk \
plat/mediatek/mt6795/platform.mk \
plat/nvidia/tegra/platform.mk \
plat/arm/board/fvp/platform.mk \
plat/arm/board/juno/platform.mk \
plat/xilinx/zynqmp/platform.mk \
plat/hisilicon/hikey960/platform.mk \
plat/hisilicon/poplar/platform.mk \
plat/hisilicon/hikey/platform.mk \
plat/socionext/uniphier/platform.mk \
plat/qemu/platform.mk \
plat/rpi3/platform.mk \
platform.mk %platform.mk %%platform.mk *platform.mk

#https://www.gnu.org/software/make/manual/html_node/Wildcard-Examples.html: * in variable definition will not be expanded
#so the above *platform is just a string of '*platform.mk'
#* can be used in:prerequisite/target/recipe, $(wildcard ...)

$(warning $(filter %platform.mk,$(platform)))
$(warning $(filter platform.mk%,$(platform)))
#% in pattern can only occur once, the later is recognized as a plain character %
$(warning $(filter %%platform.mk,$(platform)))
$(warning $(filter *platform.mk,$(platform))) #* in pattern is explained as plain *, not a wildcard
#the following will return nothing!
$(warning $(filter %tegra,$(platform)))
$(warning $(filter %tegra*,$(platform)))
#the following 2 will return nothing!(% canonly exist once!)
$(warning $(filter %tegra%,$(platform)))
$(warning $(filter %tegra/platform.%,$(platform)))
$(warning $(filter %tegra/platform.mk,$(platform)))

.PHONY:all
all:
