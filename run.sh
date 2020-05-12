mkdir build
cd build
CC=gcc-9 CXX=g++-9 cmake -G "Unix Makefiles" ..
make
file=output.txt
#config1=config_simple.txt
#config2=config_simple.txt
config1=config.txt
config2=config_3threads.txt

echo start 4 treads > $file
./words_count_2_0 ../$config1 >> $file

echo start 3 treads >> $file
./words_count_2_0 ../$config2  >> $file

