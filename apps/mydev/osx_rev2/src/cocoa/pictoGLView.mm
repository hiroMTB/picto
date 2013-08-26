#import "pictoGLView.h"
#include "testApp.h"
#include "PictoUIController.h"

@implementation pictoGLView

- (void)setup{

    [PictoUIController setupDefault];

    string newPath = [[NSString stringWithFormat:@"%@/../data/", [[NSBundle mainBundle] bundlePath]] cString];
    ofSetDataPathRoot(newPath);

    
    testApp::init();
    testApp::getInstance()->setup();

    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(didEnterFull:)
                                                 name:NSWindowDidEnterFullScreenNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(didExitFull:)
                                                 name:NSWindowDidExitFullScreenNotification
                                               object:nil];
    
    testApp::getInstance()->w = ofGetWidth();
    testApp::getInstance()->h = ofGetHeight();
    
}

- (void)didEnterFull:(NSNotification *)notif{
    [self reshape];
}

- (void)didExitFull:(NSNotification *)notif
{
    [self reshape];
}

- (void)update{
    
    testApp::getInstance()->update();
}

- (void)draw{
    testApp::getInstance()->draw();    
}

- (void)exit
{
    testApp::getInstance()->exit();
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
