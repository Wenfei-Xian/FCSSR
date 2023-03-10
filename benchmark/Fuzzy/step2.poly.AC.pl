open IN1,"$ARGV[0]";
while(<IN1>){
	if(/#/){
		print $_;
	}
	else{
		my@vcf=split /\t/,$_;
		my@info=split /;/,$vcf[7];
		my$AN=0;
		my$REFAC=0;
		my$AC=0;
		foreach my$info_each (@info){
			if( $info_each=~m/AN=/){
				$AN=(split /=/,$info_each)[1];
			}
			elsif( $info_each=~m/REFAC=/){
				$REFAC=(split /=/,$info_each)[1];
			}
		}
		my$altAC=$AN-$REFAC;
		my$cutoff1=$AN*$ARGV[1];
		if( $altAC > $cutoff1 ){
			print $_;
		}
	}
}			
