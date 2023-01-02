#!/usr/bin/perl

use warnings;
use strict;
use List::Util qw(shuffle);
use Getopt::Long;

=head1 Usage

 perl simualtion.pl <parameters>
 	--unitfile|u       unit file which include SSR units (mandatory parameter)
	--length|l         length of the simulation fasta (default value: 100,000,000 )
	--numberssrs|n     the number the SSRs (default value: 5,000)
	--outfa|f          simulate fasta (dafault: Out.perfect.fa | Out.fuzzy.fa)
	--outbed|b         simulate information (default: Out.fuzzy.bed | Out.fuzz.bed)
	--fuzzy|z          perfect simulate or fuzzy simulate (0:perfect | 1:fuzzy)
	--help|h           Help
=cut

my($unitfile,$length,$numberssrs,$help,$outfa,$outbed,$fuzzy);

GetOptions(
	"unitfile|u:s"=>\$unitfile,
	"length|l:s"=>\$length,
	"numberssrs|n:s"=>\$numberssrs,
	"outfa|f:s"=>\$outfa,
	"outbed|b:s"=>\$outbed,
	"fuzzy|z:s"=>\$fuzzy,
	"help|h"=>\$help
);

$length ||= 10000000;
$numberssrs ||= 5000;
#$outfa ||= "out.fa";
#$outbed ||= "out.bed";
$fuzzy ||= 0;

if( $fuzzy == 0 ){
	$outfa = "Out.perfect.fa";
	$outbed = "Out.perfect.bed";
}
elsif( $fuzzy == 1){
	$outfa = "Out.fuzzy.fa";
	$outbed = "Out.fuzzy.bed";
}


die `pod2text $0` if ( $help );

if( -e $outfa || -e $outbed ){
	print "$outfa or $outbed exists !!!\n";
	exit;
}

open IN1,"$unitfile" or die "Can't open $unitfile !!! ";

my@unit;

while(<IN1>){
	chomp;
	push @unit,$_;
}

my@base_array=('A','T','C','G');
my@repeat_time=( 2 .. 50 );
my%site_insertion;

my@variant=( 1 .. 3 );

for( 1 .. $numberssrs ){
	my$range=$length;
	my$no;
	do{
		$no=int rand($range);
	}
	while( exists $site_insertion{$no});
	$site_insertion{$no}=1;
}

open OUT1,">$outfa";
open OUT2,">$outbed";

print OUT1 ">Simulation\n";

my$len=1;
my$last="";

for ( my$i=0; $i<$length; $i++ ){

	if( exists $site_insertion{$i}){

		foreach ( my$time=0; $time < $length; $time++ ){

			my@random_unit=shuffle(@unit);
			my@random_repeat_time=shuffle(@repeat_time);
			my$unit_last=(split //,$random_unit[0])[-1];

			if( length($random_unit[$time])*$random_repeat_time[0] > 9 && $unit_last ne $last ){
				my$len_add=$len+length($random_unit[0])*$random_repeat_time[0];
		
				my$type;

				for( my$i=1; $i<=$random_repeat_time[0]; $i++ ){
					if( $fuzzy == 1){
						if($i == 2){
							my@random_variants=shuffle(@variant);
							if($random_variants[0] == 1 ){
								$type="insertion";
								my$random_unit_c=insertion($random_unit[0]);
								print OUT1 "$random_unit_c";
							}
							elsif($random_variants[0] == 2 ){
								$type="deletion";
								my$random_unit_c=deletion($random_unit[0]);
								print OUT1 "$random_unit_c";
							}
							elsif($random_variants[0] == 3 ){
								$type="mismatch";
								my$random_unit_c=mismatch($random_unit[0]);
								print OUT1 "$random_unit_c";
							}
						}
						else{
							print OUT1 "$random_unit[0]";
						}
					}
					elsif($fuzzy == 0){
						print OUT1 "$random_unit[0]";
					}
				}
				
				if($fuzzy == 1 ){
					print OUT2 "Simulation\t$len\t$len_add\t$random_unit[0]\t$random_repeat_time[0]\t$unit_last\t$last\t$type\n";
				}
				elsif($fuzzy == 0){
					print OUT2 "Simulation\t$len\t$len_add\t$random_unit[0]\t$random_repeat_time[0]\t$unit_last\t$last\n";
				}

				$len+=(length$random_unit[0])*$random_repeat_time[0];

				last;
			}
		}

	}
	else{
		my@random_base=shuffle(@base_array);
		$last=$random_base[0];
		print OUT1 "$random_base[0]";
		$len++;
	}
}

sub insertion{
	my$len=length$_[0];
	my$left=substr($_[0],0,1);
	my$right=substr($_[0],1,$len-1);
	my@random_base=shuffle(@base_array);
	my$insertion_seq="$left"."$random_base[0]"."$right";
	return $insertion_seq;
}

sub deletion{
	my$len=length$_[0];
	my$left=substr($_[0],0,1);
	my$right=substr($_[0],1,$len-1);
	$right=substr($right,1,$len-2);
	my$deletion_seq="$left"."$right";
	return $deletion_seq;
}

sub mismatch{
	my$len=length$_[0];
	my$left=substr($_[0],0,1);
	my$right=substr($_[0],1,$len-1);
	my$right_first=substr($right,0,1);
	my$right_last=substr($right,1,$len-2);
	for( my$i=0; $i < 10000; $i++){
		my@random_base=shuffle(@base_array);
		if( $random_base[0] ne $right_first ){
			$right_first=$random_base[0];
			last;
		}
	}
	my$mismatch_seq="$left"."$right_first"."$right_last";
	return $mismatch_seq;
}
