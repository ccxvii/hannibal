#!/bin/bash
for A in 1 2 3 4
do
	for D in 0 1 2 3 4
	do
		./hannibal $A $D $*
		echo
	done
done
