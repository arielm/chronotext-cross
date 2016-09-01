/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#import "NSDictionary+DefautValues.h"

@implementation NSDictionary (DefautValues)

- (BOOL) boolForKey:(NSString*)key
{
    return [[self objectForKey:key] boolValue];
}

- (BOOL) boolForKey:(NSString*)key defaultValue:(BOOL)defaultValue
{
    if ([[self allKeys] containsObject:key])
    {
        return [[self objectForKey:key] boolValue];
    }
    
    return defaultValue;
}

- (int) intForKey:(NSString*)key
{
    return [[self objectForKey:key] intValue];
}

- (int) intForKey:(NSString*)key defaultValue:(int)defaultValue
{
    if ([[self allKeys] containsObject:key])
    {
        return [[self objectForKey:key] intValue];
    }
    
    return defaultValue;
}

- (unsigned int) unsignedIntForKey:(NSString*)key
{
    return [[self objectForKey:key] unsignedIntValue];
}

- (unsigned int) unsignedIntForKey:(NSString*)key defaultValue:(unsigned int)defaultValue
{
    if ([[self allKeys] containsObject:key])
    {
        return [[self objectForKey:key] unsignedIntValue];
    }
    
    return defaultValue;
}

- (float) floatForKey:(NSString*)key
{
    return [[self objectForKey:key] floatValue];
}

- (float) floatForKey:(NSString*)key defaultValue:(float)defaultValue
{
    if ([[self allKeys] containsObject:key])
    {
        return [[self objectForKey:key] floatValue];
    }
    
    return defaultValue;
}

- (NSString*) stringForKey:(NSString*)key
{
    return [self objectForKey:key];
}

- (NSString*) stringForKey:(NSString*)key defaultValue:(NSString*)defaultValue
{
    if ([[self allKeys] containsObject:key])
    {
        return [self objectForKey:key];
    }
    
    return defaultValue;
}

@end
