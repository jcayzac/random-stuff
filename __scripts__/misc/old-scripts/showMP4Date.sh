#!/bin/bash
date -juf %s $(($(mp4file --dump "$1"|grep eationT|head -n 1|sed 's/.* = \([0-9][0-9]*\).*/\1/')-0x7C25B080))

