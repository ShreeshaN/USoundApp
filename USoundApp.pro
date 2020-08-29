QT              += core gui  widgets

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 debug

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Adding c++17 support
CONFIG += c++17
CONFIG += qwt

TARGETDIR = ''

macx {
  QMAKE_CXXFLAGS += -F/Library/Frameworks
  QMAKE_LFLAGS   += -F/Library/Frameworks
  LIBS           += -framework HALCONCpp
  TARGETDIR      += $$OUT_PWD/USoundApp.app/Contents/MacOS/
  PWD_Settings = $${PWD}/USoundSettings.ini
  PWD_CameraParams = $${PWD}/CameraParameters.ini
  DESTDIR_US = $${TARGETDIR}
}
else {
  #defines
  win32:DEFINES += WIN32

  #includes
  INCLUDEPATH   += "$$(HALCONROOT)/include"
  INCLUDEPATH   += "$$(HALCONROOT)/include/halconcpp"

  #libs
  QMAKE_LIBDIR  += "$$(HALCONROOT)/lib/$$(HALCONARCH)"
  QMAKE_LIBDIR  += "$$(HALCONROOT)/lib/$$(HALCONARCH)"
  unix:LIBS     += -lhalconcpp -lhalcon -lXext -lX11 -ldl -lpthread
  win32:LIBS    += "$$(HALCONROOT)/lib/$$(HALCONARCH)/halconcpp.lib" \
                   "$$(HALCONROOT)/lib/$$(HALCONARCH)/halcon.lib"

  TARGETDIR      += $$OUT_PWD

  PWD_Settings = $${PWD}/USoundSettings.ini
  PWD_CameraParams = $${PWD}/CameraParameters.ini
  DESTDIR_US = $${TARGETDIR}
  PWD_Settings ~= s,/,\\,g
  PWD_CameraParams ~= s,/,\\,g
  DESTDIR_US ~= s,/,\\,g
}


SOURCES += \
    checkboxcontainer.cpp \
    defaultcameraparameternames.cpp \
    defaults.cpp \
    doublespinboxcontainer.cpp \
    histogramwindow.cpp \
    imageacquisition.cpp \
    imagestreamwindow.cpp \
    main.cpp \
    homescreen.cpp \
    parametercontainer.cpp \
    queuewriter.cpp \
    settings.cpp \
    settingsstore.cpp \
    spinboxcontainer.cpp

HEADERS += \
    checkboxcontainer.h \
    defaultcameraparameternames.h \
    defaults.h \
    doubleslider.h \
    doublespinboxcontainer.h \
    histogramwindow.h \
    intslider.h \
    settings.h \
    settingsstore.h \
    singleton.h \
    spinboxcontainer.h \
    homescreen.h \
    imageacquisition.h \
    imagestreamwindow.h \
    parametercontainer.h \
    queuewriter.h \
    usoundutils.h

FORMS += \
    about.ui \
    homescreen.ui \
    settings.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    icons.qrc \
    images.qrc \
    USoundSettings.ini \
    CameraParameters.ini

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../../../Program Files/MVTec/HALCON-19.11-Progress/lib/x64-win64/' -lhalconcpp
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../../../Program Files/MVTec/HALCON-19.11-Progress/lib/x64-win64/' -lhalconcppd
#else:unix: LIBS += -L$$PWD/'../../../../../Program Files/MVTec/HALCON-19.11-Progress/lib/x64-win64/' -lhalconcpp

#INCLUDEPATH += $$PWD/'../../../../../Program Files/MVTec/HALCON-19.11-Progress/include/halconcpp'
#DEPENDPATH += $$PWD/'../../../../../Program Files/MVTec/HALCON-19.11-Progress/include/halconcpp'

# MacOS specific settings. Note that while dyld will search under
# /Library/Frameworks by default, the preprocessor/compiler/linker will not
# and need to be told explicitly.

DISTFILES += \
    USoundSettings.ini \
    CameraParameters.ini \

QT += charts

# Copy INI Files
#mkdata.commands = $(MKDIR) $${DESTDIR_US}
copy.commands = $(COPY_FILE) $${PWD_Settings} $${DESTDIR_US} &&  $(COPY_FILE) $${PWD_CameraParams} $${DESTDIR_US}
first.depends = $(first)  copy #mkdata before copydata
export(first.depends)
#export(mkdata.commands)
export(copy.commands)
QMAKE_EXTRA_TARGETS += first copy #mkdata before copydata


