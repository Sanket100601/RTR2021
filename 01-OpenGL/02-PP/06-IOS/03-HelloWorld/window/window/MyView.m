//
//  MyView.m
//  window
//
//  Created by Admin on 25/02/23.
//

#import "MyView.h"

@implementation MyView
{
    @private
    NSString* string;
    
}


- (id) initWithFrame:(CGRect)frame
{
    // code
    
    self = [super initWithFrame: frame];
    
    if(self)
    {
        
        UIColor *background_color = [UIColor blackColor];
        [background_color set];
        
        //UIRectFill(dirtyRect);
        string = @"Hello World !!!";
        
        
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

-(void)drawRect:(CGRect)dirtyRect
{
   // code
   NSDictionary *dictionaryForTextAttributes =  [NSDictionary dictionaryWithObjectsAndKeys:
                                                 [UIFont fontWithName:@"Helvetica" size:24],
                                                 NSFontAttributeName,
                                                 [UIColor greenColor],
                                                 NSForegroundColorAttributeName,
                                                 nil];
    
   CGSize textSize = [string sizeWithAttributes:dictionaryForTextAttributes];

   CGPoint point;
   point.x = (dirtyRect.size.width / 2) - (textSize.width / 2) + 12;
   point.y = (dirtyRect.size.height / 2) - (textSize.height / 2) + 12;

   [string drawAtPoint:point withAttributes:dictionaryForTextAttributes];

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
    string = @"Single Tap";
    [self setNeedsDisplay];
    //printf("Single Tap\n");
}


- (void)onDoubleTap:(UITapGestureRecognizer*)gr
{
    //code
    //printf(\n");
    string = @"Double Tap";
    [self setNeedsDisplay];
}

- (void)onSwipe:(UISwipeGestureRecognizer*)gr
{
    //code
    [self release];
}

- (void) onLongPress:(UILongPressGestureRecognizer*)gr
{
    //code
    
    string = @"Long Press";
    [self setNeedsDisplay];
}

- (void)dealloc
{
    //code
    [super dealloc];
}


@end 
