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
#include "gpuPictoString.h"

@interface PictoUIController ()

@end

@implementation PictoUIController

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


-(void)awakeFromNib{
    
    [self initializeParameters];
    [self initPresetView];
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
    
    
    
    testApp::setDebugDraw((bool)debugDraw);
    testApp::setShowInfo((bool)showInfo);

    gpuPictoString::prm.message = (std::string([mes UTF8String]));
    gpuPictoString::prm.fontSize = fontSize;
    gpuPictoString::prm.iconSize = iconSize;
    gpuPictoString::prm.iconDensity = iconDensity;
    gpuPictoString::prm.fontRandomness = fontRandomenss;
    gpuPictoString::prm.letterSpacing = letterSpacing;
    gpuPictoString::prm.lineHeight = lineHeight;
    gpuPictoString::prm.speed = speed;
    gpuPictoString::prm.accel = accel;
    
    NSColor * bg=nil;
    NSData *theData=[[NSUserDefaults standardUserDefaults] dataForKey:@"BackgroundColor"];
    if (theData != nil){
        bg =(NSColor *)[NSUnarchiver unarchiveObjectWithData:theData];
        bg = [bg colorUsingColorSpaceName:NSCalibratedRGBColorSpace];
        [backgroundColorPicker setColor:bg];
        testApp::setBackgroundColor([bg redComponent]*255.0, [bg greenComponent]*255.0, [bg blueComponent]*255.0);
    }
}


-(void)initPresetView{
    NSTableColumn *column;
    column = [presetTableView tableColumnWithIdentifier:@"pMessage"];
	[column bind:@"value" toObject:myContentArray withKeyPath:@"arrangedObjects.pMessage" options:nil];
    
    column = [presetTableView tableColumnWithIdentifier:@"pFontSize"];
	[column bind:@"value" toObject:myContentArray withKeyPath:@"arrangedObjects.pFontSize" options:nil];

    column = [presetTableView tableColumnWithIdentifier:@"pLineHeight"];
	[column bind:@"value" toObject:myContentArray withKeyPath:@"arrangedObjects.pLineHeight" options:nil];

    column = [presetTableView tableColumnWithIdentifier:@"pLetterSpacing"];
	[column bind:@"value" toObject:myContentArray withKeyPath:@"arrangedObjects.pLetterSpacing" options:nil];

    column = [presetTableView tableColumnWithIdentifier:@"pRandomness"];
	[column bind:@"value" toObject:myContentArray withKeyPath:@"arrangedObjects.pRandomness" options:nil];

    column = [presetTableView tableColumnWithIdentifier:@"pIconSize"];
	[column bind:@"value" toObject:myContentArray withKeyPath:@"arrangedObjects.pIconSize" options:nil];

    column = [presetTableView tableColumnWithIdentifier:@"pIconDensity"];
	[column bind:@"value" toObject:myContentArray withKeyPath:@"arrangedObjects.pIconDensity" options:nil];

    column = [presetTableView tableColumnWithIdentifier:@"pSpeed"];
	[column bind:@"value" toObject:myContentArray withKeyPath:@"arrangedObjects.pSpeed" options:nil];

    column = [presetTableView tableColumnWithIdentifier:@"pAccel"];
	[column bind:@"value" toObject:myContentArray withKeyPath:@"arrangedObjects.pAccel" options:nil];

    column = [presetTableView tableColumnWithIdentifier:@"pVibration"];
	[column bind:@"value" toObject:myContentArray withKeyPath:@"arrangedObjects.pVibration" options:nil];

    
    PrmData prm1("BUILDING\nWORKER'S\nPOWER", 0.12, 1.15, 1, 0, 0.03, 0.05, 10, 14, 0.0);
    [self addPresetFromPrmData:prm1];
}


-(void)addPresetFromPrmData:(PrmData) prm
{
    NSMutableDictionary * data = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                 [NSString stringWithUTF8String:prm.message.c_str()], @"pMessage",
                                 [NSNumber numberWithFloat:prm.fontSize], @"pFontSize",
                                 [NSNumber numberWithFloat:prm.lineHeight], @"pLineHeight",
                                 [NSNumber numberWithFloat:prm.letterSpacing], @"pLetterSpacing",
                                 [NSNumber numberWithFloat:prm.fontRandomness], @"pRandomness",
                                 [NSNumber numberWithFloat:prm.iconSize], @"pIconSize",
                                 [NSNumber numberWithFloat:prm.iconDensity], @"pIconDensity",
                                 [NSNumber numberWithFloat:prm.speed], @"pSpeed",
                                 [NSNumber numberWithFloat:prm.accel], @"pAccel",
                                 [NSNumber numberWithFloat:prm.vibration], @"pVibration",
                                 nil];
	
    
    [myContentArray addObject: data];
}

-(IBAction)closePictoUI:(id)sender
{
//    [[NSUserDefaults standardUserDefaults] synchronize];
    [self close];
}

- (IBAction)changeMessage:(NSTextField *)sender {
    NSString * m = sender.stringValue;
    gpuPictoString::prm.message = (std::string([m UTF8String]));
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

- (IBAction)pushClearAll:(NSButton *)sender {
    testApp::getInstance()->clearAll();
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

- (IBAction)pushStartAnimation:(NSButton *)sender {
    NSString * m = message.stringValue;
    gpuPictoString::prm.message = (std::string([m UTF8String]));
    testApp::getInstance()->makeAnimation();
}

- (IBAction)pushPreviewButton:(NSButton *)sender {
    NSString * m = message.stringValue;
    gpuPictoString::prm.message = (std::string([m UTF8String]));
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

- (IBAction)pushSendToPresetButton:(NSButton *)sender {
    
    // set preset pannel
    PrmData prm;
    prm.message = string([[message stringValue] UTF8String]);
    prm.fontSize = [fontSizeSlider floatValue];
    prm.lineHeight = [lineHeightSlider floatValue];
    prm.letterSpacing = [letterSpasingSlider floatValue];
    prm.fontRandomness = [fontRandomnessSlider floatValue];
    prm.iconSize = [iconSizeSlider floatValue];
    prm.iconDensity = [iconDensitySlider floatValue];
    prm.speed = [speedSlider floatValue];
    prm.accel = [accelSlider floatValue];
    prm.vibration = [vibrationSlider floatValue];
    
    [self addPresetFromPrmData:prm];
}

- (IBAction)changeFontSize:(NSSlider *)sender {
    gpuPictoString::prm.fontSize = sender.floatValue;
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

- (IBAction)changeLineHeightSlider:(NSSlider *)sender {
    gpuPictoString::prm.lineHeight = sender.floatValue;
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

- (IBAction)changeLetterSpacing:(NSSlider *)sender{
    gpuPictoString::prm.letterSpacing = sender.floatValue;
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

- (IBAction)changeIconSize:(NSSlider *)sender {
    gpuPictoString::prm.iconSize = sender.floatValue;
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

- (IBAction)changeIconDensity:(NSSlider *)sender {
    gpuPictoString::prm.iconDensity = sender.floatValue;
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

- (IBAction)changeFontRandomness:(NSSlider *)sender {
    gpuPictoString::prm.fontRandomness = sender.floatValue;
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
}

- (IBAction)changeVibration:(NSSlider *)sender {
    gpuPictoString::prm.vibration = sender.floatValue;
}

- (IBAction)changeSpeed:(NSSlider *)sender {
    gpuPictoString::prm.speed = sender.floatValue;
}

- (IBAction)changeAccel:(NSSlider *)sender {
    gpuPictoString::prm.accel = sender.floatValue;
}



- (IBAction)debugDrawSwitch:(NSSegmentedControl *)sender {
    testApp::setDebugDraw(sender.selectedSegment);
}

- (IBAction)changeInfoBar:(NSSegmentedControl *)sender {
    testApp::setShowInfo(sender.selectedSegment);
}

- (IBAction)pushCaptureButton:(NSButton *)sender {

    NSSavePanel* capPanel = [NSSavePanel savePanel];
    
    [capPanel setMessage:@"Rendering PDF file"];
  	NSArray* fileTypes = [NSArray arrayWithObjects:@"pdf", nil];
    [capPanel setAllowedFileTypes:fileTypes];
    [capPanel beginSheetModalForWindow:self.window completionHandler:^(NSInteger result) {
        if(result == NSOKButton){
            string path = std::string([[[capPanel URL] path] UTF8String]);
            testApp::bCap = true;
            testApp::pdfCapturePath = path;
        }
    }];
}

-(PrmData)getRecordAtRow: (int)row
{
    PrmData prm;
    NSMutableDictionary *record = [myContentArray.arrangedObjects objectAtIndex:row];
    
    prm.message         = string([[record valueForKey:@"pMessage"] UTF8String]);
    prm.fontSize        = [[record valueForKey:@"pFontSize"] floatValue];
    prm.lineHeight      = [[record valueForKey:@"pLineHeight"] floatValue];
    prm.letterSpacing   = [[record valueForKey:@"pLetterSpacing"] floatValue];
    prm.fontRandomness  = [[record valueForKey:@"pRandomness"] floatValue];
    prm.iconSize        = [[record valueForKey:@"pIconSize"] floatValue];
    prm.iconDensity     = [[record valueForKey:@"pIconDensity"] floatValue];
    prm.speed           = [[record valueForKey:@"pSpeed"] floatValue];
    prm.accel           = [[record valueForKey:@"pAccel"] floatValue];
    prm.vibration       = [[record valueForKey:@"pVibration"] floatValue];

    //NSLog(@"%@", record);
    //printf("%s, %f, %f, %f, %f, %f, %f, %f, %f, %f", prm.message.c_str(), prm.fontSize, prm.lineHeight, prm.letterSpacing, prm.fontRandomness, prm.iconSize, prm.iconDensity, prm.speed, prm.accel, prm.vibration);
    return prm;
}


- (IBAction)pushStartPresetButton:(NSButton *)sender {
    NSInteger row = presetTableView.selectedRow;

    PrmData prm = [self getRecordAtRow:row];
    
    // set
    gpuPictoString::prm = prm;
    //testApp::getInstance()->setPreviewText(std::string([prm.message UTF8String]));
    [message setStringValue:[NSString stringWithUTF8String:prm.message.c_str()]];
    [fontSizeSlider setFloatValue:prm.fontSize];
    [lineHeightSlider setFloatValue:prm.lineHeight];
    [letterSpasingSlider setFloatValue:prm.letterSpacing];
    [fontRandomnessSlider setFloatValue:prm.fontRandomness];
    [iconSizeSlider setFloatValue:prm.iconSize];
    [iconDensitySlider setFloatValue:prm.iconDensity];
    [speedSlider setFloatValue:prm.speed];
    [accelSlider setFloatValue:prm.accel];
    [vibrationSlider setFloatValue:prm.vibration];
    
    testApp::getInstance()->gps->bNeedUpdateCharPos = true;
    testApp::getInstance()->makeAnimation();
}

- (IBAction)pushRemovePresetButton:(NSButton *)sender {
    NSInteger row = presetTableView.selectedRow;
    if(row>=0){
        NSIndexSet * indexes = [NSIndexSet indexSetWithIndex:row];
    
        [presetTableView beginUpdates];
        [presetTableView removeRowsAtIndexes:indexes withAnimation:nil];
        [presetTableView endUpdates];

        // remove from model
        [myContentArray removeObjectAtArrangedObjectIndex:row];
        
        // move select up
        NSIndexSet * newSelect = [NSIndexSet indexSetWithIndex:row-1];
        [presetTableView selectRowIndexes:newSelect byExtendingSelection:NO];
    }
}


- (IBAction)pushSavePresetButton:(NSButton *)sender {

    NSSavePanel* savePanel = [NSSavePanel savePanel];
    
    [savePanel setMessage:@"Save XML file"];
  	NSArray* fileTypes = [NSArray arrayWithObjects:@"xml", nil];
    [savePanel setAllowedFileTypes:fileTypes];
    [savePanel beginSheetModalForWindow:self.window completionHandler:^(NSInteger result) {
        if(result == NSOKButton){
            string path = std::string([[[savePanel URL] path] UTF8String]);
            
            int rowNum = [presetTableView numberOfRows];
            vector<PrmData> prms;
            prms.reserve(rowNum);
            // iterate all presetTableView
            for(int row=0; row<rowNum; row++) {
                prms.push_back([self getRecordAtRow:row]);
            }
            
            //save to XML
            gpuPictoString::savePresets(path, prms);
        }
    }];
}

- (IBAction)pushLoadPresetButton:(NSButton *)sender {
    NSOpenPanel* openPanel = [NSOpenPanel openPanel];
    
	NSArray* fileTypes = [NSArray arrayWithObjects:@"xml", nil];
	[openPanel setAllowsMultipleSelection:NO];
    [openPanel setAllowedFileTypes:fileTypes];
	[openPanel setMessage:@"Open XML file"];
    [openPanel beginSheetModalForWindow:self.window completionHandler:^(NSInteger result) {
        if (result == NSOKButton){
            if ([[openPanel URL] isFileURL]){
                [self clearAllPreset];
                string path = std::string([[[openPanel URL] path] UTF8String]);
                vector<PrmData> ps = gpuPictoString::loadPresets(path);
                
                for(int i=0; i<ps.size(); i++){
                    [self addPresetFromPrmData:ps[i]];
                }
            }
        }
    }];
}

- (IBAction)changeFullscreen:(NSSegmentedControl *)sender {
    testApp::setFullscreen(sender.selectedSegment);
}

- (IBAction)changeBlack:(NSSegmentedControl *)sender {
    testApp::setBlack(sender.selectedSegment);
}

- (IBAction)changeBackgroundColor:(NSColorWell *)sender {
    NSColor * bg = [sender.color colorUsingColorSpaceName:NSCalibratedRGBColorSpace];
    float r = bg.redComponent;
    float g = bg.greenComponent;
    float b = bg.blueComponent;
    testApp::getInstance()->setBackgroundColor(r*255.0, g*255.0, b*255.0);
}


- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender{
    // store the value in aColor in user defaults
    // as the value for key aKey
    NSColor * bg = [backgroundColorPicker.color colorUsingColorSpaceName:NSCalibratedRGBColorSpace];
    NSData *theData=[NSArchiver archivedDataWithRootObject:bg];
    [[NSUserDefaults standardUserDefaults] setObject:theData forKey:@"BackgroundColor"];
//    [super applicationSohuldTerminate:self];
    return true;
}

-(void) clearAllPreset
{
    [[myContentArray mutableArrayValueForKey:@"content"] removeAllObjects];
}


@end
