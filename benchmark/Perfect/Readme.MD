This script was used to benchmark of perfect SSR tools.
```
for i in {1..3};do bash benckmark.sh t2t-col.20210610.fa ;done > time.txt
```

| tools | run1 | run2 | run3 | average | 
| --- | --- | --- | --- | --- |
| FCSSR | | | | |
| Divissr | | | | |
| Dot2dot | | | | |
| PERF-pypy | | | | |
| PERF-python | | | | |
| Misa | | | | |
| Kmer-SSR | | | | |


All the *site files are the input for upset plot.

All jobs ran on AMD EPYC 7742 64-Core Processor
