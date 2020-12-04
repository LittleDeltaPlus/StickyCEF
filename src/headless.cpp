//
// Created by lildeltaplus on 04/12/2020.
//


#include <iostream>

#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_render_handler.h"

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
        auto* img = (unsigned char*)buffer;
        printf("frame rendered (pixel[0]: (%d %d %d - %d)\n", img[2], img[1], img[0], img[3]);
    }

IMPLEMENT_REFCOUNTING(OSRHandler);

};

class BrowserClient : public CefClient {
private:
    CefRefPtr<CefRenderHandler> m_renderHandler;

public:
    explicit BrowserClient(OSRHandler *renderHandler)  {
        m_renderHandler = renderHandler;
    }

    CefRefPtr<CefRenderHandler> GetRenderHandler() override {
        return m_renderHandler;
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
    CefRefPtr<BrowserClient> browserClient = new BrowserClient(osrHandler);

    CefBrowserSettings browserSettings;
    CefWindowInfo window_info;
    window_info.SetAsWindowless(0);


    CefRefPtr<CefBrowser> browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient.get(), "https://dmitrybaranovskiy.github.io/raphael/polar-clock.html", browserSettings,
                                                                      nullptr, nullptr);

    CefRunMessageLoop();

    CefShutdown();
    return 0;
}