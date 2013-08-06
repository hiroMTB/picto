//
//  PreferenceController.m
//  dial_osx
//
//  Created by matobahiroshi on 13/07/09.
//
//

#import "PreferenceController.h"

#include "ofMain.h"

#include "testApp.h"

@interface PreferenceController ()

@end

@implementation PreferenceController

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    [self setPreferencePaneNotification];
    
    return self;
}


- (void)windowDidLoad
{
    [super windowDidLoad];
}


- (void)setPreferencePaneNotification{
    if (self) {

        [[NSUserDefaults standardUserDefaults] addObserver:self
                                                forKeyPath:@"commandFontSize"
                                                   options:NSKeyValueObservingOptionNew
                                                   context:NULL];

        
        
    }
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context{
    
    if([keyPath isEqual:@"commandFontSize"]){
        NSNumber *commandFontSize = [object objectForKey:@"commandFontSize"];
    }
    
}


-(IBAction)closePreference:(id)sender
{
    [self close];
}
@end