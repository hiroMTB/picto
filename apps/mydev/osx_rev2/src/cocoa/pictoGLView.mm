#import "pictoGLView.h"
#include "testApp.h"
#include "PictoUIController.h"

@implementation pictoGLView

- (void)setup{

    [PictoUIController setupDefault];

    string newPath = [[NSString stringWithFormat:@"%@/../data/", [[NSBundle mainBundle] bundlePath]] cString];
    ofSetDataPathRoot(newPath);

    
    testApp::w = ofGetWidth();
    testApp::h = ofGetHeight();
    
    testApp::init();
    testApp::getInstance()->setup();

}



- (void)update{
    
    testApp::update();
}

- (void)draw{
    testApp::draw();    
}

- (void)exit
{
//    testApp::getInstance()->exit();
}

- (void)keyPressed:(int)key
{
    testApp::getInstance()->keyPressed(key);
}

- (void)keyReleased:(int)key
{
    testApp::getInstance()->keyReleased(key);
}

- (void)mouseMoved:(NSPoint)p
{
    testApp::getInstance()->mouseMoved(p.x, p.y);
}

- (void)mouseDragged:(NSPoint)p button:(int)button
{
    testApp::getInstance()->mouseDragged(p.x, p.y, button);
}

- (void)mousePressed:(NSPoint)p button:(int)button
{
    testApp::getInstance()->mousePressed(p.x, p.y, button);
}

- (void)mouseReleased:(NSPoint)p button:(int)button
{
    testApp::getInstance()->mouseReleased(p.x, p.y, button);
}

- (void)windowResized:(NSSize)size
{
    testApp::getInstance()->windowResized(size.width, size.height);
}


@end
