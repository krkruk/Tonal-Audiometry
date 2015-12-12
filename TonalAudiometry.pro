TEMPLATE = app

QT += qml quick multimedia core
CONFIG += c++11

SOURCES += main.cpp \
    sound.cpp \
    filesound.cpp \
    playlist.cpp \
    soundplayer.cpp \
    audiogram.cpp \
    appengine.cpp \
    volume.cpp

RESOURCES += qml.qrc \
    soundsamples.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    soundsample.h \
    sound.h \
    filesound.h \
    playlist.h \
    soundplayer.h \
    audiogram.h \
    appengine.h \
    volume.h \
    constants.h

