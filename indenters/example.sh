#!/bin/sh

string="Hallo Welt"
# if else test
if [ -n "$string" ]; then
echo "The string is \"$string\"!"
else
echo "The string is empty!"
fi

# for test
array="Text1 Text2 Text3 Text4"
for i in $array
do
echo "The string \"$i\" is in the array!"
done

count=0
while [ $count -le 10 ]
do
echo "We've counted up to $count."
count=$[$count+1]  #increment counter by one.
done

echo "Passed everything!"
#read -p "press any key to continue"

