#import "previewGLView.h"
#include "testApp.h"
#import "pictoGLView.h"

@implementation previewGLView

- (void)setup{}

- (void)update{}

- (void)draw{
    if(![pictoGLView getWindowing]){
        testApp::getInstance()->drawPreview();
    }
}

- (void)exit{}
- (void)keyPressed:(int)key{}
- (void)keyReleased:(int)key{}
- (void)mouseMoved:(NSPoint)p{}
- (void)mouseDragged:(NSPoint)p button:(int)button{}
- (void)mousePressed:(NSPoint)p button:(int)button{}
- (void)mouseReleased:(NSPoint)p button:(int)button{}
- (void)windowResized:(NSSize)size{}

@end
