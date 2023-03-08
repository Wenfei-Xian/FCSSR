genome=$1

source /tmp/global2/wxian/software/anaconda3/bin/activate
conda activate divissr

echo "FCSSR:"
time -p ../bin/FCSSR/src/FCSSR perfect -f $genome 1> FCSSR.$genome.out 2>FCSSR.log
awk 'NR>1{print $1"_"$2}' FCSSR.$genome.out > FCSSR.$genome.site

echo "Kmer-SSR"
time -p ../bin/Kmer-SSR/bin/kmer-ssr -i $genome -o kmerssr.$genome.out -n 10 -p 1-6 2> Kmer-SSR.log
awk 'NR>1{print $1"_"$4+1}' kmerssr.$genome.out > kmerssr.$genome.site

echo "Misa"
time -p perl ../bin/Misa/misa.pl $genome 2>misa.log
cat *.gff | grep -Ev "#|region" |awk '{print $1"_"$4}' > Misa.site

echo "python-PERF"
time -p python ../bin/perf/PERF/core.py -i $genome -m 1 -M 6 -u repeat_units.txt -o python.PERF.$genome.out 2>python.PERF.log
awk '{print $1"_"$2+1}' python.PERF.$genome.out > python.PERF.$genome.site

echo "pypy-PERF"
time -p ../bin/perf/PERF/pypy3.8-v7.3.11-linux64/bin/pypy ../bin/perf/PERF/core.py -i $genome -m 1 -M 6 -u repeat_units.txt -o pypy.PERF.$genome.out 2>pypy.PERF.log
awk '{print $1"_"$2+1}' pypy.PERF.$genome.out > pypy.PERF.$genome.site

echo "Divissr"
time -p python3 ../bin/divissr/divissr/core.py -i $genome -m 1 -M 6 -l 10 -o divissr.$genome.out 1> divissr.log
perl -ne 'chomp; ($chr,$site,$rep,$motif)=(split /\t/,$_)[0,1,6,7]; $site++;  if( (length$motif == 1 && $rep >=10) || (length$motif == 2 && $rep >=5) || (length$motif == 3 && $rep >=4) || (length$motif == 4 && $rep >=3) || (length$motif == 5 && $rep >=2) || (length$motif == 6 && $rep >=2) ){print "$chr\_$site\n"}' divissr.$genome.out > divissr.$genome.site 

echo "Dot2dot"
time -p ../bin/Dot2dot/executables/dot.linux -c dot2dot.config -s $genome 1> dot2dot.$genome.out 2>dot2dot.log
awk 'NR>1{if($4*$5 >=10){print $1"_"$2}}' dot2dot.$genome.out > dot2dot.$genome.site 
