//
//  PreferenceController.h
//  dial_osx
//
//  Created by matobahiroshi on 13/07/09.
//
//
#pragma once

#import <Cocoa/Cocoa.h>

@interface PictoUIController: NSWindowController{
    
    IBOutlet NSTextField *message;
    IBOutlet NSButton *clearAllButton;
    IBOutlet NSButton *startAnimationButton;
    IBOutlet NSButton *sendToPresetButton;
    IBOutlet NSSlider *fontSizeSlider;
    IBOutlet NSSlider *lineHeightSlider;
    IBOutlet NSSlider *letterSpasingSlider;
    IBOutlet NSSlider *fontRandomnessSlider;
    IBOutlet NSSlider *iconSizeSlider;
    IBOutlet NSSlider *iconDensitySlider;
    IBOutlet NSSlider *vibrationSlider;
    IBOutlet NSSlider *speedSlider;
    IBOutlet NSSlider *accelSlider;
    IBOutlet NSSegmentedControl *InfoBarSwitch;
    IBOutlet NSButton *captureButton;
    IBOutlet NSSegmentedControl *fullscreenSwitch;
    IBOutlet NSSegmentedControl *blackSwitch;
    IBOutlet NSSegmentedControl *debugDrawSwitch;
    IBOutlet NSColorWell *backgroundColorPicker;

    IBOutlet NSTableView *presetTableView;
    IBOutlet NSArrayController *myContentArray;
    IBOutlet NSButton *startPresetButton;
    IBOutlet NSButton *removePresetButton;
    IBOutlet NSButton *savePresetButton;
    IBOutlet NSButton *loadPresetButton;
    IBOutlet NSSegmentedControl *WallMapMouseAdjustSwitch;
    IBOutlet NSSegmentedControl *testPictureSwitch;
}

extern NSString * const MESSAGE;
extern NSString * const FONT_SIZE;
extern NSString * const FONT_RANDOMNESS;
extern NSString * const LINE_HEIGHT;
extern NSString * const LETTER_SPACING;

extern NSString * const ICON_SIZE;
extern NSString * const ICON_DENSITY;

extern NSString * const SPEED;
extern NSString * const ACCEL;
extern NSString * const HOLD_TIME;
extern NSString * const ANIMATE_IMMIDIATE;
extern NSString * const AUTO_DELETE;
extern NSString * const VIBRATION;



// action 1
- (IBAction)changeMessage:(NSTextField *)sender;
- (IBAction)pushClearAll:(NSButton *)sender;
- (IBAction)pushStartAnimation:(NSButton *)sender;
- (IBAction)pushPreviewButton:(NSButton *)sender;
- (IBAction)pushSendToPresetButton:(NSButton *)sender;
- (IBAction)changeFontSize:(NSSlider *)sender;
- (IBAction)changeLineHeightSlider:(NSSlider *)sender;
- (IBAction)changeLetterSpacing:(NSSlider *)sender;
- (IBAction)changeIconSize:(NSSlider *)sender;
- (IBAction)changeIconDensity:(NSSlider *)sender;
- (IBAction)changeFontRandomness:(NSSlider *)sender;
- (IBAction)changeVibration:(NSSlider *)sender;

- (IBAction)changeSpeed:(NSSlider *)sender;
- (IBAction)changeAccel:(NSSlider *)sender;


// action 2
- (IBAction)changeFullscreen:(NSSegmentedControl *)sender;
- (IBAction)changeBlack:(NSSegmentedControl *)sender;
- (IBAction)changeBackgroundColor:(NSColorWell *)sender;
- (IBAction)debugDrawSwitch:(NSSegmentedControl *)sender;
- (IBAction)changeInfoBar:(NSSegmentedControl *)sender;
- (IBAction)pushCaptureButton:(NSButton *)sender;

// action 3
- (IBAction)pushStartPresetButton:(NSButton *)sender;
- (IBAction)pushRemovePresetButton:(NSButton *)sender;
- (IBAction)pushSavePresetButton:(NSButton *)sender;
- (IBAction)pushLoadPresetButton:(NSButton *)sender;
- (IBAction)changeWallMapMouseAdjust:(NSSegmentedControl *)sender;
- (IBAction)changeTestPicture:(NSSegmentedControl *)sender;


+ (void) setupDefault;
- (void) initializeParameters;
- (IBAction)closePictoUI:(id)sender;

@end




