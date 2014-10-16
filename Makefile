default : all;

test.out : all e0.tcl Makefile
	(make -C kernel kernel.img)
	expect e0.tcl | grep -v sgabios > test.out
	
run : Makefile test.out
	cat test.out

test : Makefile test.out
	(diff -b test.out test.ok > test.diff 2>&1 && echo pass) || (echo "\n\n:::::::: expected ::::::::"; cat test.ok ; echo "\n\n:::::::: found ::::::::" ; cat test.out)

% :
	(make -C kernel $@)
