#!/bin/sh

if [[ -f "$@" ]]; then
  less "$@"
else
  if [[ "$@" = "" ]]; then
    ls
  else
    ls "$@"
  fi
fi

