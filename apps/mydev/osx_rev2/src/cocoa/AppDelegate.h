#pragma once

#import <Cocoa/Cocoa.h>


#import "PreferenceController.h"
#import "PictoUIController.h"

#include "ofMain.h"
    
@interface AppDelegate : NSObject <NSApplicationDelegate>
{
    NSWindow * window;
    NSWindow * pictoUIWindow;
    PreferenceController * _prefController;
    PictoUIController * _pictoUIController;
}

@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet NSWindow *pictoUIWindow;

@property (retain) PreferenceController * prefController;
@property (retain) PictoUIController * pictoUIController;

-(IBAction)showPreference:(id)sender;
-(IBAction)showPictoUI:(id)sender;

@end
