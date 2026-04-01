all:
	mkdir -p lib bin
	mkdir -p vme_drs_v5.1_tdc/lib vme_drs_v5.1_tdc/bin
	mkdir -p vme_drs_v5.1_tdc/common/lib vme_drs_v5.1_tdc/drs4/lib
	mkdir -p Analyzer/lib Analyzer/bin
	$(MAKE) -C vme_drs_v5.1_tdc all;
	$(MAKE) -C daq-mw-app/util all;
	$(MAKE) -C daq-mw-app/component/MznHRTdcReader1/standalone/src all;
	$(MAKE) -C daq-mw-app/component/MznHRTdcReader2/standalone/src all;
	$(MAKE) -C daq-mw-app/component/MznHRTdcReader3/standalone/src all;
	$(MAKE) -C daq-mw-app/component all;
	$(MAKE) -C Analyzer/src all;
clean:
	$(MAKE) -C vme_drs_v5.1_tdc clean;
	$(MAKE) -C daq-mw-app/util clean;
	$(MAKE) -C daq-mw-app/component/MznHRTdcReader1/standalone/src clean;
	$(MAKE) -C daq-mw-app/component/MznHRTdcReader2/standalone/src clean;
	$(MAKE) -C daq-mw-app/component/MznHRTdcReader3/standalone/src clean;
	$(MAKE) -C daq-mw-app/component clean;
	$(MAKE) -C Analyzer/src clean;
