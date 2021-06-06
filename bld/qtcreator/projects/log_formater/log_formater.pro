TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
TARGET = tenacitas.log_formater

include (../../common.pri)

SOURCES += \
        $$code_src_dir/log_formater/main.cpp
