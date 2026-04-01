TGT = check_drs4_run0e_e16 ana_rpccosmic merge_mrpc_trig check_tag_trig check_tag_HRTDC

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

check_drs4_run0e_e16:check_drs4_run0e_e16.o
	g++ -ggdb $(ROOT_INC) -o $@ $^ $(ROOT_LIBS) -lMinuit
#	g++ -Ofast $(ROOT_INC) -o $@ $^ $(ROOT_LIBS) -lMinuit

merge_mrpc_trig:merge_mrpc_trig.o
	g++ -ggdb $(ROOT_INC) -o $@ $^ $(ROOT_LIBS) -lMinuit
#	g++ -Ofast $(ROOT_INC) -o $@ $^ $(ROOT_LIBS) -lMinuit

check_tag_trig:check_tag_trig.o
	g++ -ggdb $(ROOT_INC) -o $@ $^ $(ROOT_LIBS) -lMinuit
#	g++ -Ofast $(ROOT_INC) -o $@ $^ $(ROOT_LIBS) -lMinuit

check_tag_HRTDC:check_tag_HRTDC.o
	g++ -ggdb $(ROOT_INC) -o $@ $^ $(ROOT_LIBS) -lMinuit
#	g++ -Ofast $(ROOT_INC) -o $@ $^ $(ROOT_LIBS) -lMinuit


.C.o:
	g++ -c $(ROOT_INC) $< -D__OUTPUT_ROOTFILE__ -Ofast
#	g++ -c $(ROOT_INC) $< -D__OUTPUT_ROOTFILE__ -ggdb

clean:
	$(RM) *~ $(TGT) *.o
