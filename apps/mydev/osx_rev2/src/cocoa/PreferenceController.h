//
//  PreferenceController.h
//  dial_osx
//
//  Created by matobahiroshi on 13/07/09.
//
//
#pragma once

#include "ofMain.h"
#import <Cocoa/Cocoa.h>

@interface PreferenceController : NSWindowController

//+ (NSDictionary*)getPreferencePaneDictionay;
- (void)setPreferencePaneNotification;
- (IBAction)closePreference:(id)sender;

@end
