include($${PWD}/../common.pri)
QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    lrTabWidget/lrTabBar.cpp \
    lrTabWidget/lrTabPage.cpp \
    lrTabWidget/lrTabPopup.cpp \
    lrTabWidget/lrTabWidget.cpp \
    main.cpp \
    mainwindow.cpp \
    report/datasourceconfigmgr.cpp \
    report/reportconfigmgr.cpp \
    report/reportdatabaseconnection.cpp \
    report/reportfilemgr.cpp \
    report/reportformatmgr.cpp \
    reporttemplatefilemgr.cpp \
    reporttemplatepage.cpp \
    reporttemplatepagemgr.cpp \
    templateconfigmgr.cpp \
    utils/commonutils.cpp \
    utils/jsonhandler.cpp \
    utils/pmfileutils.cpp \
    utils/pminifile.cpp \
    utils/pmprogressbardialog.cpp \
    utils/pmprojectpath.cpp \
    utils/pmquazipcompress.cpp

HEADERS += \
    lrTabWidget/lrTabBar.h \
    lrTabWidget/lrTabPage.h \
    lrTabWidget/lrTabPopup.h \
    lrTabWidget/lrTabWidget.h \
    mainwindow.h \
    report/datasourceconfigmgr.h \
    report/reportconfigmgr.h \
    report/reportdatabaseconnection.h \
    report/reportfilemgr.h \
    report/reportformatmgr.h \
    reporttemplatefilemgr.h \
    reporttemplatepage.h \
    reporttemplatepagemgr.h \
    templateconfigmgr.h \
    utils/commonutils.h \
    utils/debugtrap.h \
    utils/jsonhandler.h \
    utils/pmfileutils.h \
    utils/pminifile.h \
    utils/pmprogressbardialog.h \
    utils/pmprojectpath.h \
    utils/pmquazipcompress.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += $$PWD/../include
DEPENDPATH  += $$PWD/../include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

CONFIG(debug, debug|release): BASEDIR = $${PWD}/Debug
else: CONFIG(release, debug|release): BASEDIR = $${PWD}/Release

# put all .o files in a separate folder
OBJECTS_DIR = $${BASEDIR}/build_obj
# separate moc files
MOC_DIR = $${BASEDIR}/moc_files
# target file in the bin folder.
# DESTDIR = $${BASEDIR}/bin
DESTDIR = $${BASEDIR}/../output


LIBS += -L$${DEST_LIBS}
CONFIG(debug, debug|release) {
    LIBS += -llimereportd
} else {
    LIBS += -llimereport
}

!CONFIG(static_build) : CONFIG(zint) {
    LIBS += -L$${DEST_LIBS}
    CONFIG(debug, debug|release) {
        LIBS += -lQtZintd
    } else {
        LIBS += -lQtZint
    }
}

LIBS += -L$$PWD/../../quazip-master/build/quazip/ -lquazip1-qt5
unix::LIBS += -lz

include($$PWD/../../quazip-master/quazip/quazip.pri)

RESOURCES += \
    res.qrc
