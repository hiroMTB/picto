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
#include "pictoChar.h"
#include "gpuPictoString.h"

@interface PictoUIController ()

@end

@implementation PictoUIController

@synthesize message;
@synthesize clearAllButton;
@synthesize startAnimationButton;
@synthesize fontSizeSlider;
@synthesize lineHeightSlider;
@synthesize letterSpasingSlider;
@synthesize iconSizeSlider;
@synthesize iconDensitySlider;
@synthesize fontRandomnessSlider;
@synthesize vibrationSlider;
@synthesize speedSlider;
@synthesize accelSlider;
@synthesize holdTimeSlider;
@synthesize animateImmediateRadio;
@synthesize autoDeleteRadio;
@synthesize InfoBarSwitch;
@synthesize captureButton;
@synthesize fullscreenSwitch;
@synthesize blackSwitch;
@synthesize debugDrawSwitch;
@synthesize backgroundColorPicker;

extern NSString * const MESSAGE             = @"Message";
extern NSString * const FONT_SIZE           = @"FontSize";
extern NSString * const ICON_SIZE           = @"IconSize";
extern NSString * const ICON_DENSITY        = @"IconDensity";
extern NSString * const FONT_RANDOMNESS     = @"FontRandomness";
extern NSString * const LINE_HEIGHT         = @"LineHeight";
extern NSString * const LETTER_SPACING      = @"LetterSpacing";

extern NSString * const SPEED               = @"Speed";
extern NSString * const ACCEL               = @"Accel";
extern NSString * const HOLD_TIME           = @"HoldTime";
extern NSString * const ANIMATE_IMMIDIATE   = @"AnimateImmediate";
extern NSString * const AUTO_DELETE         = @"AutoDelete";
extern NSString * const VIBRATION         = @"Vibration";

static NSString * paramList[] = { MESSAGE, FONT_SIZE, ICON_SIZE, ICON_DENSITY, FONT_RANDOMNESS, SPEED, ACCEL, HOLD_TIME, ANIMATE_IMMIDIATE, AUTO_DELETE, VIBRATION};


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
    
    // set them in the standard user defaults
    [[NSUserDefaults standardUserDefaults] registerDefaults:userDefaultsValuesDict];
    
 //   [PictoUIController initializeParameters];
}

- (void) initializeParameters
{
    NSDictionary * defaults = [[NSUserDefaults standardUserDefaults] dictionaryRepresentation];
//    for(NSString * key in defaults){
//        id v = [defaults objectForKey:key];
//        cout << string([key UTF8String]) << "   ";
//        if([v isKindOfClass:[NSString class]]){
//            cout << [defaults stringForKey:key] << endl;
//        }else if([v isKindOfClass:[NSNumber class]]){
//            cout << [v floatValue] << endl;
//        }
//    }
    
    NSString * mes          = [defaults stringForKey:MESSAGE];
    float fontSize          = [[defaults objectForKey:FONT_SIZE] floatValue];
    float fontRandomenss    = [[defaults objectForKey:FONT_RANDOMNESS] floatValue];
    float letterSpacing     = [[defaults objectForKey:LETTER_SPACING] floatValue];
    float lineHeight        = [[defaults objectForKey:LINE_HEIGHT] floatValue];
    
    float iconSize          = [[defaults objectForKey:ICON_SIZE] floatValue];
    float iconDensity      = [[defaults objectForKey: ICON_DENSITY] floatValue];
    
    float speed             = [[defaults objectForKey:SPEED] floatValue];
    float accel             = [[defaults objectForKey:ACCEL] floatValue];
    int holdTime            = [[defaults objectForKey:HOLD_TIME] intValue];
    
    float vibration         = [[defaults objectForKey:VIBRATION] floatValue];
    
    int showInfo           = [[defaults objectForKey:@"ShowInfo"] intValue];
    int debugDraw           = [[defaults objectForKey:@"DebugDraw"] intValue];
    
    
    testApp::getInstance()->setPreviewText(std::string([mes UTF8String]));
    
    testApp::setDebugDraw((bool)debugDraw);
    testApp::setShowInfo((bool)showInfo);
    
    picto::setSPEED(speed);
    picto::setACCEL(accel);
    
    pictoChar::setFONT_SIZE(fontSize);
    pictoChar::setICON_SIZE(iconSize);
    pictoChar::setICON_DISTANCE(iconDensity);
    pictoChar::setFONT_RANDOMNESS(fontRandomenss);
    pictoChar::setLETTER_SPACING(letterSpacing);
    pictoChar::setLINE_HEIGHT(lineHeight);
    
    gpuPictoString::FONT_SIZE = fontSize;
    gpuPictoString::ICON_SIZE = iconSize;
    gpuPictoString::ICON_DENSITY = iconDensity;
    gpuPictoString::FONT_RANDOMNESS = fontRandomenss;
    gpuPictoString::LETTER_SPACING = letterSpacing;
    gpuPictoString::LINE_HEIGHT = lineHeight;
    gpuPictoString::SPEED = speed;
    gpuPictoString::ACCEL = accel;
    
    NSColor * bg=nil;
    NSData *theData=[[NSUserDefaults standardUserDefaults] dataForKey:@"BackgroundColor"];
    if (theData != nil){
        bg =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
        [backgroundColorPicker setColor:bg];
        testApp::setBackgroundColor(bg.redComponent*255.0, bg.greenComponent*255.0, bg.blueComponent*255.0);
        
    }
    
}

-(void)awakeFromNib{
    [self initializeParameters];
}

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    return self;
}


- (void)windowDidLoad
{
    [super windowDidLoad];
}


-(IBAction)closePictoUI:(id)sender
{
//    [[NSUserDefaults standardUserDefaults] synchronize];
    [self close];
}

- (IBAction)changeMessage:(NSTextField *)sender {
    NSString * m = sender.stringValue;
//    testApp::getInstance()->makeAnimation(std::string([m UTF8String]));
    testApp::getInstance()->setPreviewText(std::string([m UTF8String]));
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

- (IBAction)pushClearAll:(NSButton *)sender {
    testApp::getInstance()->clearAll();
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

- (IBAction)pushStartAnimation:(NSButton *)sender {
    NSString * m = message.stringValue;
    testApp::getInstance()->setPreviewText(std::string([m UTF8String]));
    testApp::getInstance()->makeAnimation();
}

- (IBAction)pushPreviewButton:(NSButton *)sender {
    NSString * m = message.stringValue;
    testApp::getInstance()->setPreviewText(std::string([m UTF8String]));
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

- (IBAction)changeFontSize:(NSSlider *)sender {
    gpuPictoString::FONT_SIZE = sender.floatValue;
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

- (IBAction)changeLineHeightSlider:(NSSlider *)sender {
    gpuPictoString::LINE_HEIGHT = sender.floatValue;
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

- (IBAction)changeLetterSpacing:(NSSlider *)sender{
    gpuPictoString::LETTER_SPACING = sender.floatValue;
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

- (IBAction)changeIconSize:(NSSlider *)sender {
    gpuPictoString::ICON_SIZE = sender.floatValue;
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

- (IBAction)changeIconDensity:(NSSlider *)sender {
    gpuPictoString::ICON_DENSITY = sender.floatValue;
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

- (IBAction)changeFontRandomness:(NSSlider *)sender {
    gpuPictoString::FONT_RANDOMNESS = sender.floatValue;
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

- (IBAction)changeVibration:(NSSlider *)sender {
    gpuPictoString::VIBRATION = sender.floatValue;
}

- (IBAction)changeSpeed:(NSSlider *)sender {
    picto::setSPEED(sender.floatValue);
    gpuPictoString::SPEED = sender.floatValue;
}

- (IBAction)changeAccel:(NSSlider *)sender {
    picto::setACCEL(sender.floatValue);
    gpuPictoString::ACCEL = sender.floatValue;
}

- (IBAction)changeHoldTime:(NSSlider *)sender {
    picto::setHoldTime(sender.intValue);
}

- (IBAction)changeAnimateImmediate:(NSMatrix *)sender {
    
}

- (IBAction)changeAutoDelete:(NSMatrix *)sender {
}


- (IBAction)debugDrawSwitch:(NSSegmentedControl *)sender {
    testApp::setDebugDraw(sender.selectedSegment);
}

- (IBAction)changeInfoBar:(NSSegmentedControl *)sender {
    testApp::setShowInfo(sender.selectedSegment);
}

- (IBAction)pushCaptureButton:(NSButton *)sender {
    testApp::bCap = true;
}


- (IBAction)changeFullscreen:(NSSegmentedControl *)sender {
    testApp::setFullscreen(sender.selectedSegment);
}

- (IBAction)changeBlack:(NSSegmentedControl *)sender {
    testApp::setBlack(sender.selectedSegment);
}

- (IBAction)changeBackgroundColor:(NSColorWell *)sender {
    float r = sender.color.redComponent;
    float g = sender.color.greenComponent;
    float b = sender.color.blueComponent;
    testApp::getInstance()->setBackgroundColor(r*255.0, g*255.0, b*255.0);
}


- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender{
    // store the value in aColor in user defaults
    // as the value for key aKey
    NSData *theData=[NSArchiver archivedDataWithRootObject:backgroundColorPicker.color];
    [[NSUserDefaults standardUserDefaults] setObject:theData forKey:@"BackgroundColor"];
//    [super applicationSohuldTerminate:self];
    return true;
}

@end
