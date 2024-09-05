#include <Cocoa/Cocoa.h>
#include "../include/league.h"

@interface CustomTextView : NSView

@property (nonatomic, copy) NSString *displayQueueType;
@property (nonatomic, copy) NSString *displayRank;
@property (nonatomic, copy) NSString *displayWins;
@property (nonatomic, copy) NSString *displayLosses;
@property (nonatomic, assign) double x;
@property (nonatomic, assign) double y;

@end

@implementation CustomTextView

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];

    if (self.displayQueueType == nil) {
        return; 
    }

    NSDictionary *attributes = @{
        NSFontAttributeName: [NSFont systemFontOfSize:15],
        NSForegroundColorAttributeName: [NSColor whiteColor],
    };

    [self.displayQueueType drawAtPoint:NSMakePoint(self.x, self.y) withAttributes:attributes];
    [self.displayRank drawAtPoint:NSMakePoint(self.x, self.y - 20) withAttributes:attributes];
    [self.displayWins drawAtPoint:NSMakePoint(self.x, self.y - 40) withAttributes:attributes];
    [self.displayLosses drawAtPoint:NSMakePoint(self.x, self.y - 60) withAttributes:attributes];
}

@end

// CustomFloatingWindow subclass of NSWindow
@interface CustomFloatingWindow : NSWindow
@end

@implementation CustomFloatingWindow

- (instancetype)init {
    // Obtain the screen dimensions
    NSRect screenRect = [[NSScreen mainScreen] frame];

    self = [super initWithContentRect:screenRect
                            styleMask:NSWindowStyleMaskBorderless
                              backing:NSBackingStoreBuffered
                                defer:NO];
    if (self) {
        [self setLevel:NSFloatingWindowLevel];
        [self setOpaque:NO];
        [self setBackgroundColor:[NSColor clearColor]];

        [self setLevel:NSScreenSaverWindowLevel];
        [self setCollectionBehavior:NSWindowCollectionBehaviorCanJoinAllSpaces | NSWindowCollectionBehaviorStationary];

        // create button
        NSButton *myButton = [[NSButton alloc] initWithFrame:NSMakeRect(14, 980, 100, 100)]; 
        [myButton setTitle:@"Stalk 'em"];
        [myButton setBezelStyle:NSBezelStyleRounded];
        [myButton setTarget:self];
        [myButton setAction:@selector(buttonClicked:)];

        [self.contentView addSubview:myButton];

    }
    return self;
}

- (void)buttonClicked:(id)sender {
    NSLog(@"Button was clicked");

    std::pair<double, double> playerIdTopLeftCoordinates[10] = {
        {363, 975}, {660, 975}, {955, 975}, {1250, 975}, {1547, 975},
        {363, 450}, {660, 450}, {955, 450}, {1250, 450}, {1547, 450}
    };

    // for (int i = 0; i < 10; i++){
    //     for (int j = 0; j < 3; j++){
    //         CustomTextView *textView = [[CustomTextView alloc] initWithFrame:[self.contentView bounds]];
    //         textView.x = playerIdTopLeftCoordinates[i].first - 70;
    //         textView.y = (playerIdTopLeftCoordinates[i].second - 12) - 100 * j;
    //         textView.displayQueueType = @"RANKED_SOLO_5x5";
    //         textView.displayRank = @"Rank: SILVER II";
    //         textView.displayWins = @"Wins: 2";
    //         textView.displayLosses = @"Losses: 3";
    //         [self.contentView addSubview:textView];
    //     }
    // }

    std::vector<std::vector<UserDetails>> listOfUsersDetails = getPlayerInfo();

    for (int i = 0; i < listOfUsersDetails.size(); i++){
        for (int j = 0; j < listOfUsersDetails[i].size(); j++){
            CustomTextView *textView = [[CustomTextView alloc] initWithFrame:[self.contentView bounds]];
            std::string userQueueType = listOfUsersDetails[i][j].queueType;
            std::string userRank = listOfUsersDetails[i][j].wins == -1 ? "" : "Rank: " + listOfUsersDetails[i][j].tier + " " + listOfUsersDetails[i][j].rank;
            std::string userWins = listOfUsersDetails[i][j].wins == -1 ? "" : "Wins: " + std::to_string(listOfUsersDetails[i][j].wins);
            std::string userLosses = listOfUsersDetails[i][j].losses == -1 ? "" : "Losses: " + std::to_string(listOfUsersDetails[i][j].losses);

            textView.x = playerIdTopLeftCoordinates[i].first - 70;
            textView.y = (playerIdTopLeftCoordinates[i].second - 12) - 100 * j;
            textView.displayQueueType = [NSString stringWithCString:userQueueType.c_str() encoding:[NSString defaultCStringEncoding]];
            textView.displayRank = [NSString stringWithCString:userRank.c_str() encoding:[NSString defaultCStringEncoding]];
            textView.displayWins = [NSString stringWithCString:userWins.c_str() encoding:[NSString defaultCStringEncoding]];
            textView.displayLosses = [NSString stringWithCString:userLosses.c_str() encoding:[NSString defaultCStringEncoding]];
            [self.contentView addSubview:textView];
        }
    }
}

@end

int main(int argc, const char * argv[]) {

    @autoreleasepool {

        usleep(1000000);//Sleep Time 1 second.

        NSApplication *app = [NSApplication sharedApplication];
        
        CustomFloatingWindow *window = [[CustomFloatingWindow alloc] init];
        [window makeKeyAndOrderFront:nil];
        
        [app run];
    }

    return 0;
}
