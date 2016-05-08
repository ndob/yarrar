#import <GLKit/GLKit.h>
#import <AVFoundation/AVFoundation.h>

@interface YarrarViewController : GLKViewController <AVCaptureVideoDataOutputSampleBufferDelegate>
- (void)onYarrarInitialized;
- (void)addModel:(int)modelId withName:(NSString*)name withVertices:(NSArray*)vertexArray withTranslation:(vector_float3)translation;
@end
