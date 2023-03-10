use List::Util qw(max);

open IN1,"$ARGV[0]";#intersect.txt
my%hash;
my%hash_length;
while(<IN1>){
	chomp;
	my@array=(split /\t/,$_);
	my$len=$array[3]*$array[4];
	push @{$hash{"$array[7]\_$array[8]"}},"$len\t$array[0]\_$array[1]";
	$hash_length{"$array[7]\_$array[8]"}=$array[10]*$array[11];
}
print "FCSSR\tFCSSRsite\tTRF\tTRFsite\n";
foreach my$ssr ( sort keys %hash_length ){
	my$max=max (@{$hash{$ssr}});
	print "$max\t$hash_length{$ssr}\t$ssr\n";
}
