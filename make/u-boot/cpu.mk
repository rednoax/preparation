#$ rm -rf .depend &&make -f cpu.mk\
rules.mk:3: rules.mk included\
cpu.mk:4: cpu.mk included<--even rules.mk has told how to generate .dpend but "sinclude .depend" will not try to generate .depend until the whold cpu.mk is parsed\
echo '$(warning .depended included)'>.depend\
rules.mk:3: rules.mk included\
.depend:1: .depended included\
cpu.mk:4: cpu.mk included\
make: 'all' is up to date.

#about the 3rd line comments of the above:\
why sinclude will not generate .depend before finishing parsing of the whole cpu.mk?\
suppose there is different rules to generate .depend before and after "sinclude .depend".\
the former is a pattern rules while the later is a specific rule. So the later should be used,\
that's why the process of generating .depend will not be started until the whole makefile,\
ie cpu.mk, is parsed.\
to summarize: if the included file does not exist, its generation will not be started until finishing\
parsing the whole makefile.
all:;
include rules.mk
sinclude .depend
$(warning cpu.mk included)