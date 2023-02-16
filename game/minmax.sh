#!/bin/bash

# SanyaSho: ye, im lazy

WHORE=tf

for i in $(find {client,shared,server}/$WHORE/ ); do 

if grep -q " min(" $i; then
	echo $i
	sed -i 's/ min(/ MIN(/g' $i
fi

if grep -q " max(" $i; then
	echo $i
	sed -i 's/ max(/ MAX(/g' $i
fi

done
