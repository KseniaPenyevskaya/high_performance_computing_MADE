#!/bin/bash
file="linux"
if [ -f "$file" ]
then
	echo "course"
else
	echo "very easy"
	echo "course is easy" > $file
fi
