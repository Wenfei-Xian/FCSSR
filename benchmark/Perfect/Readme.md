This script was used to benchmark the performance of various SSR tools.
```
for i in {1..3};do bash benckmark.sh t2t-col.20210610.fa ;done > time.txt
```
***
**Time consumming** (AMD EPYC 7742 64-Core Processor)
| tools | run1 Real time(s)/CPU time(s) | run2 | run3 | average | 
| --- | --- | --- | --- | --- |
| FCSSR | 4.43/4.53 | 4.33/4.25 | 4.42/4.31 | 4.39/4.36 |
| [Divissr](https://github.com/avvaruakshay/divissr) | 4.13/5.83 | 3.91/5.47 | 4.48/6.05 | 4.17/5.78 |
| [PERF-pypy](https://github.com/RKMlab/perf) | 14.72/16.05 | 15.00/16.23 | 15.51/16.61 | 15.08/16.30 |
| [Dot2dot](https://github.com/Gege7177/Dot2dot) | 23.58/20.26 | 21.49/20.62 | 21.66/20.82 | 22.24/20.57 |
| [PERF-python](https://github.com/RKMlab/perf) | 59.10/60.54 | 60.93/62.40 | 58.74/60.17 | 59.59/61.04 |
| [Misa](https://webblast.ipk-gatersleben.de/misa) | 127.26/126.19 | 124.44/123.30 | 127.42/125.97 | 126.37/125.15|
| [Kmer-SSR](https://github.com/ridgelab/Kmer-SSR) | 192.75/192.56 | 193.32/193.20 | 189.25/189.18 | 191.77/191.65 |
***
**Consistency** (Visualization using TBtools with *site files)
![image](https://github.com/Wenfei-Xian/FCSSR/blob/main/benchmark/Perfect/perfect-ssr.png)
***
**Short conclusion**

FCSSR and Divissr use almost the same amount of time, with FCSSR even faster in terms of CPU time. In terms of consistency, all software has very high consistency.
