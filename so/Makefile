#{s0,s1}.out are to check if there's any difference in xx.out linked with different archive files\
that derives from a single .o by `ld -r -o` and multiple .o respectively
#md5sum result: s0.a != s1.a; \
Both of the following can happen:\
s0.out == s1.out!= mab.out \
s0.out == mab.out != s1.out

objs = a.o b.o
mobjs=main.o
ars=libs0.a libs1.a

#archive out:s0.out s1.out
aouts:=$(subst lib,,$(patsubst %.a,%.out,$(ars)))
#_aouts := $(aouts)
#mab.out from multiple .o
oouts=mab.out

OUTPUT=staticobjs/ sharedobjs/

addp = $(foreach v,$1,$(eval $v := $(addprefix $(OUTPUT),$($v))))
needp = objs ars aouts oouts mobjs
$(call addp, $(needp))
#=======
iter = $(foreach v,$1,$(warning $v=$($v)))
#$(call iter, $(needp))

$(if $(wildcard $(OUTPUT)),,$(shell mkdir -p $(OUTPUT)))

outs=$(aouts) $(oouts)
a2o=$(@:.a=-in.o)
#VPATH=obj
all: $(outs)

$(oouts):$(mobjs) $(objs)
	gcc -o $@ $^
	md5sum $@

$(aouts):$(OUTPUT)%.out:$(mobjs) $(OUTPUT)lib%.a
	gcc -o $@ $^$(if $V,$(warning [$*]))
	md5sum $@

$(word 1,$(ars)):$(objs)
	ld -r -o $(a2o) $^
	ar rcs $@ $(a2o); md5sum $@

$(word 2,$(ars)):$(objs)
	ar rcs $@ $^; md5sum $@

$(OUTPUT)%.o: %.c
	gcc -Wall $-c $< -o $@
clean:
	rm -rf *.o *.lst *.a *.out $(OUTPUT)