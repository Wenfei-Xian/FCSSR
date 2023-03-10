1) Fuzzy SSRs identification

TRF
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

awk '{if($4>1)print}' filtered_repeats.mm10.bed > filtered_repeats.mm10.2-6nt.bed

bedtools sort -i filtered_repeats.mm10.2-6nt.bed > filtered_repeats.mm10.sorted.bed

python ../HipSTR-references-master/scripts/analyze_overlaps.py filtered_repeats.mm10.sorted.bed pass.mm10 fail.mm10

bedtools window -w 10 -a pass.mm10 -b fail.mm10 -v > pass.mm10.r2

bedtools merge -i pass.mm10.r2 -c 4,6 -o collapse -d 10 | grep -v "," > pass.mm10.r3

bedtools merge -i pass.mm10.r2 -c 4,4,4,6 -o collapse,count_distinct,distinct,collapse -d 10 | grep "," | awk '$5 == 1' | awk -v OFS="\t" '{print $1, $2, $3, $6, $7}' | sed "s/,/\//g" >> pass.mm10.r3

cat pass.mm10.r3 | bedtools sort | awk -v OFS="\t" '{print $1, $2, $3, $4, ($3-$2+1)/$4, "TRF_STR_"NR, $5}' > mm10.hipstr_reference.bed

perl ../01.FCSSR/filter.bed.pl ../01.FCSSR/centromere.gff mm10.hipstr_reference.bed > trf.ref.bed
```
