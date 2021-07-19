#!/bin/bash
# Pitfalls of variables in a subshell.
echo "current pid:$$==$BASHPID PPID:$PPID"
set|grep nautilus
set -o|grep xtrace
set -x
set -o|grep xtrace
outer_variable=outer
echo
echo "outer_variable = $outer_variable"
echo
(
echo "current pid:$$!=$BASHPID PPID:$PPID"
set -o|grep xtrace
# Begin subshell
echo "outer_variable inside subshell = $outer_variable"
inner_variable=inner # Set
echo "inner_variable inside subshell = $inner_variable"
outer_variable=inner # Will value change globally?
echo "outer_variable inside subshell = $outer_variable"
# Will 'exporting' make a difference?
 export inner_variable
 export outer_variable
# sleep 30
# Try it and see.
# End subshell
)
echo
echo "inner_variable outside subshell = $inner_variable" # Unset.
echo "outer_variable outside subshell = $outer_variable" # Unchanged.
echo
#sleep 30
exit 0
# What happens if you uncomment lines 19 and 20?
# Does it make a difference?