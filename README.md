# Efficient Process Scheduling Algorithm using RR and SRTF (ESRR)

## Overview

ESRR aims to:
1. Reduce total waiting time and turnaround time in comparison to RR 
2. Reduce the waiting time of shorter processes
3. Increase response time for longer processes

## Required inputs

This program requires 4 pieces of information to work
1. Total number of processes
2. Arrival time for each process
3. Burst time for each process
4. Time quantum

## Algorithm

The program will be in 'SRTF-mode' when it is still expecting processes to arrival (not all processes have arrived)

When all processes have arrived, it will do one last sort (by burst left) and then switch over to 'RR-mode' where it will run until all processes in the ready queue is completed

## Implementation

Algorithm is implemented in C.

Written by me

It is not properly formatted and messily done up but it works.. so i count that as a win (:

## Background

Paper was published in the 2020 International Conference on Emerging Trends in Information Technology and Engineering (ic-ETITE)

## Authors of the paper:
- Preeti Sinha
- B.Prabadevi
- Sonia Dutta
- Deepa N
- Neha Kumari

More details can found in the published work.
