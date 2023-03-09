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
	-P int        maximum unmatch(inserted+mismatch+deleted) percentage of the whole SSRs region (default value: 0.1)
	-u int        whether to replace all letters with uppercase letters (default value: 1 -> replace; 0 -> don't replace)
	-s int        whether output the flanking sequnce of SSR region (default value: 0 -> don't output; length of flanking sequences -> output)
	-r int        whether output the reverse complement sequence (defalut value: 0 -> don't output; 1 -> output)

```

-l: The maximum length of the SSR unit is specified by the "-l" parameter, with a default value of 6. This means that the unit length can range from 1 to 6 nucleotides. If you want to search for longer units, you can increase the value of "-l" to your desired length, for example, to 10.

-c: The copy cutoff for each SSR unit is set by default to 10, 5, 4, 3, 2, 2, respectively. For a unit with a length of 1, only copy numbers greater than or equal to 10 will be included in the output. For a unit with a length of 2, only copy numbers greater than or equal to 5 will be included in the output.

-s: You can choose whether to output the flanking sequence of the SSR. By default, the value is set to 0, meaning that no flanking sequences will be included in the output. However, you can specify the length of the flanking sequence using the "-s" parameter. For example, setting "-s 100" will output the 100 base pairs of flanking sequence on either side of the SSR.

-i: The maximum length of insertion allowed within an SSR unit is determined by the "-i" parameter, with a default value of 1. For example, if the unit length is 5, the algorithm will compare the first 5 base pairs of the sequence with the next 6 base pairs (5 base pairs of the SSR unit + 1 base pair of insertion).

-m: The maximum number of allowed unmatch (insertions, deletions, and substitutions) within an SSR unit is specified by the "-m" parameter, with a default value of 1. For example, if the unit length is 5, the algorithm will compare the first 5 base pairs of the sequence with the next 6 base pairs. The longest common sequence (LCS) between the two sequences must be at least 4 base pairs long (5 base pairs of the SSR unit minus 1 allowed unmatch).

-b: You can choose whether to allow deletion/mismatch and insertion within a single SSR unit using the "-b" parameter, which is set to 0 by default. For example, the sequences "ATCG" and "ACTG" have a longest common sequence of 3, but an edit distance of 2, meaning that two mutations are required to convert "ATCG" to "ACTG". In DNA, this would correspond to two mutations in the nucleotide sequence. If you want to search for conserved SSRs, it is recommended to set the "-b" parameter to 0.

-P: The maximum allowed percentage of unmatches (insertions, deletions, and substitutions) in the entire SSR region is set by the "-P" parameter, with a default value of 0.1. For example, if the SSR region is 20 base pairs long, the maximum number of allowed unmatches would be 20 multiplied by 0.1, which is equal to 2. If the number of unmatches exceeds 2, the program will stop searching for that SSR unit.
