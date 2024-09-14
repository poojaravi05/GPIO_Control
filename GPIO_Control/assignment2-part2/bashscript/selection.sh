#!/bin/bash


case $1 in
  0) ./mux.sh 32  ; gpio=11;exit $gpio ;;
  1) ./mux.sh 28  ;./mux.sh 45 ;gpio=12;exit $gpio ;;
  2) ./mu.sh 77 ;./mux.sh 34  ;gpio=13;exit $gpio ;;
  3) ./mux.sh 16 ;./mu.sh 76 ; ./mu.sh 64 ; gpio=14;exit $gpio;;
  4) ./mux.sh 36 ;gpio=6;exit $gpio;;
  5) ./mux.sh 18 ;./mu.sh 66 ;gpio=0;exit $gpio;;
  6) ./mux.sh 20 ;./mu.sh 68 ;gpio=1;exit $gpio;;
  7) gpio=38 ; exit $gpio;;
  8) gpio=40 ; exit $gpio;;
  9) ./mux.sh 22 ; ./mu.sh 70 ;gpio=4;exit $gpio;;
  10) ./mux.sh 26 ; ./mu.sh 74 ;gpio=10;exit $gpio;;
  11) ./mux.sh 24 ; ./mux.sh 44 ;./mux.sh 72 ;gpio=5;exit $gpio;;
  12) ./mux.sh 42 ; gpio=15;exit $gpio;;
  13) ./mux.sh 46 ; ./mux.sh 30 ;gpio=7;exit $gpio;;
esac
