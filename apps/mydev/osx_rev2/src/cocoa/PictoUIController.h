//
//  PreferenceController.h
//  dial_osx
//
//  Created by matobahiroshi on 13/07/09.
//
//
#pragma once

#import <Cocoa/Cocoa.h>

@interface PictoUIController: NSWindowController {
    NSTextField *message;
    NSButton *clearAllButton;
    NSButton *clearAll;
    NSButton *sendToScheduleButton;
    NSButton *startAnimationButton;
    NSSlider *fontSizeSlider;
    NSSlider *lineHeightSlider;
    NSSlider *letterSpasingSlider;
    NSSlider *FontSizeSlider;
    NSSlider *iconSizeSlider;
    NSSlider *iconDistanceSlider;
    NSSlider *fontRandomnessSlider;
    NSSlider *vibrationSlider;
    NSSlider *speedSlider;
    NSSlider *accelSlider;
    NSSlider *holdTimeSlider;
    NSMatrix *animateImmediateRadio;
    NSMatrix *autoDeleteRadio;
    NSSegmentedControl *InfoBarSwitch;
    NSSegmentedControl *fullscreenSwitch;
    NSSegmentedControl *blackSwitch;
    NSSegmentedControl *debugDrawSwitch;
    NSColorWell *backgroundColorPicker;
}

extern NSString * const MESSAGE;
extern NSString * const FONT_SIZE;
extern NSString * const FONT_RANDOMNESS;
extern NSString * const LINE_HEIGHT;
extern NSString * const LETTER_SPACING;

extern NSString * const ICON_SIZE;
extern NSString * const ICON_DISTANCE;

extern NSString * const SPEED;
extern NSString * const ACCEL;
extern NSString * const HOLD_TIME;
extern NSString * const ANIMATE_IMMIDIATE;
extern NSString * const AUTO_DELETE;

// GUI parts page 1
@property (assign) IBOutlet NSTextField *message;
@property (assign) IBOutlet NSButton *clearAllButton;
@property (assign) IBOutlet NSButton *startAnimationButton;
@property (assign) IBOutlet NSSlider *fontSizeSlider;
@property (assign) IBOutlet NSSlider *lineHeightSlider;
@property (assign) IBOutlet NSSlider *letterSpasingSlider;
@property (assign) IBOutlet NSSlider *iconSizeSlider;
@property (assign) IBOutlet NSSlider *iconDistanceSlider;
@property (assign) IBOutlet NSSlider *fontRandomnessSlider;
@property (assign) IBOutlet NSSlider *vibrationSlider;
@property (assign) IBOutlet NSSlider *speedSlider;
@property (assign) IBOutlet NSSlider *accelSlider;
@property (assign) IBOutlet NSSlider *holdTimeSlider;
@property (assign) IBOutlet NSMatrix *animateImmediateRadio;
@property (assign) IBOutlet NSMatrix *autoDeleteRadio;

// GUI parts page 2
@property (assign) IBOutlet NSSegmentedControl *fullscreenSwitch;
@property (assign) IBOutlet NSSegmentedControl *blackSwitch;
@property (assign) IBOutlet NSSegmentedControl *debugDrawSwitch;
@property (assign) IBOutlet NSColorWell *backgroundColorPicker;
@property (assign) IBOutlet NSSegmentedControl *InfoBarSwitch;


// action 1
- (IBAction)changeMessage:(NSTextField *)sender;
- (IBAction)pushClearAll:(NSButton *)sender;
- (IBAction)pushStartAnimation:(NSButton *)sender;
- (IBAction)pushPreviewButton:(NSButton *)sender;
- (IBAction)changeFontSize:(NSSlider *)sender;
- (IBAction)changeLineHeightSlider:(NSSlider *)sender;
- (IBAction)changeLetterSpacing:(NSSlider *)sender;
- (IBAction)changeIconSize:(NSSlider *)sender;
- (IBAction)changeIconDistance:(NSSlider *)sender;
- (IBAction)changeFontRandomness:(NSSlider *)sender;
- (IBAction)changeVibration:(NSSlider *)sender;

- (IBAction)changeSpeed:(NSSlider *)sender;
- (IBAction)changeAccel:(NSSlider *)sender;
- (IBAction)changeHoldTime:(NSSlider *)sender;
- (IBAction)changeAnimateImmediate:(NSMatrix *)sender;
- (IBAction)changeAutoDelete:(NSMatrix *)sender;


// action 2
- (IBAction)changeFullscreen:(NSSegmentedControl *)sender;
- (IBAction)changeBlack:(NSSegmentedControl *)sender;
- (IBAction)changeBackgroundColor:(NSColorWell *)sender;
- (IBAction)debugDrawSwitch:(NSSegmentedControl *)sender;
- (IBAction)changeInfoBar:(NSSegmentedControl *)sender;


+ (void) setupDefault;
+ (void) initializeParameters;
- (IBAction)closePictoUI:(id)sender;

@end




