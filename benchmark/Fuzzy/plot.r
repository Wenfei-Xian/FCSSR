library(ggplot2);
data=read.table("dot.txt",header=1);
ggplot(data,aes(x=FCSSR,y=TRF))+geom_point(size=0.5);
