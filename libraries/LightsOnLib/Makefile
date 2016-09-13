Test: Test.o LightsOn.o ComsStation.o 
	g++ -g -o Test Test.o LightsOn.o ComsStation.o
	./Test

Test.o: ComsStation.o LightsOn.o
	g++ -g -c Test.cpp

ComsStation.o: ComsStation.h ComsStation.cpp
	g++ -g -c ComsStation.cpp

LightsOn.o: LightsOn.h LightsOn.cpp
	g++ -g -c LightsOn.cpp

clean:
	rm -f *.o *~ Test
