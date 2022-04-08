#!/bin/bash

# Usage: ./bumb-version.sh
# bumb the version number in version.h
# 1.0.1 --> 1.0.2

VERSION_H=$1

INITIAL_CONTENT=$(cat $VERSION_H)
VERSION=$(cat $VERSION_H | awk '{print $3}' | sed 's/"//g')
MINOR=$(echo $VERSION | awk -F . '{print $3}')
MINOR=$(($MINOR+1))
NEW_VERSION=$(echo $VERSION | awk -F . '{printf "%d.%d.%d\n", $1, $2, '$MINOR})

echo $INITIAL_CONTENT | awk '{printf "%s %s ", $1, $2}' > $VERSION_H
printf '"%s"\n' $NEW_VERSION >> $VERSION_H
