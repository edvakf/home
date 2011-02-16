#!/bin/sh

if [[ -f "$1" ]]; then
  less "$1"
else
  if [[ "$1" = "" ]]; then
    ls -G
  else
    ls -G "$1"
  fi
fi

