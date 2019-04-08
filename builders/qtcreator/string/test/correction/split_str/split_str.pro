TEMPLATE = app
TARGET = tenacitas.string.test.correction.split_str
CONFIG+=test

QT -= core
QT -= widgets
QT -= network
QT -= gui




message("LIBS = $$LIBS")


SOURCES += \
    ../../../../../../code/string/test/correction/split_str/main.cpp


include (../../../../common.pri)



LIBS += -ltenacitas.logger -ltenacitas.string
