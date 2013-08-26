#import "previewGLView.h"
#include "testApp.h"
//#import "PictoUIController.h"

@implementation previewGLView

- (void)setup{}

- (void)update{}

- (void)draw{
    NSDictionary * defaults = [[NSUserDefaults standardUserDefaults] dictionaryRepresentation];
    
//    NSString * message          = [[defaults objectForKey:@"Message"] stringValue];
    //    const char *cString = [message UTF8String];
    //    string s(cString);
    //    testApp::getInstance()->drawPreviewView(s);
    //    testApp::getInstance()->drawPreviewView("AFGE");

    
    glPushMatrix();
//    glScalef(0.2, 0.2, 0.2);
    testApp::getInstance()->drawPreviewView("s");
    
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
