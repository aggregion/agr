#!/bin/bash

# Usage:
# Go into cmd loop: sudo ./clagr.sh
# Run single cmd:  sudo ./clagr.sh <clagr paramers>

PREFIX="docker-compose exec nodagrd clagr"
if [ -z $1 ] ; then
  while :
  do
    read -e -p "clagr " cmd
    history -s "$cmd"
    $PREFIX $cmd
  done
else
  $PREFIX "$@"
fi
