# LinuxShell

Project for CS 3650 at Northeastern University where I created a working Bash-like Shell written in C. It is a bare bones shell that supports the following bash features: < (redirect input), > (redirect output), && (and), || (or), ; (execute consecutive commands) and the creation of environment variables.

An example series of command supported by the shell is 
mkdir -p tmp
sort -o tmp/sorted.txt tests/sample.txt
cat tmp/sorted.txt
__or__
true&& 	echo dont 	need spaces
__or__
mkdir -p tmp
sort tests/sample.txt > tmp/sorted2.txt
cat tmp/sorted2.txt
__or__
FOO=one
BAR=two
echo $FOO $BAR $FOO
FOO=three
echo $BAR $FOO $BAR

