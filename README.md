# FCSSR
A Fast and Comprehensive Short Simple Repeat Finder
## Installation
```
#download
git clone https://github.com/Wenfei-Xian/FCSSR.git
cd FCSSR/src
#install
make clean && make
```
## Usage
```
./FCSSR
This program consists of two SSR searching modules: perfect and fuzzy
Version:1.0, March,2023
Usage: FC_SSR <command> [options]
Command:
	perfect: perfect model
	fuzzy: fuzzy model
```

```
Usage: FC_SSR perfect [options]
arguments:
       -f string     fasta format file (mandatory)
       -l int        maximum length of SSR unit (default value: 6)
       -c string     copy cutoff for each unit, the number of cutoff should be the same as SSR unit (default value: 10,5,4,3,2,2)
       -s int        whether output the flanking sequence of SSR region (default value: 0 -> don't output; length of flanking sequences -> output)
       -u int        whether replace all letters with uppercase letters (default value: 1 -> replace; 0 -> don't replace)
       -r int        whether output the reverse complement sequence (default value: 0 -> don't output; 1 -> output)
```

```
Usage: FC_SSR perfect [options]
arguments:
	-f string     fasta format file (mandatory)
	-l int        maximum length of SSR unit (default value: 6)
	-i int        maximum length of insertion allowed within unit (default value: 1)
	-c string     copy cutoff for each unit, the number of cutoff should be the same as SSR unit (default value: 10,5,4,3,2,2)
	-m int        maximum length of unmatch within unit (default value: 1)
	-b int        whether allow deletion/mismatch and insertion retain in one unit, 1 allow, 0 not allow (default value: 0)
	-P int        maximum unmatch(inserted+mismatch+deleted) percentage whold SSRs region (default value: 0.1)
	-u int        whether to replace all letters with uppercase letters (default value: 1 -> replace; 0 -> don't replace)
	-s int        whether output the flanking sequnce of SSR region (default value: 0 -> don't output; length of flanking sequences -> output)
	-r int        whether output the reverse complement sequence (defalut value: 0 -> don't output; 1 -> output)

```

-l: maximum length of SSR unit. The default value is 6. Unit lenght among 1-6 nt will be used. If you want to search longer unit, please increase the number to what you want, such as 10.

-c: copy cutoff for each unit. The default value is 10,5,4,3,2,2. When unit length is 1, only copy number larger or equal to 10 will be output, when unit lenght is 2, only copy number larger or equal to 5 will be output.

-s: whether output the flanking sequence of SSR. The default value is 0. No flanking sequences will be outoupt. When you set -s 100, flanking 100bp sequence will be output

-i: maximum length of insertion allowed within unit. The default value is 1. When the unit length is 5, the first 5bp sequence will be compared with the next 6bp (5bp+1bp) sequences. 

-m: maximum unmatch (insertion+mismatch+deletion) with unit. The default value is 1. The longest common sequence (LCS) of the frist 5bp sequence and the next 6bp sequence is 5-1=4bp.

-b: whether allow deletion/mismatch and insertion exists in one unit. The default value is 0. Such as ATCG ACTG, the LCS is 3, but the edit distance is 2. In a DNA molecular, ATCG needs to undergo two mutations to become ACTG. If you want to search conservated SSR, please set the value to 0.







