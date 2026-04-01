#!/bin/bash
./kill_all_1HRTDC.sh
sleep 1
./show_logs_1HRTDC.sh
echo ""
echo "USE Ctrl+C to stop"
echo ""
sleep 1
#run.py -cl config_all.xml 

xterm -T Operator  -geom 95x30+0+100  -e 'run.py -cl config_all.xml.1HRTDC' &
