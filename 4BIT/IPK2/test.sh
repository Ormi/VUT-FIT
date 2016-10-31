#!/bin/sh

###
# @file test.sh
#
# Druhy projekt pre predmet IPK
# Prenos suborov
#
# @author xormos00, Michal Ormos
# xormos00@stud.fit.vutbr.cz
# @date 9.4.2016
###

#prelozenie suboru
make

#Pri opatovnom spusteni skriptu vymaz jeho predoslu cinnost
rm -rf clientDir serverDir

#vytvorenie demonstracnych suborov a prieconkov
mkdir -p clientDir/dir
echo "upload" > clientDir/upload.txt
echo "upload2" > clientDir/dir/upload2.txt
mkdir -p serverDir/dir
echo "download" > serverDir/download.txt
echo "download2" > serverDir/dir/download2.txt

# Spustenie serveru
cp server serverDir
cd serverDir
./server -p 1999 &
pidServer=$!

cd -
cp client clientDir
cd clientDir

# Validne testy
echo "-------------------------------------------------------------------------"
echo "Validne testy"
./client -p 1999 -h localhost -u upload.txt
./client -p 1999 -h localhost -d download.txt

./client -p 1999 -h localhost -u dir/upload2.txt
./client -p 1999 -h localhost -d dir/download2.txt

echo "Specialne testy"
./client -p 1999 -h localhost -u ../temp/empty_file.txt
./client -p 1999 -h localhost -d ../temp/fit_logo.png

################################################################################

# Nevalidne testy
echo "--------------------------------------------------------------------------"
echo "Nevalidne testy"
# Neznamy port
./client -p -1 -h localhost -d none
# Neznamy server
./client -p 1999 -h none -d none
# Neznamy subor
./client -p 1999 -h localhost -u none
./client -p 1999 -h localhost -d none

# Vypnutie serveru
kill $pidServer
