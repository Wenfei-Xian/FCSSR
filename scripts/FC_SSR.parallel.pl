my$local_datestring = localtime();
print "$local_datestring\n";
my$folder="$ARGV[0].dir";
if( -e $folder ){
	print "Error, $ARGV[0].dir already exist!!!\n";
	exit;
}
else{
	`mkdir $ARGV[0].dir`;
}

my$parallelscript="$ARGV[0].parallel.sh";
if( -e $parallelscript ){
	print "Error, $ARGV[0].parallel.sh already exists!!!\n";
	exit;
}

my$combinedfile="$ARGV[0].FC_SSR.out";
if( -e $combinedfile ){
	print "Error, $ARGV[0].FC_SSR.out already exists!!!\n";
	exit;
}

open IN1,"$ARGV[0]" or die "Can't open $ARGV[0]";
my%hash_chr_split;
my@split_file;
open OUT2,">$ARGV[0].parallel.sh";
$/=">";<IN1>;
while(<IN1>){
	chomp;
	my($ID,$seq)=(split /\n/,$_,2)[0,1];
	$seq=~s/\n//g;
	$seq=~s/>//;
	if($ID=~/ /){
		$ID=$1 if(/(\S+)\s(\S+)/);
	}
	push @split_file,"$ID";
	my$len=length$seq;
	my$order=0;
	for( my$start=0; $start<$len; $start+=100000000 ){
		my$subseq=substr($seq,$start,100001000);
		#print "$ID\t$start\n";
		push @{$hash_chr_split{$ID}},$order;
		print OUT2 "./FC_SSR perfect -f $ARGV[0].dir/$ID\_$order.fa -l 6 -L 10 -s 0 -u 0 -r 0 > $ARGV[0].dir/$ID\_$order.fa.FC_SSR.out\n";
		open OUT1,">$ARGV[0].dir/$ID\_$order.fa";
		print OUT1 ">$ID\_$order\n$subseq\n";
		$order++;
	}
}
$/="\n";
$local_datestring = localtime();
print "$local_datestring\n";

`cat $ARGV[0].parallel.sh | parallel -j 2 2> $ARGV[0].parallel.sh.err`;
$local_datestring = localtime();
print "$local_datestring\n";

my$first_file="$split_file[0]\_0.fa";
open INfile1,"$ARGV[0].dir/$first_file.FC_SSR.out" or die "Can't open $ARGV[0].dir/$first_file.FC_SSR.out";
my$header=<INfile1>;

open OUTcombined,">$ARGV[0].FC_SSR.out";
print OUTcombined "$header";

foreach my$chr ( @split_file ){
	my$cutoff=0;
	foreach my$split ( @{$hash_chr_split{$chr}} ){
		#my$cutoff=0;
		open INN,"$ARGV[0].dir/$chr\_$split.fa.FC_SSR.out";
		<INN>;
		while(<INN>){
			chomp;
			my@out=(split /\t/,$_);
			$out[0]=$chr;
			if( $out[2] <= 100000000 && $out[1] >= $cutoff ){
				$out[1]+=100000000*$split;
				$out[2]+=100000000*$split;
				print OUTcombined join "\t",@out;
				print OUTcombined "\n";
			}
			elsif( $out[1] <= 100000000 && $out[2] > 100000000 ){
				$cutoff=$out[2]-100000000;
				$out[1]+=100000000*$split;
				$out[2]+=100000000*$split;
				print OUTcombined join "\t",@out;
				print OUTcombined "\n";
				last;
			}
			elsif( $out[1] > 100000000 ){
				$cutoff=0;
				last;
			}
		}
	}
}
$local_datestring = localtime();
print "$local_datestring\n";
