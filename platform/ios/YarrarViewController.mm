#import "YarrarViewController.h"
#import <AVFoundation/AVFoundation.h>

#include "yarrar/Pipeline.h"
#include "yarrar/dataprovider/IOSCameraProvider.h"

#include <string>

@interface YarrarViewController ()
{
    yarrar::Pipeline* m_pipeline;
    AVCaptureSession* m_session;
    AVCaptureDevice* m_device;
    AVCaptureDeviceInput* m_input;
    AVCaptureVideoDataOutput* m_output;
}

@property(strong, nonatomic) EAGLContext* context;

- (void)setupGL;
- (void)setupVideoCapture;
- (void)setupYarrar;
- (void)captureOutput:(AVCaptureOutput*)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection*)connection;

@end

@implementation YarrarViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if(!self.context)
    {
        NSLog(@"Failed to create ES context");
    }

    GLKView* view = (GLKView*) self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;

    [self setupVideoCapture];
    [self setupGL];
    [self setupYarrar];
}

- (void)dealloc
{
    delete m_pipeline;

    [EAGLContext setCurrentContext:self.context];
    if([EAGLContext currentContext] == self.context)
    {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
}

- (void)setupVideoCapture
{
    NSError* error = nil;
    m_session = [AVCaptureSession new];
    m_session.sessionPreset = AVCaptureSessionPresetMedium;

    // TODO: front/back camera
    m_device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    [m_device lockForConfiguration:nil];
    m_device.activeVideoMinFrameDuration = CMTimeMake(1, 15);
    [m_device unlockForConfiguration];

    m_input = [AVCaptureDeviceInput deviceInputWithDevice:m_device
                                                    error:&error];
    if(!m_input)
    {
        NSLog(@"Can't open camera");
        return;
    }

    [m_session addInput:m_input];

    m_output = [AVCaptureVideoDataOutput new];
    dispatch_queue_t queue = dispatch_queue_create("cameraQueue", NULL);
    [m_output setSampleBufferDelegate:self queue:queue];

    m_output.videoSettings = [NSDictionary dictionaryWithObject:
                                               [NSNumber numberWithInt:kCVPixelFormatType_32BGRA]
                                                         forKey:(id) kCVPixelBufferPixelFormatTypeKey];
    [m_session addOutput:m_output];
    [m_session startRunning];
}

- (void)captureOutput:(AVCaptureOutput*)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection*)connection
{
    CVImageBufferRef bufferRef = CMSampleBufferGetImageBuffer(sampleBuffer);

    CVPixelBufferLockBaseAddress(bufferRef, 0);

    int width = static_cast<int>(CVPixelBufferGetWidth(bufferRef));
    int height = static_cast<int>(CVPixelBufferGetHeight(bufferRef));
    unsigned char* baseAddress = (unsigned char*) CVPixelBufferGetBaseAddress(bufferRef);
    cv::Mat image = cv::Mat(height, width, CV_8UC4, baseAddress).clone();

    CVPixelBufferUnlockBaseAddress(bufferRef, 0);

    yarrar::IOSCameraProvider::injectCameraSize(width, height);
    yarrar::IOSCameraProvider::injectCameraFrame(image);
}

- (void)setupYarrar
{
    yarrar::IOSCameraProvider::injectCameraSize(480, 360);
    m_pipeline = new yarrar::Pipeline(std::string("pipeline.json"));

    yarrar::Model cube;
    cube.name = "cube";
    cube.translation = { 0.0f, 0.0f, 0.0f };
    cube.vertices = {
        // bottom
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 2.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 2.0f,
        -1.0f, -1.0f, 2.0f,

        // top
        -1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 2.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 2.0f,
        1.0f, 1.0f, 2.0f,

        // front
        -1.0f, -1.0f, 2.0f,
        1.0f, -1.0f, 2.0f,
        -1.0f, 1.0f, 2.0f,
        1.0f, -1.0f, 2.0f,
        1.0f, 1.0f, 2.0f,
        -1.0f, 1.0f, 2.0f,

        // back
        -1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,

        // left
        -1.0f, -1.0f, 2.0f,
        -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 2.0f,
        -1.0f, 1.0f, 2.0f,
        -1.0f, 1.0f, 0.0f,

        // right
        1.0f, -1.0f, 2.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 2.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 2.0
    };

    m_pipeline->addModel(30, cube);
}

- (void)glkView:(GLKView*)view drawInRect:(CGRect)rect
{
    m_pipeline->run();
}

@end
