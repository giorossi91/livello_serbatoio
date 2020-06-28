CC=g++
CFLAGS=-Wall -Wextra -O0 -g2
LIB=-lcppunit -lpthread -lgcov
INC=-I. -I..
BUILDDIR=build

all: env
	cp -f ../livello_serbatoio.ino livello_serbatoio_uut.hpp
	$(CC) -c $(CFLAGS) -ftest-coverage -fprofile-arcs $(INC) -o $(BUILDDIR)/livello_serbatoio_test.o livello_serbatoio_test.cpp
	$(CC) -c $(CFLAGS) $(INC) -o $(BUILDDIR)/livello_serbatoio_testapp.o livello_serbatoio_testapp.cpp
	$(CC)    $(CFLAGS)        -o livello_serbatoio_testapp $(BUILDDIR)/livello_serbatoio_testapp.o $(BUILDDIR)/livello_serbatoio_test.o $(LIB)

check: all
	./livello_serbatoio_testapp
	cd $(BUILDDIR)
	gcovr -r .. --html --html-details -o coverage/coverage_report.html
	cd ..

env:
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)
	rm -f livello_serbatoio_testapp
