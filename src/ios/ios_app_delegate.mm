#import <ConnectIQ/ConnectIQ.h>
#import "UIKit/UIKit.h"
#import "UserNotifications/UserNotifications.h"

@interface QIOSApplicationDelegate <IQAppMessageDelegate>
@end

@interface QIOSApplicationDelegate (QZApplicationDelegate) <IQAppMessageDelegate>
@end

@implementation QIOSApplicationDelegate (QZApplicationDelegate)

- (BOOL)application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    NSLog(@"launch!");
    UNUserNotificationCenter* center = [UNUserNotificationCenter currentNotificationCenter];
    [center requestAuthorizationWithOptions:UNAuthorizationOptionBadge
      completionHandler:^(BOOL granted, NSError *error){
          if(granted == YES) {
              [[UIApplication sharedApplication] setMinimumBackgroundFetchInterval:UIApplicationBackgroundFetchIntervalMinimum];
          };
      }];

    [[ConnectIQ sharedInstance] initializeWithUrlScheme:@"org.cagnulein.ConnectIQComms-ciq"
                                 uiOverrideDelegate:self];

    return YES;
}

- (BOOL)application:(UIApplication *)application
            openURL:(NSURL *)url
  sourceApplication:(NSString *)sourceApplication
         annotation:(id)annotation {
    if ([url.scheme isEqualToString:ReturnURLScheme] &&
        [sourceApplication isEqualToString:IQGCMBundle]) {

        NSArray *devices = [[ConnectIQ sharedInstance]
                             parseDeviceSelectionResponseFromURL:url];
        if (devices != nil) {
            [self.devices removeAllObjects];
            for (IQDevice *device in devices) {
                self.devices[device.uuid] = device;
                [[ConnectIQ sharedInstance] registerForDeviceEvents:device
                                           delegate:self];                
                NSUUID *uuid = [[NSUUID alloc] initWithUUIDString:@"feec8674-2795-4e03-a283-0b69a0a291e3"];
                IQApp *app = [IQApp appWithUUID:uuid device:device];

                [[ConnectIQ sharedInstance] getAppStatus:app
                                            completion:^(IQAppStatus *appStatus) {
                    if (appStatus != nil && appStatus.isInstalled) {
                        NSLog(@”App is installed! Version: %d”, appStatus.version);
                    }
                }];  

                [[ConnectIQ sharedInstance] openAppRequest:app
                                                completion:^(IQSendMessageResult result) {
                    switch(result) {
                        case IQSendMessageResult_Success: NSLog(@”Popup was displayed”); break;
                        case IQSendMessageResult_Failure_PromptNotDisplayed: NSLog(@”Popup was
                                displayed”); break;
                        case IQSendMessageResult_Failure_AppAlreadyRunning: NSLog(@”Popup was
                                displayed”); break;
                    }
                }];         

                NSArray *message = @[@”hello pi”, @(3.14159)];
                [[ConnectIQ sharedInstance] sendMessage:message
                                                toApp:app
                                            progress:^(uint32_t sent, uint32_t total) {
                    float percent = 100 * sent / (float)total;
                    NSLog(@"%02.2f%% - %u/%u", percent, sent, total);
                } completion:^(IQSendMessageResult result) {
                    NSLog(@"Send message finished with result %@",
                        NSStringFromSendMessageResult(result));
                }];                                     
            }
            return YES;
        }
    }
    return NO;
}

- (void)viewWillAppear:(BOOL)animated {
    [[ConnectIQ sharedInstance] registerForAppMessages:self.app delegate:self];
}

- (void)viewDidDisappear:(BOOL)animated {
    [[ConnectIQ sharedInstance] unregisterForAllAppMessages:self];
}

- (void)receivedMessage:(id)message fromApp:(IQApp *)app {
    NSLog(@"Received message from app %@: '%@'", app, message);
}

- (void)application:(UIApplication *)application
performFetchWithCompletionHandler:(void (^)(UIBackgroundFetchResult result))completionHandler
{
}
@end