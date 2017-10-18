SRCS = Bucket.cpp SignedRandomProjection.cpp LSH.cpp
HF = Bucket.h HashFunction.h SignedRandomProjection.h LSH.h
ALL = clean experiment

all: $(ALL)

experiment:
	#g++ -fopenmp --std=c++11 -o experiment $(SRCS) main.cpp
	g++ -g --std=c++11 -o experiment $(SRCS) main.cpp
clean:
	rm -rf $(ALL) target/