
# ROOT's own flags select the C++ standard appropriate for its installation.
CXX = $(shell root-config --cxx)
CPPFLAGS += -Iinclude $(shell root-config --cflags)
CXXFLAGS += -O2 -Wall -Wextra -Wpedantic
LDLIBS += $(shell root-config --glibs)

SOURCES = main.cxx src/AnalysisCommon.cxx src/ReadData.cxx src/GivenDay.cxx \
          src/EveryDay.cxx src/YearExtremes.cxx src/CompareYears.cxx src/GraphStyle.cxx

.PHONY: all clean
all: temperature_analysis

temperature_analysis: $(SOURCES) include/Analysis.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SOURCES) -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f temperature_analysis
