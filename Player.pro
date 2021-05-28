QT += quick \
      multimedia \
      quickcontrols2 \
      network

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        src/audioplayermodel.cpp \
        src/person.cpp \
        src/audioplayercontroller.cpp \
        src/autor.cpp \
        src/client.cpp \
        src/coautor.cpp \
        src/ftp.cpp \
        src/ftpclient.cpp \
        src/genre.cpp \
        src/serializer.cpp \
        src/song.cpp \


RESOURCES += qml.qrc

LIBS += -L"D:\Qt\Tools\OpenSSL\Win_x86\lib" -lpsapi
LIBS += -lws2_32


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = $$PWD/qml
QML2_IMPORT_PATH += $$PWD/qml

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/../shared/include


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    include/audioplayercontroller.h \
    include/audioplayermodel.h \
    include/autor.h \
    include/client.h \
    include/coautor.h \
    include/ftp.h \
    include/ftpclient.h \
    include/genre.h \
    include/person.h \
    include/serializer.h \
    include/song.h \
