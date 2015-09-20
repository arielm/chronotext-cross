/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#import "NSDictionary+DefautValues.h"
#import "NSMutableDictionary+Merge.h"
#import "NSString+JSON.h"

#if DEBUG
    #define DLOG(FORMAT,...) NSLog(@"%@",[NSString stringWithFormat:(FORMAT), ##__VA_ARGS__]);
#else
    #define DLOG(...)
#endif
