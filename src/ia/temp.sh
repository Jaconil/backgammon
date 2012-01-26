#!/bin/bash

i=0;

while [ $i -lt 15 ] ; do
	./a.out > test.txt
	i=$(($i + 1))
done
