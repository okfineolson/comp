for f in *.sh;
do
  if [ $f != 'test.sh' ];then
    echo $f
    bash $f
  fi
done