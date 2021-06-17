#outside the recipe lines, \\n=>a single space. And the following is deleted:\
 all whitespace after/before \\n. all consecutive \\n after \\n

#var is ',\t<space>\\n\\n\t<space>\\n<space>\t\\n<space><space><space>,' \
 and its final value is ',<space>,'
var=,	 \
\
	 \
	\
   ,
$(info [$(var)])#[, ,]
all: