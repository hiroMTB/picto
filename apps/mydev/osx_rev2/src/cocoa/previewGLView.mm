#import "previewGLView.h"
#include "testApp.h"
//#import "PictoUIController.h"

@implementation previewGLView

- (void)setup{}

- (void)update{}

- (void)draw{
    NSDictionary * defaults = [[NSUserDefaults standardUserDefaults] dictionaryRepresentation];
    
    glPushMatrix();
    testApp::getInstance()->drawPreview();
    
    glPopMatrix();
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
