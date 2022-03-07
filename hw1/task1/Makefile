run:
	g++ -std=c++11 -DNDEBUG -g -I ~/include -L ~/lib main.cpp NaiveRankSupport.cpp RankSupport.cpp SelectSupport.cpp SparseArray.cpp -o rank -lsdsl -ldivsufsort -ldivsufsort64
	./rank
clean:
	rm rank
