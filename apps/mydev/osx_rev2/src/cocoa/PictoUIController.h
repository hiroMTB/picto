//
//  PreferenceController.h
//  dial_osx
//
//  Created by matobahiroshi on 13/07/09.
//
//
#pragma once

#import <Cocoa/Cocoa.h>

@interface PictoUIController: NSWindowController


extern NSString * const MESSAGE;
extern NSString * const FONT_SIZE;
extern NSString * const ICON_SIZE;
extern NSString * const SPEED;
extern NSString * const ACCEL;
extern NSString * const HOLD_TIME;
extern NSString * const ANIMATE_IMMIDIATE;
extern NSString * const AUTO_DELETE;


//+ (NSDictionary*)getPreferencePaneDictionay;


+ (void) setupDefault;
+ (void) initializeParameters;
- (void)setPictoUIControllerNotification;
- (IBAction)closePictoUI:(id)sender;

@end
