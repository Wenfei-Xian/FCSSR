This script was used to benchmark of perfect SSR tools.
```
for i in {1..3};do bash benckmark.sh t2t-col.20210610.fa ;done > time.txt
```

| tools | ran1 | ran2 | ran3 | average | 
| --- | --- | --- | --- | --- |
| `git status` | List all *new or modified* files |
| `git diff` | Show file differences that **haven't been** staged |


All the *site files are the input for upset plot.

All jobs ran on AMD EPYC 7742 64-Core Processor
