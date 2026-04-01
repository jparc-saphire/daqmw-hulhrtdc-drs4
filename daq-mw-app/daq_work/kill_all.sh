#!/bin/bash

pgrep -fl Comp
#pkill -P $$
killall -KILL -q tail
killall -KILL -q DaqOperatorComp
killall -KILL -q BestEffortDispatcherComp
killall -KILL -q MergerComp
killall -KILL -q Drs4QdcReader1Comp
killall -KILL -q MznHRTdcReader1Comp
killall -KILL -q MznHRTdcReader2Comp
killall -KILL -q MznHRTdcReader3Comp
killall -KILL -q MonitorComp
killall -KILL -q DAQLoggerComp
