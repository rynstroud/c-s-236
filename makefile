# lab number
NUM?=5

# all of the input/output file numbers that it will run with
numbers=0 1 2 3 5 6 7

# variable for folder containing all I/O
# tests=project$(NUM)-exampleIO
BUCKET?=$(NUM)-80
tests=Lab$(NUM)PassOffCases/$(BUCKET)

run:
	for number in $(numbers); \
	do \
		./lab$(NUM) $(tests)/input$$number.txt > $(tests)/out.txt ; \
		diff $(tests)/answer$$number.txt $(tests)/out.txt || (echo "diff failed on test $$number \n") ; \
	done
	rm $(tests)/out.txt

compile:
	g++ -Wall -Werror -std=c++17 -g *.cpp -o lab$(NUM)


