#import "pictoGLView.h"
#include "testApp.h"
#include "PictoUIController.h"
#include "gpuPictoString.h"
#import "AppDelegate.h"

@implementation pictoGLView


- (void)setup{

    [PictoUIController setupDefault];

    //
    // data folder settings
    //
    //string newPath = [[NSString stringWithFormat:@"%@/../data/", [[NSBundle mainBundle] bundlePath]] cString];
    //ofSetDataPathRoot(newPath);
    
    std::string path;
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    path = [[[NSBundle mainBundle] bundlePath] UTF8String];
    [pool release];
    ofSetDataPathRoot(path + "/Contents/Resources/data/");
    
    testApp::w = ofGetWidth();
    testApp::h = ofGetHeight();
    
    testApp::init();
    testApp::getInstance()->setup();

    
    
    // fullscrenn
//    [[NSNotificationCenter defaultCenter] addObserver:self
//                                         selector:@selector(willEnterFull:)
//                                             name:NSWindowWillEnterFullScreenNotification
//                                           object:nil];
//    [[NSNotificationCenter defaultCenter] addObserver:self
//                                             selector:@selector(didEnterFull:)
//                                                 name:NSWindowDidEnterFullScreenNotification
//                                               object:nil];
//
//    [[NSNotificationCenter defaultCenter] addObserver:self
//                                             selector:@selector(willExitFull:)
//                                                 name:NSWindowWillExitFullScreenNotification
//                                               object:nil];
//
//    [[NSNotificationCenter defaultCenter] addObserver:self
//                                         selector:@selector(didExitFull:)
//                                             name:NSWindowDidExitFullScreenNotification
//                                           object:nil];


    // resize
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(didResize:)
                                                 name:NSWindowDidResizeNotification
                                               object:nil];

    
    bWindowing = false;

}

//- (void)willEnterFull:(NSNotification *)notif{
//    bWindowing = true;
//    cout << endl << "willEnterFull, " <<  endl;
//}
//
//
//- (void)didEnterFull:(NSNotification *)notif{
//
//    int w = _window.frame.size.width;
//    int h = _window.frame.size.height;
//    testApp::w = w;
//    testApp::h = h;
//    cout << "didEnterFull, " << w << ", " << h <<  endl;
//    testApp::getInstance()->gps->setRenderFboResolution(w, h);
//    
//    bWindowing = false;
////    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
//}
//
//- (void)willExitFull:(NSNotification *)notif
//{
//    cout << endl <<  "willExitFull, "  <<  endl;
//    bWindowing = true;
//}
//
//- (void)didExitFull:(NSNotification *)notif
//{
//    int w = _window.frame.size.width;
//    int h = _window.frame.size.height;
//    testApp::w = w;
//    testApp::h = h;
//    testApp::getInstance()->gps->setRenderFboResolution(w, h);
//    cout << "didExitFull, " << w << ", " << h << endl;
//    bWindowing = false;
//    
////    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
//}

- (void)didResize:(NSNotification *)notif{
    int w = _window.frame.size.width;
    int h = _window.frame.size.height;
    testApp::w = w;
    testApp::h = h;
    testApp::getInstance()->gps->resize(w, h);
//    cout << "didResizeFull, " << w << ", " << h << endl;
    bWindowing = false;
}


- (void)update{
    if(!bWindowing)
        testApp::update();
    if( testApp::bAutoPlay && testApp::isNeedStartNextAnimation())
        [(PictoUIController *)[[NSApplication sharedApplication] delegate] startNextAnimation];
}

- (void)draw{
    if(!bWindowing)
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

static bool bWindowing;
+ (bool)getWindowing{
    return bWindowing;
}

@end
