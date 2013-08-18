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

extern NSString * const MESSAGE             = @"Message";
extern NSString * const FONT_SIZE           = @"FontSize";
extern NSString * const ICON_SIZE           = @"IconSize";
extern NSString * const SPEED               = @"Speed";
extern NSString * const ACCEL               = @"Accel";
extern NSString * const HOLD_TIME           = @"HoldTime";
extern NSString * const ANIMATE_IMMIDIATE   = @"AnimateImmediate";
extern NSString * const AUTO_DELETE         = @"AutoDelete";

static NSString * paramList[] = { MESSAGE, FONT_SIZE, ICON_SIZE, SPEED, ACCEL, HOLD_TIME, ANIMATE_IMMIDIATE, AUTO_DELETE };

+ (void) setupDefault
{
    NSString *userDefaultsValuesPath;
    NSDictionary *userDefaultsValuesDict;
    NSDictionary *initialValuesDict;
    NSArray *resettableUserDefaultsKeys;
    
    // load the default values for the user defaults
    userDefaultsValuesPath=[[NSBundle mainBundle] pathForResource:@"UserDefaults"
														   ofType:@"plist"];
    userDefaultsValuesDict=[NSDictionary dictionaryWithContentsOfFile:userDefaultsValuesPath];
    
    for(id key in userDefaultsValuesDict){
        id object = [userDefaultsValuesDict objectForKey:key];
        float f = [object floatValue];
        NSLog(@"%@, %f", key, f);
    }
    
    // set them in the standard user defaults
    [[NSUserDefaults standardUserDefaults] registerDefaults:userDefaultsValuesDict];
    
    
    [PictoUIController initializeParameters];
}

+ (void) initializeParameters
{
    NSDictionary * defaults = [[NSUserDefaults standardUserDefaults] dictionaryRepresentation];
    
    NSString * message          = [[defaults objectForKey:MESSAGE] stringValue];
    float fontSize              = [[defaults objectForKey:FONT_SIZE] floatValue];
    float iconSize              = [[defaults objectForKey:ICON_SIZE] floatValue];
    float speed              = [[defaults objectForKey:SPEED] floatValue];
    float accel              = [[defaults objectForKey:ACCEL] floatValue];
    int holdTime              = [[defaults objectForKey:HOLD_TIME] intValue];
    
    
    picto::setSPEED(speed);
    picto::setACCEL(accel);
    
    pictoChar::setFONT_SIZE(fontSize);
    pictoChar::setICON_SIZE(iconSize);
    
    
}

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

        for(int i=0; i<8; i++){
            NSString * key = paramList[i];
            
            [[NSUserDefaults standardUserDefaults] addObserver:self
                                                forKeyPath:key
                                                   options:NSKeyValueObservingOptionNew
                                                   context:NULL];
      
            NSLog(@"%@", key);
        }
    }
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context{
    
    if([keyPath isEqual:MESSAGE]){
    }else if([keyPath isEqual:FONT_SIZE]){
        float f = [[object objectForKey:FONT_SIZE] floatValue];
        pictoChar::setFONT_SIZE(f);
    }else if([keyPath isEqual:ICON_SIZE]){
        float f = [[object objectForKey:ICON_SIZE] floatValue];
        pictoChar::setICON_SIZE(f);
    }else if([keyPath isEqual:SPEED]){
        float f = [[object objectForKey:SPEED] floatValue];
        picto::setSPEED(f);
    }else if([keyPath isEqual:ACCEL]){
        float f = [[object objectForKey:ACCEL] floatValue];
        picto::setACCEL(f);
    }else if([keyPath isEqual:HOLD_TIME]){
        int i = [[object objectForKey:HOLD_TIME] intValue];
        picto::setHoldTime(i);
    }else{
        NSLog(keyPath);
    }


}


-(IBAction)closePictoUI:(id)sender
{
    [[NSUserDefaults standardUserDefaults] synchronize];
    
    [self close];
}
@end
