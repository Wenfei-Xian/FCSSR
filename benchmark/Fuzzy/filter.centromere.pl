open IN1,"$ARGV[0]";#centromere
my@centromere;
while(<IN1>){
	chomp;
	push @centromere,$_;
}

open IN2,"$ARGV[1]";#SSR
while(<IN2>){
	chomp;
	my($Chr,$Start,$End)=(split /\t/,$_)[0,1,2];
	my$with=0;
	foreach my$list ( @centromere ){
		my($chr,$start,$end)=(split /\t/,$list)[0,1,2];
		if( $Chr eq $chr && $Start>$start && $End<$end ){
			$with=1;
		}
	}
	if( $with == 0 ){
		print "$_\n";
	}
}
