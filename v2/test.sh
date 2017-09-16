#for ((i=0;i<15;i++))
while [ 1 ]
do
    ./sol.exe > log
    if [ "$?" == "1" ];then
        echo 'break'
        exit
    fi
done
echo 'all pass'

#http://minesweeperonline.com/ on my chrome
#ZERO.x = 256 , ZERO.y = 171
#./execute.exe -s -b -xy 171 256
