OF = $$PWD/../../..

OFX_QTGL_WIDGET = $$OF/addons/ofxQtGLWidget/src
INCLUDEPATH *= $$OFX_QTGL_WIDGET
HEADERS += $$OFX_QTGL_WIDGET/ofxQtGLWidget.h

SOURCES += $$OFX_QTGL_WIDGET/ofxQtGLWidget.cpp


OFX_SVG =   $$OF/addons/ofxSvg
INCLUDEPATH *= $$OFX_SVG/src
INCLUDEPATH *= $$OFX_SVG/libs/svgTiny/src
HEADERS +=  $$OFX_SVG/src/ofxSvg.h\
            $$files($$OFX_SVG/libs/svgTiny/src/*.h)
SOURCES +=  $$OFX_SVG/src/ofxSvg.cpp\
            $$files($$OFX_SVG/libs/svgTiny/src/*.cpp)


OFX_XML =   $$OF/addons/ofxXmlSettings
INCLUDEPATH *= $$OFX_XML/src
INCLUDEPATH *= $$OFX_XML/libs
HEADERS +=  $$OFX_XML/src/ofxXmlSettings.h\
            $$files($$OFX_XML/libs/*.h)
SOURCES +=  $$OFX_XML/src/ofxXmlSettings.cpp\
            $$files($$OFX_XML/libs/*.cpp)


