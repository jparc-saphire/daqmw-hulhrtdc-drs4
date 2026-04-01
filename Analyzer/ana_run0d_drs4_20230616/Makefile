TGT = check_ac ana_run0d_drs4_1

ROOT_INC  = `root-config --cflags`
ROOT_LIBS = `root-config --libs`

#CXXFLAG = -Wall -Ofast $(ROOT_INC)
#CPPFLAGS += -D__OUTPUT_ROOTFILE__ -Ofast
CPPFLAGS += -D__OUTPUT_ROOTFILE__ -ggdb
#CXXFLAG += -D__OUTPUT_ROOTFILE__ -ggdb

CPPFLAGS += -D__OUTPUT_ROOTFILE__
CXXFLAG += -D__OUTPUT_ROOTFILE__
all: $(TGT)

ana_run0d_drs4_1:ana_run0d_drs4_1.o
	g++ -Ofast $(ROOT_INC) -o $@ $^ $(ROOT_LIBS) -lMinuit

check_ac:check_ac.o
#	g++ -ggdb $(ROOT_INC) -o $@ $^ $(ROOT_LIBS) -lMinuit
	g++ -Ofast $(ROOT_INC) -o $@ $^ $(ROOT_LIBS) -lMinuit


.C.o:
#	g++ -c $(ROOT_INC) $< -D__OUTPUT_ROOTFILE__ -Ofast
	g++ -c $(ROOT_INC) $< -D__OUTPUT_ROOTFILE__ -ggdb

clean:
	$(RM) *~ $(TGT) *.o
