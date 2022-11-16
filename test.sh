#!/bin/sh
#
# This is the configuration of the benchmarking test run.
#

SERVER=192.168.101.10
PORT=5004
SEED=3435245
TOTAL=100
START=0
DIFFICULTY=3000000
REP_PROB_PERCENT=20
DELAY_US=60000
PRIO_LAMBDA=1.5

/home/vagrant/os-challenge-common/client $SERVER $PORT $SEED $TOTAL $START $DIFFICULTY $REP_PROB_PERCENT $DELAY_US $PRIO_LAMBDA
