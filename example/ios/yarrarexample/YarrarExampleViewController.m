#import "YarrarExampleViewController.h"

@implementation YarrarExampleViewController

- (void) onYarrarInitialized
{
    NSArray* vertices = @[
        // bottom
        @-1.0f, @-1.0f, @0.0f,
        @1.0f, @-1.0f, @0.0f,
        @-1.0f, @-1.0f, @2.0f,
        @1.0f, @-1.0f, @0.0f,
        @1.0f, @-1.0f, @2.0f,
        @-1.0f, @-1.0f, @2.0f,
        
        // top
        @-1.0f, @1.0f, @0.0f,
        @-1.0f, @1.0f, @2.0f,
        @1.0f, @1.0f, @0.0f,
        @1.0f, @1.0f, @0.0f,
        @-1.0f, @1.0f, @2.0f,
        @1.0f, @1.0f, @2.0f,
        
        // front
        @-1.0f, @-1.0f, @2.0f,
        @1.0f, @-1.0f, @2.0f,
        @-1.0f, @1.0f, @2.0f,
        @1.0f, @-1.0f, @2.0f,
        @1.0f, @1.0f, @2.0f,
        @-1.0f, @1.0f, @2.0f,
        
        // back
        @-1.0f, @-1.0f, @0.0f,
        @-1.0f, @1.0f, @0.0f,
        @1.0f, @-1.0f, @0.0f,
        @1.0f, @-1.0f, @0.0f,
        @-1.0f, @1.0f, @0.0f,
        @1.0f, @1.0f, @0.0f,
        
        // left
        @-1.0f, @-1.0f, @2.0f,
        @-1.0f, @1.0f, @0.0f,
        @-1.0f, @-1.0f, @0.0f,
        @-1.0f, @-1.0f, @2.0f,
        @-1.0f, @1.0f, @2.0f,
        @-1.0f, @1.0f, @0.0f,
        
        // right
        @1.0f, @-1.0f, @2.0f,
        @1.0f, @-1.0f, @0.0f,
        @1.0f, @1.0f, @0.0f,
        @1.0f, @-1.0f, @2.0f,
        @1.0f, @1.0f, @0.0f,
        @1.0f, @1.0f, @2.0f
    ];
    
    vector_float3 translation = {0.0f, 0.0f, 0.0f};
    [self addModel:30
          withName:@"cube"
      withVertices:vertices
   withTranslation:translation];
}
@end
