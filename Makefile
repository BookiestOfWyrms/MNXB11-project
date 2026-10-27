
# ROOT's own flags select the C++ standard appropriate for its installation.
CXX = $(shell root-config --cxx)
CPPFLAGS += $(shell root-config --cflags)
CXXFLAGS += -O2 -Wall -Wextra -Wpedantic
LDLIBS += $(shell root-config --glibs)

SOURCES = main.cxx AnalysisCommon.cxx ReadData.cxx GivenDay.cxx \
          EveryDay.cxx YearExtremes.cxx CompareYears.cxx

.PHONY: all clean
all: temperature_analysis

temperature_analysis: $(SOURCES) Analysis.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SOURCES) -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f temperature_analysis
