#!/bin/bash

# This small script dublicates certain files
# in case small changes has been done.

for dat in FS_Array1D C2_Array1D FS_C2_Array1D
do
    # basic file
    f=${dat}.F90
    
    for i in 2 3
    do
	# new file name
	nf=${f//Array1D/Array${i}D}

	# copy to new file
	cp $f $nf
	
	sed -i -e "s/Array1D/Array${i}D/g" $nf

    done
    
done
