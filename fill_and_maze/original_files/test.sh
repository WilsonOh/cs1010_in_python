#!/bin/bash
function summary() {
  IFS=' ' read -r -a array <<< "$g_failed_cases"
  begin=${array[0]}
  end=${array[0]}
  i=1
  len=${#array[@]}
  while [ $i -lt $len ]; do
    next=${array[$i]}
    prev=${array[$((i - 1))]}
    if [ $next -ne $((prev + 1)) ]; then
      end=$prev
      if [ $begin -eq $end ]; then
        g_summary=" $g_summary $begin"
      else
        g_summary=" $g_summary $begin-$end"
      fi
      begin=$next
    fi
    end=$next
    i=$((i + 1))
  done
  if [ $begin -eq $end ]; then
    g_summary=" $g_summary $begin"
  else
    g_summary=" $g_summary $begin-$end"
  fi
}

function control_c() {
	if [ -e $out ]
	then
		rm -f $out
	fi
}

function make_temp() {
	if [ $(uname) == "Darwin" ]; then
		out=$(mktemp -t $PROG)
	else
		out=$(mktemp --suffix=$PROG)
	fi
}

trap control_c INT

PROG=$1
if [ ! -e $PROG ]
then
	echo "$PROG does not exist.  Have you compiled it with make?"
	exit 1
fi

num_failed=0
i=1
g_failed_cases=""
g_summary=""
while true
do
	if [ -e inputs/$PROG.$i.in ] && [ -e outputs/$PROG.$i.out ]
	then
		#out=$(mktemp --suffix=$PROG)
		make_temp
		./$PROG < inputs/$PROG.$i.in | head -c 100MB > $out
		status="$?"
		if [ "$status" -ne "0" ]
		then
			echo "$PROG: return non-zero status $status for test case $i"
			# cat inputs/$PROG.$i.in
			num_failed=$((num_failed + 1))
		else 
			if [ -e $out ] 
			then
				if [ `diff -w $out outputs/$PROG.$i.out | wc -l` -ne 0 ] 
				then
					#echo "$PROG: incorrect output for test case $i "
					g_failed_cases="$g_failed_cases $i"
					#cat inputs/$PROG.$i.in
					num_failed=$((num_failed + 1))
				fi
				rm -f $out
			else
				echo "$PROG: cannot find output file. Execution interrupted?"
				num_failed=$((num_failed + 1))
			fi
		fi
		i=$((i + 1))
	else
		break
	fi
done

if [ $i -eq 1 ] 
then
	echo "$PROG: no test cases found"
elif [ $num_failed -eq 0 ] 
then
	echo "$PROG: passed"
else
	summary
	echo "$PROG: incorrect output for $num_failed test case(s):$g_summary"
fi
# vim:noexpandtab:sw=4:ts=4
