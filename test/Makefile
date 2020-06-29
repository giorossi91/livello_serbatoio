SHELL=/bin/sh
.SUFFIXES:
.SUFFIXES: .cpp .o

CXXFLAGS:=-Wall -Wextra -O0 -g2 -ftest-coverage -fprofile-arcs
LDFLAGS=-lcppunit -lpthread -lgcov
EXE:=livello_serbatoio_testapp
OBJS:=livello_serbatoio_testapp.o livello_serbatoio_test.o

all:
	$(MAKE) pre-build
	$(MAKE) $(EXE)

pre-build:
	cp -f ../livello_serbatoio.ino livello_serbatoio_uut.hpp

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I. -I.. -MD -MP $< -c -o $@

$(EXE): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

check: $(EXE)
	$(RM) -f *.gcda
	$(RM) -f coverage/*
	./$(EXE)
	gcovr -r . --html --html-details -o coverage/coverage_report.html


clean:
	rm -f $(OBJS) $(EXE) *.d *.gcda *.gcno


.PHONY: all pre-build check clean 

-include $(OBJS:.o=.d)

