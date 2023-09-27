QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DownParser.cpp \
    LL1Parser.cpp \
    ParserPreTask.cpp \
    PreTask.cpp \
    SemAnalyze.cpp \
    SynTaxTree.cpp \
    globals.cpp \
    lexical.cpp \
    lexinfo.cpp \
    main.cpp \
    mainwindow.cpp \
    parseinfo.cpp \
    seminfo.cpp

HEADERS += \
    DownParser.h \
    LL1Parser.h \
    ParserPreTask.h \
    PreTask.h \
    SemAnalyze.h \
    SynTaxTree.h \
    globals.h \
    lexical.h \
    lexinfo.h \
    mainwindow.h \
    parseinfo.h \
    seminfo.h

FORMS += \
    lexinfo.ui \
    mainwindow.ui \
    parseinfo.ui \
    seminfo.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
