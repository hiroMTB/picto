//
//  PreferenceController.m
//  dial_osx
//
//  Created by matobahiroshi on 13/07/09.
//
//

#import "PictoUIController.h"

#include "ofMain.h"

#include "testApp.h"

@interface PictoUIController ()

@end

@implementation PictoUIController

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    [self setPictoUIControllerNotification];
    
    return self;
}


- (void)windowDidLoad
{
    [super windowDidLoad];
}


- (void)setPictoUIControllerNotification{
    if (self) {

        [[NSUserDefaults standardUserDefaults] addObserver:self
                                                forKeyPath:@"?????"
                                                   options:NSKeyValueObservingOptionNew
                                                   context:NULL];

        
        
    }
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context{
    
    if([keyPath isEqual:@"???"]){
        NSNumber *commandFontSize = [object objectForKey:@"???"];
    }
    
}


-(IBAction)closePreference:(id)sender
{
    [self close];
}
@end
