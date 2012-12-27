HEADERS += usagi/sensors.hxx
SOURCES += cpu_temperature.cxx
QMAKE_CXXFLAGS += -std=c++11
QMAKE_LIBS += -lsensors
