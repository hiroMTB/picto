#import "AppDelegate.h"

@implementation AppDelegate

@synthesize window;
@synthesize prefController = _prefController;
@synthesize pictoUIController = _pictoUIController;


- (void)applicationDidFinishLaunching:(NSNotification *)anotification
{
//    NSDictionary *prefDefault = [PreferenceController getPreferencePaneDictionay];
//    [[NSUserDefaults standardUserDefaults] registerDefaults:prefDefault];
//    self.prefController = [[PreferenceController alloc] initWithWindowNibName:@"Preference"];
}

-(IBAction)showPreference:(id)sender
{
    if(!self.prefController)
        self.prefController = [[PreferenceController alloc] initWithWindowNibName:@"Preference"];
    
    [self.prefController showWindow:self];
    
}

-(IBAction)showPictoUI:(id)sender
{
    if(!self.pictoUIController)
        self.pictoUIController = [[PictoUIController alloc] initWithWindowNibName:@"PictoUI"];
    
    [self.pictoUIController showWindow:self];
    
}



- (void)dealloc
{
    [super dealloc];
}


@end
