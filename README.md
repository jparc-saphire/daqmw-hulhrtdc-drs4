# daqmw-hulhrtdc-drs4

DAQ-Middleware system for HUL-HRTDC and DRS4 readout, with setup scripts and fixes for AlmaLinux 10.

## System Overview

```
Drs4QdcReader1  ──┐
MznHRTdcReader1 ──┤
MznHRTdcReader2 ──┼─→ Merger → BestEffortDispatcher → DAQLogger
MznHRTdcReader3 ──┘                                 → Monitor
```

## Environment Setup

Tested on AlmaLinux 10.0 (Purple Lion) x86_64.

This project uses the same environment as `daqmw-hulhrtdc`. If dependencies are not yet installed:

```bash
bash setup/setup.sh 2>&1 | tee /tmp/setup_log.txt
```

This script automatically builds and installs:
omniORB 4.2.5, OpenRTM-aist 1.2.2, DAQ-Middleware libraries, TBB 2020.3 (for ROOT compatibility), and patches `run.py` for Python 3.

## Building

To build all components and tools:

```bash
make clean
make
```

## Usage

Start the DAQ system using the local boot mode:

```bash
run.py -l daq_work/config_all.xml
```

Refer to `daq_work/README` (if available) or the project structure for more details on configuration.
