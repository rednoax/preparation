(ls unexist)
echo $?
(ls *.c)
echo $?
echo "----"
vars=`ls *.c *.t`
echo "===$vars=="
