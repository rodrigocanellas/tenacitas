CONFIG -= qt


# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target


TEMPLATE = lib

VERSION = 0.0.1

TARGET = tenacitas.calendar.entities


HEADERS += \
  ../../../../../code/calendar/entities/amount.h \
  ../../../../../code/calendar/entities/day.h \
  ../../../../../code/calendar/entities/hour.h \
  ../../../../../code/calendar/entities/minute.h \
  ../../../../../code/calendar/entities/month.h \
  ../../../../../code/calendar/entities/second.h \
  ../../../../../code/calendar/entities/timestamp.h \
  ../../../../../code/calendar/entities/timestamp_second.h \
  ../../../../../code/calendar/entities/weekday.h \
  ../../../../../code/calendar/entities/year.h


SOURCES += \
  ../../../../../code/calendar/entities/internal/day.cpp \
  ../../../../../code/calendar/entities/internal/hour.cpp \
  ../../../../../code/calendar/entities/internal/minute.cpp \
  ../../../../../code/calendar/entities/internal/month.cpp \
  ../../../../../code/calendar/entities/internal/second.cpp \
  ../../../../../code/calendar/entities/internal/weekday.cpp



include (../../../common.pri)
