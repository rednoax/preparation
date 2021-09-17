vars=MAKEFLAGS

envbeforemakecons:=[$(MAKELEVEL):$(shell printenv|grep $(vars))]
aftermakecons:=[$(MAKELEVEL):$(MAKEFLAGS)]
#MAKEFLAGS's road: \
1. for LN make(N>=1), its parent L(N-1) make AL transfers MAKEFLAGS in LN make program's env list(via C's main(,,char **envp));\
 then LN make cons will add option from LN make's cmd line option, if any.\
2. when LN(N>=1) make prepares MAKEFLAGS for L(N+1) child make, it uses the value of 1, plus its own explicitly value\
 from MAKEFLAGS+=..., if any. The transferring is AL via environment.\
3. for L0 make, it's the same as 1+2, Note in 1 MAKEFLAGS can still be from environment even L0 make has no parent make.\
eg `export MAKEFLAGS=xx;make` or `MAKEFLAGS=xx make`.\
IN BRIEF: child make's env list AL contains MAKEFLAGS transferred from parent make. For any level's make (LN N>=0) the MAKEFLAGS is\
(environment transferred+some command line options,if any+file variable like MAKEFLAGS+=-rR,if any)\
see "make test2" example.
show=$(info $(envbeforemakecons)=>$(aftermakecons)=>[$(MAKELEVEL):$(MAKEFLAGS)])#initial env for make=>phase 1=>phase2
CMD=$(MAKE) -f opt.mk

ifeq (0,1)
#$ VAR=e make -f opt.mk -e \
: 'e'
#$ VAR=e make -f opt.mk\
: 'e -R'
#$ make -f opt.mk VAR=c\
: 'c'
VAR+=-R#-R will not be added to env's value "e", at last VAR's value is e
test:
	: '${VAR}'
endif

ifeq ($(MAKECMDGOALS),test2)
MAKEFLAGS+=-r
test2:
	@printenv|grep MAKEFLAGS$(show)
ifeq ($(MAKELEVEL) , 0)
	$(MAKE) -f opt.mk test2
endif	
#$ MAKEFLAGS=k make -f opt.mk -i test2 \
[0:MAKEFLAGS=k]=>[0:ik]=>[0:ikr]<-initial env has k,make cons adds cmd line option -i,phase 2 add -r from file +=\
MAKEFLAGS=ikr <--child process env list contains the MAKEFLAGS transferred by L0 make\
make -f opt.mk test2 <--this child process is a real make whose env variable MAKECMDGOALS value is ikr\
make[1]: Entering directory '/home/rednoah/opt/preparation/make/TGMB' \
[1:MAKEFLAGS=ikr]=>[1:ikrw]=>[1:ikrw]<-‘-w’ is turned on automatically when you use the ‘-C’ option, and in sub-makes. \
MAKEFLAGS=ikrw <-L2 process, even not make, has been transferred with MAKEFLAGS in its env list\
make[1]: Leaving directory '/home/rednoah/opt/preparation/make/TGMB'
endif

#-s will stop:1.recipe command's display; 2.-w(which enable Entering/Leaving pairs display)\
-e will prevent all2 adding -R to MAKEFLAGS(see above "make test"), so I just use -k to lauch L1 make
all:
	${CMD} -k all2$(show)

ifeq ($(MAKECMDGOALS),all2)
MAKEFLAGS += -R
endif

all2:
	${CMD} -r all3$(show)
all3:
	${CMD} all4$(show)
all4:
	:$(show)

ifeq (0,1)
$ make -f opt.mk <--not in each level, the initial env MAKEFLAGS's value==last level's phase2's MAKEFLAGS's value
[0:]=>[0:]=>[0:]<-no MAKEFLAGS in L0 make's initial env list, phase 1 & phase 2
make -f opt.mk -k all2 <-L1 make launched, -w is added auto
make[1]: Entering directory '/home/rednoah/opt/preparation/make/TGMB' 
[1:MAKEFLAGS=]=>[1:kw]=>[1:kRw]<-L1 make init env has no MAKEFLAGS=>k from cmd line,w is auto added=>-R added in phase1 file variable+=
make -f opt.mk -r all3<-L2 make launched
make[2]: Entering directory '/home/rednoah/opt/preparation/make/TGMB'
[2:MAKEFLAGS=kRw]=>[2:krRw]=>[2:krRw]<-L1's phase 2 value=>-r from cmd line=>phase 2 value==phase 1 value
make -f opt.mk all4<--L3 make launched
make[3]: Entering directory '/home/rednoah/opt/preparation/make/TGMB'
[3:MAKEFLAGS=krRw]=>[3:krRw]=>[3:krRw]<--L3 MAKEFLAGS inherits values of each upper make,including:cmd line option(k from L1,r from L2);explicitly file var+=(R from L1 make phase 1);w is added auto by default
:
make[3]: Leaving directory '/home/rednoah/opt/preparation/make/TGMB'
make[2]: Leaving directory '/home/rednoah/opt/preparation/make/TGMB'
make[1]: Leaving directory '/home/rednoah/opt/preparation/make/TGMB'
endif