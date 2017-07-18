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
