QT = core
TEMPLATE = app
CONFIG += console
TARGET = testdriver
SOURCES += testdriver.cpp \
    ReadTester.cpp

HEADERS += \
    ReadTester.h \
    testdriver.h

# Uncomment to regenerate test results
# DEFINES += UPDATE_RESULTS

include(../src/Qson.pri)
