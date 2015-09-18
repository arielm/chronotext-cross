/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#import <Foundation/Foundation.h>

@interface NSDictionary (DefautValues)

- (BOOL) boolForKey:(NSString*)key;
- (BOOL) boolForKey:(NSString*)key defaultValue:(BOOL)defaultValue;

- (int) intForKey:(NSString*)key;
- (int) intForKey:(NSString*)key defaultValue:(int)defaultValue;

- (unsigned int) unsignedIntForKey:(NSString*)key;
- (unsigned int) unsignedIntForKey:(NSString*)key defaultValue:(unsigned int)defaultValue;

- (float) floatForKey:(NSString*)key;
- (float) floatForKey:(NSString*)key defaultValue:(float)defaultValue;

- (NSString*) stringForKey:(NSString*)key;
- (NSString*) stringForKey:(NSString*)key defaultValue:(NSString*)defaultValue;

@end
