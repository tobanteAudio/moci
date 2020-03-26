#include "moci/moci.hpp"

#include "mtlpp.hpp"

#define GLFW_INCLUDE_NONE
#import <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#import <GLFW/glfw3native.h>

#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>
#import <simd/simd.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>

int main(int, char**)
{

    moci::Log::Init();
    glfwSetErrorCallback([](int error, const char* description) { MOCI_ERROR("{}", description); });

    if (!glfwInit()) {
        MOCI_ERROR("Failed to init glfw");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "GLFW Metal", NULL, NULL);
    if (!window) {
        glfwTerminate();
        MOCI_ERROR("Failed to init window");
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    });

    mtlpp::Device dev = mtlpp::Device::CreateSystemDefaultDevice();
    id<MTLDevice> device = (__bridge id<MTLDevice>)dev.GetPtr();

    if (!device) {
        MOCI_ERROR("Failed to init device");
        exit(EXIT_FAILURE);
    }

    NSWindow* nswin = glfwGetCocoaWindow(window);
    CAMetalLayer* layer = [CAMetalLayer layer];
    layer.device = device;
    layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    nswin.contentView.layer = layer;
    nswin.contentView.wantsLayer = YES;

    constexpr char shaderSrc[] = R"""(
        #include <metal_stdlib>
        using namespace metal;
        vertex float4 v_simple (
            constant float4* in  [[buffer(0)]],
            uint             vid [[vertex_id]])
        {
            return in[vid];
        }
        fragment float4 f_simple (float4 in [[stage_in]])
        {
            return float4(1, 0, 0, 1);
        }
    )""";

    mtlpp::Library library = dev.NewLibrary(shaderSrc, mtlpp::CompileOptions(), nullptr);
    MOCI_ASSERT(library, "");
    mtlpp::Function vertFunc = library.NewFunction("v_simple");
    MOCI_ASSERT(vertFunc, "");
    mtlpp::Function fragFunc = library.NewFunction("f_simple");
    MOCI_ASSERT(fragFunc, "");

    id<MTLCommandQueue> cq = [device newCommandQueue];
    MOCI_ASSERT(cq, "");

    mtlpp::RenderPipelineDescriptor renderPipelineDesc;
    renderPipelineDesc.SetVertexFunction(vertFunc);
    renderPipelineDesc.SetFragmentFunction(fragFunc);
    renderPipelineDesc.GetColorAttachments()[0].SetPixelFormat(mtlpp::PixelFormat::BGRA8Unorm);
    mtlpp::RenderPipelineState renderPipelineState = dev.NewRenderPipelineState(renderPipelineDesc, nullptr);
    MOCI_ASSERT(renderPipelineState, "");
    id<MTLRenderPipelineState> rps = (__bridge id<MTLRenderPipelineState>)renderPipelineState.GetPtr();

    while (!glfwWindowShouldClose(window)) {
        float ratio;
        int width, height;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        layer.drawableSize = CGSizeMake(width, height);
        id<CAMetalDrawable> drawable = [layer nextDrawable];
        assert(drawable);

        id<MTLCommandBuffer> cb = [cq commandBuffer];

        MTLRenderPassDescriptor* rpd = [MTLRenderPassDescriptor new];
        MTLRenderPassColorAttachmentDescriptor* cd = rpd.colorAttachments[0];
        cd.texture = drawable.texture;
        cd.loadAction = MTLLoadActionClear;
        cd.clearColor = MTLClearColorMake(0.1, 0.1, 0.1, 1.0);
        cd.storeAction = MTLStoreActionStore;
        id<MTLRenderCommandEncoder> rce = [cb renderCommandEncoderWithDescriptor:rpd];

        [rce setRenderPipelineState:rps];
        [rce setVertexBytes:(vector_float4[]) {
                                { 0, 0, 0, 1 },
                                { -1, -1, 0, 1 },
                                { 1, -1, 0, 1 },
                            }
                     length:3 * sizeof(vector_float4)
                    atIndex:0];
        [rce drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];

        [rce endEncoding];
        [cb presentDrawable:drawable];
        [cb commit];

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}

//! [code]
