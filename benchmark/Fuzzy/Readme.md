**1) SSRs panel construction**

TRF 

following the tutorial of https://github.com/HipSTR-Tool/HipSTR-references/blob/master/mouse/mouse_reference.md
```
for i in {1..5};do
	echo mm10/raw_fasta//$i.fa mm10/trf_results 5
done | xargs -L 1 -P 30 ../HipSTR-references-master/scripts/run_TRF.sh

for i in {1..5};do 
	echo ../HipSTR-references-master/scripts/fix_trf_output.py mm10/trf_results/$i.fa mm10/fixed_trf_results/$i.fa
done | xargs -L 1 -P 40 python

file=""
for i in {1..5};do
	files="$files,mm10/fixed_trf_results/$i.fa";
done

files=`echo $files | sed "s/,//"`

python ../HipSTR-references-master/scripts/trf_parser.py $files > filtered_repeats.mm10.bed

#exclude homopolymers
awk '{if($4>1)print}' filtered_repeats.mm10.bed > filtered_repeats.mm10.2-6nt.bed

bedtools sort -i filtered_repeats.mm10.2-6nt.bed > filtered_repeats.mm10.sorted.bed

python ../HipSTR-references-master/scripts/analyze_overlaps.py filtered_repeats.mm10.sorted.bed pass.mm10 fail.mm10

bedtools window -w 10 -a pass.mm10 -b fail.mm10 -v > pass.mm10.r2

bedtools merge -i pass.mm10.r2 -c 4,6 -o collapse -d 10 | grep -v "," > pass.mm10.r3

bedtools merge -i pass.mm10.r2 -c 4,4,4,6 -o collapse,count_distinct,distinct,collapse -d 10 | grep "," | awk '$5 == 1' | awk -v OFS="\t" '{print $1, $2, $3, $6, $7}' | sed "s/,/\//g" >> pass.mm10.r3

cat pass.mm10.r3 | bedtools sort | awk -v OFS="\t" '{print $1, $2, $3, $4, ($3-$2+1)/$4, "TRF_STR_"NR, $5}' > mm10.hipstr_reference.bed

#exclude SSR loci with centromere region
perl filter.bed.pl ../01.FCSSR/centromere.gff mm10.hipstr_reference.bed > trf.ref.bed
```

FCSSR fuzzy

Identification of fuzzy SSR using FCSSR and exclude the homopolyers

```
../benchmark_perfect/bin/FCSSR/src/FCSSR fuzzy -f /tmp/global2/wxian/00.data/t2t-col.20210610.fa -c 10,6,5,4,3,3 | awk 'NR>1{if( $4 != "A" && $4 != "T" && $4 != "C" && $4 != "G" )print}' > fcssr.2-6nt.bed 
```

Since FCSSR aims to identify conserved SSRs, which may be disrupted in long and non-conserved SSR regions, we offer a solution to merge the disrupted SSR loci
```
bedtools merge -d 20 -c 2,4 -o collapse -i fcssr.2-6nt.bed > fcssr.2-6nt.overlap.bed
perl merge.pl fcssr.2-6nt.overlap.bed fcssr.2-6nt.bed > tmp
cat merge.bed tmp > tmp2
bedtools sort -i tmp2 > pass.mm10.r2
```

The same as what has been done on TRF result
```
bedtools merge -i pass.mm10.r2 -c 4,6 -o collapse -d 10 | grep -v "," > pass.mm10.r3
bedtools merge -i pass.mm10.r2 -c 4,4,4,6 -o collapse,count_distinct,distinct,collapse -d 10 | grep "," | awk '$5 == 1' | awk -v OFS="\t" '{print $1, $2, $3, $6, $7}' | sed "s/,/\//g" >> pass.mm10.r3
cat pass.mm10.r3 | bedtools sort | awk -v OFS="\t" '{print $1, $2, $3, $4, ($3-$2+1)/$4, "FCSSR_STR_"NR, $5}' > mm10.hipstr_reference.bed
perl filter.centromere.pl centromere.gff mm10.hipstr_reference.bed > mm10.hipstr_reference.noncentro.bed
```
In order to be comparable with TRF, first we calculate the minimum number of copies of each unit in the trf.ref.bed (unit length-> minimum copy: 2->6; 3->5; 4->3.8; 5->3.4; 6->3), and then filter our results
```
awk '{if( ($4 == 2 && $5>=6) || ($4 == 3 && $5>=5) ||($4 == 4 && $5>=3.8) ||($4 == 5 && $5>=3.4) ||($4 == 6 && $5>=3) )print}' mm10.hipstr_reference.noncentro.bed > mm10.hipstr_reference.noncentro.trf.bed
bedtools sort -i mm10.hipstr_reference.noncentro.trf.bed > fcssr.ref.bed
```


**2) Comparison between TRF and FCSSR**

Before length filter, 97.1% TRF loci overlap with FCSSR loci
```
overlaploci=`bedtools intersect -a trf.ref.bed -b ../01.FCSSR/mm10.hipstr_reference.noncentro.bed -wa -wb | awk '{print $6}' | sort | uniq | wc -l`
trf=`less trf.ref.bed| wc -l`
echo "scale=3; $overlaploci/$trf*100" | bc
```

After length filter, 87% TRF loci overlap with FCSSR loci
```
overlaploci=`bedtools intersect -a trf.ref.bed -b fcssr.ref.bed -wa -wb | awk '{print $6}' | sort | uniq | wc -l`
trf=`less trf.ref.bed| wc -l`
echo "scale=3; $overlaploci/$trf*100" | bc
```

Length (unit length * copy number) distribution of the overlap SSR loci between TRF and FCSSR
```
bedtools intersect -a fcssr.ref.bed -b trf.ref.bed -wa -wb > intersect.txt
perl plot.pl intersect.txt > dot.txt
Rscript plot.r
```

![image](https://github.com/Wenfei-Xian/FCSSR/blob/main/benchmark/Fuzzy/length%20distribution.png)

**3) polymerphic SSR**

Hipstr
```
#TRF
HipSTR --bams bamfiles --fasta t2t-col.20210610.fa --regions trf.ref.bed --str-vcf trf.call.vcf.gz
#FCSSR
HipSTR --bams bamfiles --fasta t2t-col.20210610.fa --regions fcssr.ref.bed --str-vcf fcssr.call.vcf.gz
```
Quality filter
```
#filter_vcf.py was downloaded from https://github.com/HipSTR-Tool/HipSTR/tree/master/scripts
python HipSTR/scripts/filter_vcf.py --vcf trf.call.vcf.gz --min-call-qual 0.9 --max-call-flank-indel 0.15 --max-call-stutter 0.15 --min-call-allele-bias -2 --min-call-strand-bias -2 --min-loc-depth 5 > trf.call.filter.vcf
python HipSTR/scripts/filter_vcf.py --vcf fcssr.call.vcf.gz --min-call-qual 0.9 --max-call-flank-indel 0.15 --max-call-stutter 0.15 --min-call-allele-bias -2 --min-call-strand-bias -2 --min-loc-depth 5 > fcssr.call.filter.vcf
```
Population filter

Maximum missing rate should be smaller than 10%
```
vcftools --vcf trf.call.filter.vcf --max-missing 0.1 --recode --recode-INFO-all --out trf.call.filter.maxmissing0.1
vcftools --vcf fcssr.call.filter.vcf  --max-missing 0.1 --recode --recode-INFO-all --out fcssr.call.filter.maxmissing0.1
```
Non-ref allele rate should be larger than 10%



