VERSION = 0.0.0

TEMPLATE = lib
QT -= gui
QT += network
CONFIG += no_keywords

TARGET  = single_core_application
DESTDIR = ../../lib
DEPENDPATH  += . ..
INCLUDEPATH += . ..

HEADERS += single_core_application.h   \
           single_application_impl.h   \
           application_server.h        \
           application_socket.h
SOURCES += single_core_application.cpp \
           single_application_impl.cpp \
           application_server.cpp      \
           application_socket.cpp
