#!/bin/bash

#delete old build
./clean.sh 2> /dev/null

directory="/home/giannis/Downloads/abalon"
con=$(ls $directory)
#build project
cmake ..
make 
echo "Running experiments";
for i in $con; do
	echo "Running experiments on file:"$i >> results.txt
	date >> results.txt;
	echo "-----------------------------------------------" >> results.txt
	numactl --cpunodebind=0 --physcpubind=0 --preferred=0	./BulkLoadRTree $directory/$i  100 0.8 >> results.txt

	echo "-----------------------------------------------" >> results.txt
	date >> results.txt;
        #clear/invalidate caches

        #clear caches to disk
        #http://unix.stackexchange.com/questions/87908/how-do-you-empty-the-buffers-$
        #free && sync && echo 3 > /proc/sys/vm/drop_caches && free
        #sync && echo 3 > /proc/sys/vm/drop_caches;

done

echo "#Dataset size|Time|Standard error" >> gnu_results.txt
cat results.txt |grep bst|tr -d "bst:" |sort -nk 1 >> gnu_results.txt

#print two new lines
echo >> gnu_results.txt
echo >> gnu_results.txt


gnuplot gnusc.sh
