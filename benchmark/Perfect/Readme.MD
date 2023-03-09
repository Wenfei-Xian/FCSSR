This script was used to benchmark of perfect SSR tools.
```
for i in {1..3};do bash benckmark.sh t2t-col.20210610.fa ;done > time.txt
```

| tools | run1 CPU time(s)/Real time(s) | run2 | run3 | average | 
| --- | --- | --- | --- | --- |
| FCSSR | 4.43/4.53 | 4.33/4.25 | 4.42/4.31 | |
| Divissr | 4.13/5.83 | 3.91/5.47 | 4.48/6.05 | |
| PERF-pypy | 14.72/16.05 | 15.00/16.23 | 15.51/16.61 | |
| Dot2dot | 23.58/20.26 | 21.49/20.62 | 21.66/20.82 | |
| PERF-python | 59.10/60.54 | 60.93/62.40 | 58.74/60.17 | |
| Misa | 127.26/126.19 | 124.44/123.30 | 127.42/125.97 | |
| Kmer-SSR | 192.75/192.56 | 193.32/193.20 | 189.25/189.18 | |


All the *site files are the input for upset plot.

All jobs ran on AMD EPYC 7742 64-Core Processor
