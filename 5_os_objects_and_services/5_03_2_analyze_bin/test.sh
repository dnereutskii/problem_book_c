#!/bin/bash

cat nums1.txt | ./bingen/bingen -s 4 -n bin1
cat nums2.txt | ./bingen/bingen -s 4 -n bin2
cat nums3.txt | ./bingen/bingen -s 4 -n bin3
cat nums4.txt | ./bingen/bingen -s 2 -n bin4
./prog bin1 bin2 bin3 bin4 info.txt
