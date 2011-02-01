#!/bin/sh

IFS='
'
str=''
delim=''
delim_="$RANDOM$RANDOM$RANDOM$RANDOM"

while read -r line; do
  str="$str$delim$line"
  delim=$delim_
done

# copy as windows format (\n becomes CRLF)
echo -n $str | sed s/$delim/\\n/g > /dev/clipboard
