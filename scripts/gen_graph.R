library(ggplot2)
library(plyr)
library(lattice)

datafile = commandArgs(TRUE)[1]
wholeframe = read.table(datafile, header=TRUE)
framsum = ddply(wholeframe, c("Runtime","Numactl", "Progname", "Placement","Size","Blocksize","Threads"), summarize, GFlops = mean(Gflops), Std = sd(Gflops))

pd = position_dodge(width=.1)

pdf(paste("graph_", datafile, ".pdf", sep = ''), width = 10, height=6)
myplot = ggplot(framsum) + geom_errorbar(aes(x=Threads, ymin=GFlops-Std, ymax=GFlops+Std, width=.1))
myplot = myplot + geom_line(aes(x=Threads, y=GFlops, group=interaction(Runtime,Size,Blocksize),color=interaction(Runtime,Size,Blocksize)))
myplot = myplot + facet_grid(~Placement)
myplot = myplot + guides(col = guide_legend(ncol=3))
myplot = myplot + theme(legend.text = element_text(size=8), legend.title = element_text(size=8), legend.position="bottom")
myplot = myplot + ggtitle("Performance of OpenMP Runtimes")
print(myplot)
dev.off()

