

#CONFIG -= qt

TEMPLATE = subdirs

#TARGET = tenacitas.concurrent.headers

HEADERS += \
    ../../../../../code/formater/_bus/file_name_from_path.h  \
    ../../../../../code/formater/_bus/format_number.h \
    ../../../../../code/formater/_bus/max_str_length.h

include (../../../common.pri)


