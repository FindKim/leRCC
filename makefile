all: leRCC

leRCC: main.o ExtractSigOrf.o ExtractMMSeq.o RunLength.o MinValue.o SeqLength.o RunSum.o
	g++ main.o ExtractSigOrf.o ExtractMMSeq.o RunLength.o MinValue.o SeqLength.o RunSum.o -o leRCC

main.o: main.cpp
	g++ -c main.cpp

ExtractSigOrf.o: ExtractSigOrf.cpp ExtractSigOrf.h
	g++ -c ExtractSigOrf.cpp

ExtractMMSeq.o: ExtractMMSeq.cpp ExtractMMSeq.h
	g++ -c ExtractMMSeq.cpp

RunLength.o: RunLength.cpp RunLength.h
	g++ -c RunLength.cpp

MinValue.o: MinValue.cpp MinValue.h
	g++ -c MinValue.cpp

SeqLength.o: SeqLength.cpp SeqLength.h
	g++ -c SeqLength.cpp

RunSum.o: RunSum.cpp RunSum.h
	g++ -c RunSum.cpp

clean:
	rm -f *.o *.pyc *~ leRCC
