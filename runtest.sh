#!/bin/bash

TEMP=$(getopt -o '' -n $(basename $0) -- "$@")

if [ $? != 0 ] ; then echo "Terminating..." >&2 ; exit 1 ; fi

eval set -- "$TEMP"

while true; do
  case "$1" in
    -- ) shift; break ;;
  esac
done

build/test &
PID=$!

if build/clienttest && wait $PID
then
    exit 0
else
    exit 1
fi
