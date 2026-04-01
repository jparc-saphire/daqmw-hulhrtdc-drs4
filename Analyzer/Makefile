TGT = check_drs4_run0e ana_rpccosmic

ROOT_INC  = `root-config --cflags`
ROOT_LIBS = `root-config --libs`

#CXXFLAG = -Wall -Ofast $(ROOT_INC)
CPPFLAGS += -D__OUTPUT_ROOTFILE__ -Ofast
#CPPFLAGS += -D__OUTPUT_ROOTFILE__ -ggdb
#CXXFLAG += -D__OUTPUT_ROOTFILE__ -ggdb

CPPFLAGS += -D__OUTPUT_ROOTFILE__
CXXFLAG += -D__OUTPUT_ROOTFILE__
all: $(TGT)

ana_rpccosmic:ana_rpccosmic.o
	g++ -Ofast $(ROOT_INC) -o $@ $^ $(ROOT_LIBS) -lMinuit

check_drs4_run0e:check_drs4_run0e.o
	g++ -ggdb $(ROOT_INC) -o $@ $^ $(ROOT_LIBS) -lMinuit
#	g++ -Ofast $(ROOT_INC) -o $@ $^ $(ROOT_LIBS) -lMinuit


.C.o:
	g++ -c $(ROOT_INC) $< -D__OUTPUT_ROOTFILE__ -Ofast
#	g++ -c $(ROOT_INC) $< -D__OUTPUT_ROOTFILE__ -ggdb

clean:
	$(RM) *~ $(TGT) *.o
