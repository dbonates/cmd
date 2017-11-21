//
//  main.m
//  cmd
//
//  Created by Daniel Bonates on 21/11/17.
//  Copyright © 2017 Daniel Bonates. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CmdManager.h"
#import "Logger.h"

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        
        
        CmdManager *manager = [[CmdManager alloc] init];
        [manager createDbFileIfNeeded];
        
        NSArray *arguments = [[NSProcessInfo processInfo] arguments];
        
        if (arguments.count < 2) {
            [Logger printIntro];
            return 1;
        }
        
        
        NSString *commandArgument = [arguments objectAtIndex: 1];
        
        
        NSString *requestedAlias = arguments.count < 3 ? Nil : [arguments objectAtIndex: 2];
        
        
        if ([commandArgument isEqualToString:@"-l"]) {
            
            NSMutableDictionary *allDirs = [manager allCmds];
            [Logger printReport:allDirs];
            return 0;
            
        }
        else if ([commandArgument isEqualToString:@"-s"]) {
            NSString *requestedCommand = arguments.count < 4 ? Nil : [arguments objectAtIndex: 3];
            return [manager saveCommandWithAlias: requestedAlias for: requestedCommand shouldOverride: NO];
        }
        else if ([commandArgument isEqualToString:@"-so"]) {
            NSString *requestedCommand = arguments.count < 4 ? Nil : [arguments objectAtIndex: 3];
            return [manager saveCommandWithAlias: requestedAlias for: requestedCommand shouldOverride: YES];
        }
        else if([commandArgument isEqualToString:@"-d"]) {
            return [manager deleteCmdWithAlias: requestedAlias];
        }
        else if ([commandArgument isEqualToString:@"-u"]) {
            return [manager updateShell];
        }
        
        
        
    }
    return 0;
}
