CONFIG(release, debug|release) {
    DEFINES += NDEBUG
} else {
    DEFINES += _DEBUG
}

DEFINES += WIN32
DEFINES += _CONSOLE
DEFINES += POCO_STATIC
DEFINES += CAIRO_WIN32_STATIC_BUILD
DEFINES += DISABLE_SOME_FLOATING_POINT
DEFINES += _UNICODE
DEFINES += UNICODE


# .obj directory
CONFIG(release, debug|release) {
    OBJECTS_DIR = ../obj/release
}else{
    OBJECTS_DIR = ../obj/debug
}


OF = $$PWD/../../..

INCLUDEPATH *= $$OF/libs/glew/include
INCLUDEPATH *= $$OF/libs/glu/include
INCLUDEPATH *= $$OF/libs/glut/include
INCLUDEPATH *= $$OF/libs/cairo/include
INCLUDEPATH *= $$OF/libs/cairo/include/libpng15
INCLUDEPATH *= $$OF/libs/cairo/include/pixman-1
INCLUDEPATH *= $$OF/libs/cairo/include/cairo
INCLUDEPATH *= $$OF/libs/fmodex/include
INCLUDEPATH *= $$OF/libs/kiss/include
INCLUDEPATH *= $$OF/libs/openssl/include
INCLUDEPATH *= $$OF/libs/openssl/include/openssl
INCLUDEPATH *= $$OF/libs/poco/include
INCLUDEPATH *= $$OF/libs/portaudio/include
INCLUDEPATH *= $$OF/libs/rtAudio/include
INCLUDEPATH *= $$OF/libs/tess2/include
INCLUDEPATH *= $$OF/libs/quicktime/include
INCLUDEPATH *= $$OF/libs/videoInput/include
INCLUDEPATH *= $$OF/libs/FreeImage/include
INCLUDEPATH *= $$OF/libs/freetype/include
INCLUDEPATH *= $$OF/libs/freetype/include/freetype2
INCLUDEPATH *= $$OF/libs/openFrameworks
INCLUDEPATH *= $$OF/libs/openFrameworks/types
INCLUDEPATH *= $$OF/libs/openFrameworks/sound
INCLUDEPATH *= $$OF/libs/openFrameworks/video
INCLUDEPATH *= $$OF/libs/openFrameworks/3d
INCLUDEPATH *= $$OF/libs/openFrameworks/math
INCLUDEPATH *= $$OF/libs/openFrameworks/events
INCLUDEPATH *= $$OF/libs/openFrameworks/utils
INCLUDEPATH *= $$OF/libs/openFrameworks/gl
INCLUDEPATH *= $$OF/libs/openFrameworks/app
INCLUDEPATH *= $$OF/libs/openFrameworks/graphics
INCLUDEPATH *= $$OF/libs/openFrameworks/communication

#library search path (-L)
LIBS += -L$$OF/libs/glut/lib/vs2010
LIBS += -L$$OF/libs/glew/lib/vs2010
LIBS += -L$$OF/libs/glu/lib/vs2010
LIBS += -L$$OF/libs/FreeImage/lib/vs2010
LIBS += -L$$OF/libs/cairo/lib/vs2010
LIBS += -L$$OF/libs/cairo/lib/vs2010
LIBS += -L$$OF/libs/freetype/lib/vs2010
LIBS += -L$$OF/libs/quicktime/lib/vs2010
LIBS += -L$$OF/libs/quicktime/lib/vs2010
LIBS += -L$$OF/libs/quicktime/lib/vs2010
LIBS += -L$$OF/libs/videoInput/lib/vs2010
LIBS += -L$$OF/libs/tess2/lib/vs2010
LIBS += -L$$OF/libs/fmodex/lib/vs2010
LIBS += -L$$OF/libs/rtAudio/lib/vs2010
LIBS += -L$$OF/libs/poco/lib/vs2010
LIBS += -L$$OF/libs/poco/lib/vs2010
LIBS += -L$$OF/libs/poco/lib/vs2010
LIBS += -L$$OF/libs/poco/lib/vs2010
LIBS += -L$$OF/libs/openFrameworksCompiled/lib/Qt

# library list (-l)
LIBS += -lglut32
LIBS += -lglew32s
LIBS += -lglu32
LIBS += -lFreeImage
LIBS += -lcairo-static
LIBS += -lpixman-1
LIBS += -llibfreetype
LIBS += -lqtmlClient
LIBS += -lQTSClient
LIBS += -lRave
LIBS += -lvideoInput
LIBS += -ltess2
LIBS += -lfmodex_vc

CONFIG(release, debug|release) {
    LIBS += -lopenFrameworksLib
    LIBS += -lrtAudio
    LIBS += -lPocoNetmd
    LIBS += -lPocoUtilmd
    LIBS += -lPocoXMLmd
    LIBS += -lPocoFoundationmd
} else {
    LIBS += -lopenFrameworksLibDebug
    LIBS += -lrtAudioD
    LIBS += -lPocoNetmdd
    LIBS += -lPocoUtilmdd
    LIBS += -lPocoXMLmdd
    LIBS += -lPocoFoundationmdd
}


# MSVC library
LIBS += -lShell32
LIBS += -lMsimg32
LIBS += -lAdvapi32


# ignore .lib
QMAKE_LFLAGS_DEBUG += /NODEFAULTLIB:MSVCRT.lib
QMAKE_LFLAGS_DEBUG += /NODEFAULTLIB:PocoFoundationd.lib
QMAKE_LFLAGS_DEBUG += /NODEFAULTLIB:PocoFoundationmtd.lib
QMAKE_LFLAGS_DEBUG += /NODEFAULTLIB:PocoUtilmtd.lib
QMAKE_LFLAGS_DEBUG += /NODEFAULTLIB:PocoNetmtd.lib
QMAKE_LFLAGS_DEBUG += /NODEFAULTLIB:atlthunk.lib
QMAKE_LFLAGS_DEBUG += /NODEFAULTLIB:LIBC.lib
QMAKE_LFLAGS_DEBUG += /NODEFAULTLIB:LIBCMT


# POST process, copy *.dll
DLLS += ..\..\..\..\export\vs2010\*.dll
CONFIG(release, debug|release) {
    QMAKE_POST_LINK =xcopy /e /i /y "$$DLLS" "..\bin\release"
}else{
   QMAKE_POST_LINK =xcopy /e /i /y "$$DLLS" "..\bin\debug"
}



