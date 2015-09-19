/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#import <Foundation/Foundation.h>

@interface NSMutableDictionary (Merge)

+ (id) dictionaryWithObjectsFromDictionary:(NSDictionary*)otherDictionary withKeys:(id)firstKey, ... NS_REQUIRES_NIL_TERMINATION;
- (void) addObjectsFromDictionary:(NSDictionary*)otherDictionary withKeys:(id)firstKey, ... NS_REQUIRES_NIL_TERMINATION;

@end
