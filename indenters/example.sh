#!/bin/bash

# if else test
if [ -n "$string" ]; then
echo "The string is \"$string\"!"
else
echo "The string is empty!"
fi

# for test
for i in $array
do
echo "The string \"$i\" is in the array!"
done

while [ $count -le 10 ]
do
echo "We've counted up to $count."
count=$[$count+1]  #increment counter by one.
done

echo "Passed everything!"
#read -p "press any key to continue"
