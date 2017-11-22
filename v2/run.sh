#if [ $# -eq 0 ];then
#    chrome http://minesweeperonline.com/ &
#    sleep 3
#fi
sleep 1
if [ $# -eq 0 ];then
    ./execute.exe -b -xy 172 256 # (0 , 0)'s white
else
    ./execute.exe $1 -b -xy 172 256
fi
