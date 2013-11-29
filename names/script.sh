#!/bin/bash

i=0
while read name; do
    filename=${i}.png
    convert -background transparent -fill black -font Sans -strokewidth '0.9' \
    -stroke white -size 500x17 -gravity center label:"$name" $filename;
    i=$((i+1))
done < territories
