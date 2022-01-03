# LinuxShell

Project for CS 3650 at Northeastern University where I created a working Bash-like Shell written in C. It is a bare bones shell that supports the following bash features: < (redirect input), > (redirect output), && (and), || (or), ; (execute consecutive commands) and the creation of environment variables.

#### Installation Instructions

1. Clone the repository
2. Make the makefile
3. Run the nush executable

The following are example commands supported by the shell:
mkdir -p tmp <br />
sort -o tmp/sorted.txt tests/sample.txt <br />
cat tmp/sorted.txt <br />
__or__ <br />
true&& 	echo dont 	need spaces <br />
__or__ <br />
mkdir -p tmp <br />
sort tests/sample.txt > tmp/sorted2.txt <br />
cat tmp/sorted2.txt <br />
__or__ <br />
FOO=one <br />
BAR=two <br />
echo $FOO $BAR $FOO <br />
FOO=three <br />
echo $BAR $FOO $BAR <br />

