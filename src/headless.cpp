//
// Created by lildeltaplus on 04/12/2020.
//


#include <iostream>
#include <vector>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_render_handler.h"
#include "include/cef_load_handler.h"

bool begin = false;

class LodHandler : public CefLoadHandler {
private:

public:
    LodHandler() = default;

    void OnLoadingStateChange( CefRefPtr< CefBrowser > browser, bool isLoading, bool canGoBack, bool canGoForward ) override {
        if(!isLoading) begin = true;
    }

IMPLEMENT_REFCOUNTING(LodHandler);
};

class OSRHandler : public CefRenderHandler {
private:
    int renderWidth;
    int renderHeight;

public:
    OSRHandler(int width, int height) {
        renderWidth = width;
        renderHeight = height;
    }

    void setRenderSize(int width, int height) {
        renderWidth = width;
        renderHeight = height;
    }

    void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override {
        rect = CefRect(0, 0, renderWidth, renderHeight);
//        return true;
    }

    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override {
        if(begin){
            auto buf = (unsigned char*)buffer;
//            std::vector<unsigned char> ImVec(buf, (unsigned char*)buffer + renderWidth*renderHeight*4);
//            cv::Mat img_output = cv::imdecode(ImVec, 1); //img_output(renderHeight,renderWidth,CV_16S,(unsigned char*)buffer);
            cv::Mat img_output(renderHeight, renderWidth, CV_8UC4, buf);
            printf("frame rendered (pixel[0]: (%d %d %d - %d)\n", buf[2], buf[1], buf[0], buf[3]);
            imshow("display", img_output);
        }
    }

IMPLEMENT_REFCOUNTING(OSRHandler);

};

class BrowserClient : public CefClient {
private:
    CefRefPtr<CefRenderHandler> m_renderHandler;
    CefRefPtr<CefLoadHandler> m_loadHandler;

public:
    explicit BrowserClient(OSRHandler *renderHandler, CefLoadHandler *loadHandler)  {
        m_renderHandler = renderHandler;
        m_loadHandler = loadHandler;
    }

    CefRefPtr<CefRenderHandler> GetRenderHandler() override {
        return m_renderHandler;
    }

    CefRefPtr<CefLoadHandler> GetLoadHandler() override {
        return m_loadHandler;
    }

IMPLEMENT_REFCOUNTING(BrowserClient);
};


int main(int argc, char* argv[]) {
    CefMainArgs main_args(argc, argv);

    int exit_code = CefExecuteProcess(main_args, nullptr, nullptr);
    if (exit_code >= 0)
        return exit_code;

    CefSettings settings;
    settings.windowless_rendering_enabled = true;
    settings.no_sandbox = true;
    CefInitialize(main_args, settings, nullptr, nullptr);

    auto* osrHandler = new OSRHandler(800, 600);
    auto* lodHandler = new LodHandler();
    CefRefPtr<BrowserClient> browserClient = new BrowserClient(osrHandler, lodHandler);


    CefBrowserSettings browserSettings;
    browserSettings.windowless_frame_rate = 1;
    CefWindowInfo window_info;
    window_info.SetAsWindowless(0);

    // Dynamic HTML example: https://dmitrybaranovskiy.github.io/raphael/polar-clock.html
    // Static HTML example: https://www.magpcss.org/ceforum/index.php
    CefRefPtr<CefBrowser> browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient.get(), "https://dmitrybaranovskiy.github.io/raphael/polar-clock.html", browserSettings,
                                                                      nullptr, nullptr);
    CefRunMessageLoop();

    CefShutdown();
    return 0;
}