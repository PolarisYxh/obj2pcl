#-------------------------------------------------
#
# Project created by QtCreator 2018-03-20T15:09:18
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = osgqt2_1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        osgqt.cpp \
    qtosgview.cpp \
    mytreewidget.cpp \
    mytreewidget2.cpp \
    AnimationTest.cpp \
    DOFCalculation.cpp \
    DOFAdder.cpp

HEADERS += \
        osgqt.h \
    structures.h \
    qtosgview.h \
    mytreewidget.h \
    mytreewidget2.h \
    AnimationTest.h \
    DOFCalculation.h \
    DOFAdder.h

FORMS += \
        osgqt.ui
INCLUDEPATH += D:/matlab/extern/include
DEPENDPATH += D:/matlab/extern/include
INCLUDEPATH += D:/matlab/extern/lib/win64/microsoft
DEPENDPATH += D:/matlab/extern/lib/win64/microsoft

INCLUDEPATH += C:/OSGQT/include
DEPENDPATH +=  C:/OSGQT/include
win32:CONFIG(release, debug|release): LIBS += -LC:/OSGQT/lib/ -losgViewer  -losgDB -losgUtil  -losg  -lOpenThreads -losgGA -losgQt -losgWidget -losgAnimation -losgFX -losgManipulator -losgParticle -losgPresentation -losgShadow -losgSim -losgTerrain -losgText -losgUI -losgUtil -losgVolume
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/OSGQT/lib/ -losgViewerd  -losgDBd -losgUtild  -losgd  -lOpenThreadsd -losgGAd -losgQtd -losgWidgetd -losgAnimationd -losgFXd -losgManipulatord -losgParticled -losgPresentationd -losgShadowd -losgSimd -losgTerraind -losgTextd -losgUId -losgUtild -losgVolumed
else:unix: LIBS += -LC:/OSGQT/lib/ -losgViewer  -losgDB -losgUtil  -losg  -lOpenThreads -losgGA -losgQt -losgWidget -losgAnimation -losgFX -losgManipulator -losgParticle -losgPresentation -losgShadow -losgSim -losgTerrain -losgText -losgUI -losgUtil -losgVolume

win32:CONFIG(release, debug|release): LIBS += -LD:/matlab/extern/lib/win64/microsoft/ -llibeng -lmclmcrrt -lmclmcr -llibmx -llibmex
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/matlab/extern/lib/win64/microsoft/ -llibeng -lmclmcrrt -lmclmcr -llibmx -llibmex
else:unix: LIBS += -LD:/matlab/extern/lib/win64/microsoft/ -llibeng -lmclmcrrt -lmclmcr -llibmx -llibmex

RESOURCES += \
    image.qrc

