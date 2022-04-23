build:
	g++ -std=c++11 -DNDEBUG -g -I ~/include -I . -L ~/lib SA-driver.cpp SuffixArray.cpp fasta/fasta.c -o buildsa -lsdsl -ldivsufsort -ldivsufsort64
	./buildsa --prefix 3 example/ecoli.fa output.txt
clean:
	rm buildsa
