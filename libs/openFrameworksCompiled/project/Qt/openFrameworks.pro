DEFINES += __WIN32__
DEFINES += WIN32
DEFINES += _CONSOLE
DEFINES += POCO_STATIC
DEFINES += CAIRO_WIN32_STATIC_BUILD
DEFINES += DISABLE_SOME_FLOATING_POINT
DEFINES +=_UNICODE
DEFINES +=UNICODE

OF = $$PWD/../../../../

TEMPLATE  = lib
CONFIG   += staticlib

CONFIG(release, debug|release) {
    #This is a release build
    TARGET    = openFrameworksLib
    DEFINES += NDEBUG
} else {
    #This is a debug build
    TARGET    = openFrameworksLibDebug
    DEFINES += _DEBUG

}

DESTDIR   = $$OF/libs/openFrameworksCompiled/lib/Qt

CONFIG -= qt

#QMAKE_CXXFLAGS += -std=c++11
#CONFIG -= warn_on
#QMAKE_CXXFLAGS_DEBUG   = -g -O0 -MDd
#QMAKE_CXXFLAGS_RELEASE = -O3 -MD

QMAKE_LFLAGS += -static-libgcc
QMAKE_LFLAGS += -static-libstdc++

INCLUDEPATH *= $$OF/libs/glew/include
INCLUDEPATH *= $$OF/libs/glu/include
INCLUDEPATH *= $$OF/libs/glut/include

INCLUDEPATH *= $$OF/libs/openFrameworks/types
INCLUDEPATH *= $$OF/libs/openFrameworks/video
INCLUDEPATH *= $$OF/libs/openFrameworks/gl
INCLUDEPATH *= $$OF/libs/openFrameworks/math
INCLUDEPATH *= $$OF/libs/openFrameworks
INCLUDEPATH *= $$OF/libs/openFrameworks/graphics
INCLUDEPATH *= $$OF/libs/openFrameworks/events
INCLUDEPATH *= $$OF/libs/openFrameworks/sound
INCLUDEPATH *= $$OF/libs/openFrameworks/communication
INCLUDEPATH *= $$OF/libs/openFrameworks/utils
INCLUDEPATH *= $$OF/libs/openFrameworks/app
INCLUDEPATH *= $$OF/libs/openFrameworks/3d

INCLUDEPATH *= $$OF/libs/rtAudio/include
INCLUDEPATH *= $$OF/libs/poco/include
INCLUDEPATH *= $$OF/libs/tess2/include
INCLUDEPATH *= $$OF/libs/openssl/include

INCLUDEPATH *= $$OF/libs/cairo/include
INCLUDEPATH *= $$OF/libs/cairo/include/libpng15
INCLUDEPATH *= $$OF/libs/cairo/include/cairo
INCLUDEPATH *= $$OF/libs/cairo/include/pixman-1
INCLUDEPATH *= $$OF/libs/fmodex/include

INCLUDEPATH *= $$OF/libs/quicktime/include
INCLUDEPATH *= $$OF/libs/videoInput/include
INCLUDEPATH *= $$OF/libs/FreeImage/include
INCLUDEPATH *= $$OF/libs/freetype/include
INCLUDEPATH *= $$OF/libs/freetype/include/freetype2


SOURCES *= $$OF/libs/openFrameworks/math/ofQuaternion.cpp
SOURCES *= $$OF/libs/openFrameworks/math/ofVec2f.cpp
SOURCES *= $$OF/libs/openFrameworks/math/ofMatrix4x4.cpp
SOURCES *= $$OF/libs/openFrameworks/math/ofVec4f.cpp
SOURCES *= $$OF/libs/openFrameworks/math/ofMath.cpp
SOURCES *= $$OF/libs/openFrameworks/math/ofMatrix3x3.cpp

SOURCES *= $$OF/libs/openFrameworks/app/ofAppRunner.cpp
SOURCES *= $$OF/libs/openFrameworks/app/ofAppGlutWindow.cpp

SOURCES *= $$OF/libs/openFrameworks/gl/ofFbo.cpp
SOURCES *= $$OF/libs/openFrameworks/gl/ofMaterial.cpp
SOURCES *= $$OF/libs/openFrameworks/gl/ofVboMesh.cpp

SOURCES *= $$OF/libs/openFrameworks/gl/ofShader.cpp
SOURCES *= $$OF/libs/openFrameworks/gl/ofTexture.cpp
SOURCES *= $$OF/libs/openFrameworks/gl/ofGLRenderer.cpp
SOURCES *= $$OF/libs/openFrameworks/gl/ofLight.cpp
SOURCES *= $$OF/libs/openFrameworks/gl/ofVbo.cpp


SOURCES *= $$OF/libs/openFrameworks/video/ofQtUtils.cpp
SOURCES *= $$OF/libs/openFrameworks/video/ofQuickTimeGrabber.cpp
SOURCES *= $$OF/libs/openFrameworks/video/ofQuickTimePlayer.cpp

SOURCES *= $$OF/libs/openFrameworks/video/ofDirectShowGrabber.cpp
SOURCES *= $$OF/libs/openFrameworks/video/ofVideoPlayer.cpp
SOURCES *= $$OF/libs/openFrameworks/video/ofVideoGrabber.cpp
SOURCES *= $$OF/libs/openFrameworks/sound/ofOpenALSoundPlayer.cpp
SOURCES *= $$OF/libs/openFrameworks/sound/ofRtAudioSoundStream.cpp
SOURCES *= $$OF/libs/openFrameworks/sound/ofSoundPlayer.cpp
SOURCES *= $$OF/libs/openFrameworks/sound/ofSoundStream.cpp
SOURCES *= $$OF/libs/openFrameworks/sound/ofFmodSoundPlayer.cpp
SOURCES *= $$OF/libs/openFrameworks/events/ofEvents.cpp
SOURCES *= $$OF/libs/openFrameworks/types/ofBaseTypes.cpp
SOURCES *= $$OF/libs/openFrameworks/types/ofRectangle.cpp

SOURCES *= $$OF/libs/openFrameworks/types/ofColor.cpp

SOURCES *= $$OF/libs/openFrameworks/communication/ofSerial.cpp
SOURCES *= $$OF/libs/openFrameworks/communication/ofArduino.cpp

SOURCES *= $$OF/libs/openFrameworks/3d/ofMesh.cpp
SOURCES *= $$OF/libs/openFrameworks/3d/ofNode.cpp
SOURCES *= $$OF/libs/openFrameworks/3d/ofCamera.cpp
SOURCES *= $$OF/libs/openFrameworks/3d/of3dUtils.cpp
SOURCES *= $$OF/libs/openFrameworks/3d/ofEasyCam.cpp
SOURCES *= $$OF/libs/openFrameworks/graphics/ofTessellator.cpp
SOURCES *= $$OF/libs/openFrameworks/graphics/ofCairoRenderer.cpp
SOURCES *= $$OF/libs/openFrameworks/graphics/ofImage.cpp

SOURCES *= $$OF/libs/openFrameworks/graphics/ofPixels.cpp

SOURCES *= $$OF/libs/openFrameworks/graphics/ofGraphics.cpp
SOURCES *= $$OF/libs/openFrameworks/graphics/ofTrueTypeFont.cpp
SOURCES *= $$OF/libs/openFrameworks/graphics/ofPath.cpp
SOURCES *= $$OF/libs/openFrameworks/graphics/ofBitmapFont.cpp
SOURCES *= $$OF/libs/openFrameworks/graphics/ofPolyline.cpp
SOURCES *= $$OF/libs/openFrameworks/utils/ofURLFileLoader.cpp
SOURCES *= $$OF/libs/openFrameworks/utils/ofUtils.cpp
SOURCES *= $$OF/libs/openFrameworks/utils/ofLog.cpp
SOURCES *= $$OF/libs/openFrameworks/utils/ofSystemUtils.cpp
SOURCES *= $$OF/libs/openFrameworks/utils/ofFileUtils.cpp

SOURCES *= $$OF/libs/openFrameworks/utils/ofThread.cpp

