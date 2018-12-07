#!/bin/bash
#
# agrio-tn_down.sh is used by the agrio-tn_bounce.sh and agrio-tn_roll.sh scripts.
# It is intended to terminate specific AGR.IO daemon processes.
#


if [ "$PWD" != "$AGRIO_HOME" ]; then
    echo $0 must only be run from $AGRIO_HOME
    exit -1
fi

prog=nodagr

DD=var/lib/node_$AGRIO_NODE
runtest=`cat $DD/$prog.pid`
echo runtest = $runtest
running=`ps -e | grep $runtest | grep -cv grep `

if [ $running -ne 0 ]; then
    echo killing $prog

    kill -15 $runtest

    for (( a = 1;11-$a; a = $(($a + 1)) )); do
        echo waiting for safe termination, pass $a
        sleep 2
        running=`ps -e | grep $runtest | grep -cv grep`
        echo running = $running
        if [ $running -eq 0 ]; then
            break;
        fi
    done
fi

if [ $running -ne 0 ]; then
    echo killing $prog with SIGTERM failed, trying with SIGKILL
    pkill -9 $runtest
fi
