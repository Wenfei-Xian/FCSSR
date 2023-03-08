genome=$1

echo "FCSSR:"
time -p ../bin/FCSSR/src/FCSSR perfect -f $genome 1> FCSSR.$genome.out 2>FCSSR.log
echo "Kmer-SSR"
time -p ../bin/Kmer-SSR/bin/kmer-ssr -i $genome -o kmerssr.$genome.out -n 10 -p 1-6 2> Kmer-SSR.log
echo "Misa"
time -p perl ../bin/Misa/misa.pl $genome 2>misa.log
echo "python-PERF"
time -p python ../bin/perf/PERF/core.py -i $genome -m 1 -M 6 -u repeat_units.txt -o python.PERF.$genome.out 2>python.PERF.log
echo "pypy-PERF"
time -p ../bin/perf/PERF/pypy3.8-v7.3.11-linux64/bin/pypy ../bin/perf/PERF/core.py -i $genome -m 1 -M 6 -u repeat_units.txt -o pypy.PERF.$genome.out 2>pypy.PERF.log
echo "Divissr"
time -p python3 ../bin/divissr/divissr/core.py -i $genome -o divissr.$genome.out 1> divissr.log
echo "Dot2dot"
time -p ../bin/Dot2dot/executables/dot.linux -c dot2dot.config -s $genome 1> dot2dot.$genome.out 2>dot2dot.log
