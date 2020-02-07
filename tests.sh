#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $DIR

retval=0

for i in test/*
do
	$1 -b 11111111 -t $i/target.* -s $i/secret.* -o $i/hidden -e --size-header
	$1 -b 11111111 -t $i/hidden -o $i/shown -d --size-header
	diff $i/secret.* $i/shown
	retval=$(( $retval + $? ))
done

exit $retval
