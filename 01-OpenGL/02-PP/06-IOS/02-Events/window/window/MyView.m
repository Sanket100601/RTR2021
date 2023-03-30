//
//  MyView.m
//  window
//
//  Created by Admin on 25/02/23.
//

#import "MyView.h"

@implementation MyView

- (id) initWithFrame:(CGRect)frame
{
    // code
    
    self = [super initWithFrame: frame];
    
    if(self)
    {
        // gesture regon code
        // single tap
        UITapGestureRecognizer *single_tap_gesture_recognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onSingleTap:)];
        
        [single_tap_gesture_recognizer setNumberOfTapsRequired:1];
        [single_tap_gesture_recognizer setNumberOfTouchesRequired:1];
        [single_tap_gesture_recognizer setDelegate:self];
           
        [self addGestureRecognizer:single_tap_gesture_recognizer];
        
        // double tap
        UITapGestureRecognizer *double_tap_gesture_recognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onDoubleTap:)];
            
        [double_tap_gesture_recognizer setNumberOfTapsRequired:2];
        [double_tap_gesture_recognizer setNumberOfTouchesRequired:1];
        [double_tap_gesture_recognizer setDelegate:self];
        [single_tap_gesture_recognizer requireGestureRecognizerToFail:double_tap_gesture_recognizer];
            
        [self addGestureRecognizer:double_tap_gesture_recognizer];
        
        
        // Swipe
        UISwipeGestureRecognizer *swipe_gesture_recognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(onSwipe:)];
        
        [swipe_gesture_recognizer setDelegate:self];
        
        [self addGestureRecognizer:swipe_gesture_recognizer];
        
        // long press
        UILongPressGestureRecognizer *long_press_gesture_recognizer = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(onLongPress:)];
            
        [long_press_gesture_recognizer setDelegate:self];
            
        [self addGestureRecognizer:long_press_gesture_recognizer];
        
        
        
    }
    
    return self;
}

-(BOOL) acceptsFirstResponder
{
    // code
    return YES;
}

-(void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    
}

- (void)onSingleTap:(UITapGestureRecognizer*)gr
{
    //code
    printf("Single Tap\n");
}


- (void)onDoubleTap:(UITapGestureRecognizer*)gr
{
    //code
    printf("Double Tap\n");
}
- (void)onSwipe:(UISwipeGestureRecognizer*)gr
{
    //code
    printf("Swipe Tap\n");
    [self release];
    
}
- (void) onLongPress:(UILongPressGestureRecognizer*)gr
{
    //code
    printf("Long Press Tap\n");}


- (void)dealloc
{
    //code
    [super dealloc];
}


@end 
