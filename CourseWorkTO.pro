#-------------------------------------------------
#
# Project created by QtCreator 2017-11-01T10:37:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webenginewidgets

TARGET = CourseWorkTO
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        Net/buffer.cpp \
        Net/device.cpp \
        Net/filereceiver.cpp \
        Net/filesender.cpp \
        Net/socketfd.cpp \
        Net/tcpclient.cpp \
        Net/tcpserver.cpp \
        Net/tcpsocket.cpp \
        SysInfo/Subsystem.cpp \
        SysInfo/SubsystemCPU.cpp \
        SysInfo/SubsystemDMI.cpp \
        SysInfo/SubsystemFilesystem.cpp \
        SysInfo/SubsystemRAM.cpp \
        SysInfo/SubsystemGPU.cpp \
        SysInfo/System.cpp \
        SysInfo/viewer.cpp \
        SysInfo/SubsystemOS.cpp \
        SysInfo/SubsystemUSB.cpp \
        SysInfo/SubsystemNet.cpp \
    form.cpp \
    serverhw.cpp \
    senderfile.cpp \
    Threads/Runnable.cpp \
    Threads/Thread.cpp \
    Threads/ThreadPool.cpp \
    Tests/Queen.cpp \
    Tests/Test.cpp \
    Tests/Zipping.cpp \
    Utils/StringUtilities.cpp \
    Utils/HtmlUtils.cpp \
    basicgraphicswidget.cpp \
    SystemOverview.cpp \
    TestHardware.cpp \
    Tests/CPUOperations.cpp \
    Tests/HardDriveSpeed.cpp \
    Tests/RAMReadWriteSpeed.cpp \
    SaverFile.cpp \
    Utils/NetUtils.cpp

HEADERS += \
        Net/buffer.h \
        Net/device.h \
        Net/filereceiver.h \
        Net/filesender.h \
        Net/socketfd.h \
        Net/tcpclient.h \
        Net/tcpserver.h \
        Net/tcpsocket.h \
        Net/file.h \
        Net/net.h \
        Net/typedefs.h \
        SysInfo/Subsystem.h \
        SysInfo/SubsystemCPU.h \
        SysInfo/SubsystemDMI.h \
        SysInfo/SubsystemFilesystem.h \
        SysInfo/SubsystemRAM.h \
        SysInfo/SubsystemGPU.h \
        SysInfo/System.h \
        SysInfo/viewer.h \
        SysInfo/All.h \
        SysInfo/SubsystemOS.h \
        SysInfo/SubsystemUSB.h \
        SysInfo/SubsystemNet.h \
        form.h \
    forms.h \
    serverhw.h \
    senderfile.h \
    Threads/Runnable.h \
    Threads/Thread.h \
    Threads/ThreadPool.h \
    Threads/MultithreadingConstant.h \
    Threads/All.h \
    Tests/Queen.h \
    Tests/DepsHeaders.h \
    Tests/TestRunner.h \
    Tests/Test.h \
    Tests/Zipping.h \
    utils.h \
    Utils/StringUtilities.h \
    Utils/HtmlUtils.h \
    basicgraphicswidget.h \
    SystemOverview.h \
    types.h \
    TestHardware.h \
    Tests/CPUOperations.h \
    Tests/HardDriveSpeed.h \
    Tests/RAMReadWriteSpeed.h \
    SaverFile.h \
    Utils/NetUtils.h

FORMS += \
    widget.ui \
    form.ui \
    testhw.ui \
    serverhw.ui \
    senderfile.ui

DISTFILES += \
    TODO.txt
