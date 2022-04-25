part1:
	g++ -std=c++11 -DNDEBUG -g -I ~/include -I . -L ~/lib SA-driver.cpp SuffixArray.cpp fasta/fasta.c -o buildsa -lsdsl -ldivsufsort -ldivsufsort64
	./buildsa --prefix 3 example/small.fa output
part2:
	g++ -std=c++11 -DNDEBUG -g -I ~/include -I . -L ~/lib querysa-driver.cpp SuffixArray.cpp  querysa.cpp fasta/fasta.c -o querysa -lsdsl -ldivsufsort -ldivsufsort64
	./querysa --index output  --queries example/query_small.fa --query_mode naive --output results
clean:
	rm buildsa
	rm querysa
