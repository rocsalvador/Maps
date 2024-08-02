TEMPLATE    = app
QT         += opengl

DESTDIR  = build
OBJECTS_DIR = $$DESTDIR/obj
MOC_DIR = $$DESTDIR/moc
RCC_DIR = $$DESTDIR/qrc
UI_DIR = $$DESTDIR/ui

INCLUDEPATH +=  /usr/include/glm src include/pugixml/src/

FORMS += ui/Form.ui

HEADERS += src/Form.hh src/GLWidget.hh src/Data.hh include/pugixml/src/pugiconfig.hpp

QMAKE_CXXFLAGS += -g -finstrument-functions

SOURCES += src/main.cc src/Form.cc src/GLWidget.cc src/Data.cc include/pugixml/src/pugixml.cpp
