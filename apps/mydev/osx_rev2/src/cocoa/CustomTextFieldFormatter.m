//
//  CustomTextFieldFormatter.m
//  osx_picto_rev02
//
//  Created by matobahiroshi on 2013/08/21.
//
//

#import "CustomTextFieldFormatter.h"

@implementation CustomTextFieldFormatter

- (id)init {
    
    if(self = [super init]){
        
    }
    
    return self;
}

- (NSString *)stringForObjectValue:(id)object {
    return (NSString *)object;
}

- (BOOL)getObjectValue:(id *)object forString:(NSString *)string errorDescription:(NSString **)error {
    *object = string;
    return YES;
}

- (BOOL)isPartialStringValid:(NSString **)partialStringPtr
       proposedSelectedRange:(NSRangePointer)proposedSelRangePtr
              originalString:(NSString *)origString
       originalSelectedRange:(NSRange)origSelRange
            errorDescription:(NSString **)error {
    
    if (![*partialStringPtr isEqual:[*partialStringPtr uppercaseString]]) {
        *partialStringPtr = [*partialStringPtr uppercaseString];
        return NO;
    }
    
    return YES;
}

- (NSAttributedString *)attributedStringForObjectValue:(id)anObject withDefaultAttributes:(NSDictionary *)attributes {
    return nil;
}

@end