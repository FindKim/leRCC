all: leRCC

leRCC: main.o ExtractSigOrf.o ExtractMMSeq.o Length_vs_Runs.o
	g++ main.o ExtractSigOrf.o ExtractMMSeq.o Length_vs_Runs.o -o leRCC

main.o: main.cpp
	g++ -c main.cpp

ExtractSigOrf.o: ExtractSigOrf.cpp ExtractSigOrf.h
	g++ -c ExtractSigOrf.cpp

ExtractMMSeq.o: ExtractMMSeq.cpp ExtractMMSeq.h
	g++ -c ExtractMMSeq.cpp

Length_vs_Runs.o: Length_vs_Runs.cpp Length_vs_Runs.h
	g++ -c Length_vs_Runs.cpp

clean:
	rm -f *.o *.pyc *~ leRCC
