#!/bin/sh
./clean.sh
if (./compile.sh) then
    qsub job.sh
    echo "Reschedule complete."

    sleep 3

    exists=0
    while [ "$exists" -ne 1 ]
    do
        exists=$([ -r buddhabrot.o* ] && echo 1 || echo 0)
        sleep 1
    done
    watch -n 0.5 "cat buddhabrot.o* | tail -20"
fi
