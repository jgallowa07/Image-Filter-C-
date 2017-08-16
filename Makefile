
SRC=myRunner.C image.C sink.C source.C PNMreader.C PNMwriter.C filters.C logging.C 
OBJ=$(SRC:.C=.o)

prog: $(OBJ)
	g++ $(OBJ) -o stress_test

.C.o: $<
	g++  -g -I. -c $<

clean:
	rm *.o stress_test 3H.pn*

run:
	./stress_test blah blah
	pnmtopng 3H.pnm > 3H.png
	open 3H.png
