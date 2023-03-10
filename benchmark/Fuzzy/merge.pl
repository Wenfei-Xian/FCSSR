use strict;
use warnings;

open IN1,"$ARGV[0]"; #overlap
open OUT1,">merge.bed";

my%hash_overlap;
while(<IN1>){
	chomp;
	my($chr,$start,$end,$site,$info)=(split /\t/,$_)[0,1,2,3,4];
	my@type;
	if($info=~m/\,/){ #overlap

		my$count = $info =~ tr/,/,/;
		my$len=$end-$start;
	       	if( $count == 1 ){
			my$match=0;
			my($type1,$type2)=(split /,/,$info)[0,1];
			my$type1_t=$type1;
			$type1_t=~tr/ATCG/TAGC/;
			my$type1_len=length$type1;
			my$type2_len=length$type2;
			my$copy=$len/$type1_len;
			$copy=sprintf "%.2f",$copy;
			if( $type1 eq $type2 ){
				$match=1;
				print OUT1 "$chr\t$start\t$end\t$type1_len\t$copy\t$type1\n";
			}
			elsif( $type1_len > $type2_len && $type1=~m/$type2/ ){
				$match=1;
				print OUT1 "$chr\t$start\t$end\t$type1_len\t$copy\t$type2\n";
			}
			elsif( $type1_len < $type2_len && $type2=~m/$type1/ ){
				$match=1;
				print OUT1 "$chr\t$start\t$end\t$type1_len\t$copy\t$type1\n";
			}
			elsif( $type1_len == $type2_len && $type1_t eq $type2 ){
				$match=1;
				print OUT1 "$chr\t$start\t$end\t$type1_len\t$copy\t$type1\n";
			}

			if( $match == 1 ){
				my@site_overlap=(split /,/,$site);
				foreach my$overlap(@site_overlap){
					my$ID="$chr=$overlap";
					$hash_overlap{$ID}=$ID;
				}
			}
		}
	}
}

open IN2,"$ARGV[1]";#Chr1    3191    3209    CCTAAA  3
while(<IN2>){
        chomp;
        my@bed=split /\t/,$_;
        my$ID="$bed[0]=$bed[1]";
	my$len=length$bed[3];
        if( exists $hash_overlap{$ID}){
        }
        else{
		print "$bed[0]\t$bed[1]\t$bed[2]\t$len\t$bed[4]\t$bed[3]\n";
        }
}
