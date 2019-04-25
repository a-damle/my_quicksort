#!/bin/bash
PROCESS_PID="$(ps aux | grep  cquicksort | grep -v grep  | awk '{print $2}')"
ps huH p $PROCESS_PID | wc -l
