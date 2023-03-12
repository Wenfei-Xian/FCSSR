all MA lines were downloaded from NCBI
1) clean reads were obtained by using fastp with -q 20 to filter reads' quality lower than q20
2) clean and q20 reads were mapped to Col-0 t2t assemblies using bwa-mem, PCR duplication were marked by MarkDuplicates in gatk-4.2.3.0
3) Hipstr was used to genotyping all the SSR loci among the MA lines samples.
4) Hipstr default filter
5) vcftools maximum missing rate 0.1
