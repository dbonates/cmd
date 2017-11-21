//
//  CmdManager.h
//  cmd
//
//  Created by Daniel Bonates on 10/11/17.
//  Copyright © 2017 Daniel Bonates. All rights reserved.
//

#ifndef CmdManager_h
#define CmdManager_h


#endif /* CmdManager_h */

#import "Logger.h"

@interface CmdManager: NSObject

- (void)createDbFileIfNeeded;

@property (nonatomic, retain) NSString *dbFile;
@property (nonatomic, retain) NSString *aliasFile;
@property (nonatomic, retain) NSMutableDictionary *allCmds;

@end



@implementation CmdManager

@synthesize dbFile;
@synthesize allCmds;


- (NSString *)dbFile {
    NSString *homeDir = NSHomeDirectory();
    return [homeDir stringByAppendingString:@"/.cmd_db.plist"];
}

- (NSString *)aliasFile {
    NSString *homeDir = NSHomeDirectory();
    return [homeDir stringByAppendingString:@"/.db_cmd_alias"];
}

- (NSString *)bashProfileFile {
    NSString *homeDir = NSHomeDirectory();
    return [homeDir stringByAppendingString:@"/.bash_profile"];
}

- (NSString *)bashProfileFileForBackup {
    NSString *homeDir = NSHomeDirectory();
    NSString *bashfile_bkp = [NSString stringWithFormat: @"/.bash_profile_%@", [[NSProcessInfo processInfo] globallyUniqueString]];
    return [homeDir stringByAppendingString:bashfile_bkp];
}


- (void)createDbFileIfNeeded {
    
    NSFileManager *fm = [[NSFileManager alloc] init];
    
    if(![fm fileExistsAtPath: self.dbFile]) {
        
        NSDictionary *plist = [NSDictionary new];
        BOOL success = [plist writeToFile:self.dbFile atomically:YES];
        
        if (success) {
            printf("db file created!");
        } else {
            printf("error creating plist");
        }
    }
    
    if (self.allCmds == nil) {
        
        NSMutableDictionary *cmds = [[NSMutableDictionary alloc] initWithContentsOfFile: self.dbFile];
        self.allCmds = cmds != nil ? cmds : [NSMutableDictionary dictionary];
        
    }
}

- (void)createAliasFileIfNeeded {
    
    NSFileManager *fm = [[NSFileManager alloc] init];
    
    if(![fm fileExistsAtPath: self.aliasFile]) {
        
        NSError *error;
        BOOL success = [@"" writeToFile:self.dbFile atomically:YES encoding:NSUTF8StringEncoding error: &error];
        
        if (success) {
            printf("alias file created!");
        } else {
            printf("error creating alias file");
        }
    }
    
}


- (int)saveCommandWithAlias:(NSString *)alias for:(NSString *)requestedCommand {
    
    if (![self validAlias: alias shouldExist: NO]) {
        return 1;
    }
    
    [Logger log:[NSString stringWithFormat: @"\nadding %s%@%s for %@...", KYEL, alias, KWHT, requestedCommand]];
    
    // add new cmd
    [self.allCmds setValue:requestedCommand forKey:alias];
    
    if ([self updateShell] != 0) {
        [Logger log: @"\nShell not updated. Try it running `cmd -u` at end.\n\n"];
    }
    return [self saveUpdatedCmds];
}


- (int)deleteCmdWithAlias: (NSString *)alias {
    
    if (![self validAlias: alias shouldExist: YES]) {
        return 1;
    }
    
    [Logger log:[NSString stringWithFormat: @"\nremoving %s%@ %s...", KYEL, alias, KWHT]];
    
    [self.allCmds removeObjectForKey: alias];
    
    if ([self updateShell] != 0) {
        [Logger log: @"\nShell not updated. Try it running `cmd -u` at end.\n\n"];
    }

    return [self saveUpdatedCmds];
}

- (int)updateShell {
    
    __block NSString *cmdList = @"";
    
    [allCmds enumerateKeysAndObjectsUsingBlock:^(id key, id obj, BOOL * stop) {
        
        NSString *cmdInfo = [NSString stringWithFormat: @"alias %@='%@'\n", key, obj];
        cmdList = [cmdList stringByAppendingString: cmdInfo];
        
    }];
    
    [self createAliasFileIfNeeded];
    
    [self updateBashProfileIfNeeded];
    
    
    NSError *error;
    
    BOOL success = [cmdList writeToFile: self.aliasFile atomically:YES encoding:NSUTF8StringEncoding error:&error];
    
    if (!success) {
        [Logger log: @"\ncouldn't save alias file, exiting...\n"];
        return 1;
    }
    
    [Logger log: @"\nalias file updated.\n\n"];
    
    system("source ~/.bash_profile");
    
    return 0;
}
                              
                              
- (void)updateBashProfileIfNeeded {
      
      NSError *error;
      NSString *bashProfileContents = [NSString stringWithContentsOfFile: [self bashProfileFile] encoding:NSUTF8StringEncoding error: &error];
      
      if (error != nil) {
          [Logger log: @"\nerror getting .bash_profile\n"];
          return;
      }
      
      NSString *injectString = @"source ~/.db_alias";
      
      if ([bashProfileContents rangeOfString:injectString].location == NSNotFound) {
          
          BOOL success = [bashProfileContents writeToFile: [self bashProfileFileForBackup] atomically:YES encoding:NSUTF8StringEncoding error:&error];
          
          if (!success) {
              [Logger log: @"\ncouldn't backup .bash_profile file, too dangerous to continue, exiting...\n"];
              return;
          }
          
          bashProfileContents = [bashProfileContents stringByAppendingString:[NSString stringWithFormat:@"\n%@\n", injectString]];
          success = [bashProfileContents writeToFile:[self bashProfileFile] atomically:YES encoding:NSUTF8StringEncoding error:&error];
          
          if (!success) {
              [Logger log: @"\ncouldn't save .bash_profile file, too dangerous to continue, exiting...\n"];
              return;
          }
          
          [Logger log: @"\n.bash_profile updated\n"];
          
      }
                                  
}



- (int)saveUpdatedCmds {
    
    BOOL success = [self.allCmds writeToFile: self.dbFile atomically:YES];
    
    if (success) {
        printf("\n%sdone!%s\n\n", KGRN, KWHT);
        return 0;
    } else {
        printf("\n%serror updating plist%s\n\n", KGRN, KWHT);
        return 1;
    }
}


- (BOOL)validAlias: (NSString *)alias shouldExist:(BOOL)shouldExist {
    
    if (alias == Nil || alias.length < 1) {
        [Logger printWrongParameters];
        return NO;
    }
    
    BOOL aliasInUse = [[self.allCmds allKeys] containsObject: alias];
    
    if (aliasInUse) {
        
        if(shouldExist) { //should exist to proceed
            return YES;
            
        } else { // alias in use cannot do save operation
            [Logger log: [NSString stringWithFormat:@"\nthe alias %s%@%s is already in use, please choose another.\n\n", KYEL, alias, KWHT]];
            return NO;
        }
        
    } else {
        
        if(shouldExist) { //should exist to proceed
            
            [Logger log: [NSString stringWithFormat:@"\nthis alias %s%@%s doesn't exists.\n\n", KYEL, alias, KWHT]];
            return NO;
        }
    }
    
    // cmd already saved?
    NSString *currentpath = [[NSFileManager defaultManager] currentDirectoryPath];
    if ([[self.allCmds allValues] containsObject: currentpath]) {
        
        NSString *keyForCmd = [[self.allCmds allKeysForObject: currentpath] lastObject];
        [Logger log: [NSString stringWithFormat: @"\nthis cmd was alread saved using alias %s%@%s.\n\n", KYEL, keyForCmd, KWHT]];
        return NO;
    }
    
    
    return YES;
}


@end
