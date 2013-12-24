#import "AppDelegate.h"

@implementation AppDelegate

@synthesize window;
@synthesize pictoUIWindow;
@synthesize prefController = _prefController;
@synthesize pictoUIController = _pictoUIController;

//- (void)applicationWillFinishLaunching:(NSNotification *) aNotification
//{
//    [PictoUIController setupDefault];
//
//}

- (void)applicationDidFinishLaunching:(NSNotification *)anotification
{
//    cout << "appDelegate::applicationDidFinishLaunching" << endl;
    
//    NSDictionary *prefDefault = [PreferenceController getPreferencePaneDictionay];
//    [[NSUserDefaults standardUserDefaults] registerDefaults:prefDefault];
//    self.prefController = [[PreferenceController alloc] initWithWindowNibName:@"Preference"];
//    [PictoUIController setupDefault];
    
    [self showPictoUI:self];
    
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

- (void)windowWillEnterFullScreen:(NSNotification *)notification{
//    cout << "AppDelegate" << endl;
}

@end
