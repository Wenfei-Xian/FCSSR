This script was used to benchmark of perfect SSR tools.
```
for i in {1..3};do bash benckmark.sh t2t-col.20210610.fa ;done > time.txt
```

| tools | run1 CPU time(s)/Real time(s) | run2 | run3 | average | 
| --- | --- | --- | --- | --- |
| FCSSR | 4.43/4.53 | | | |
| Divissr | 4.13/5.83 | | | |
| Dot2dot | 23.58/20.26 | | | |
| PERF-pypy | 14.72/16.05 | | | |
| PERF-python | 59.10/60.54 | | | |
| Misa | 127.26/126.19 | | | |
| Kmer-SSR | 192.75/192.56 | | | |


All the *site files are the input for upset plot.

All jobs ran on AMD EPYC 7742 64-Core Processor
