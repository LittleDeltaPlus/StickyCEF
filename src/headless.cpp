//
// Created by lildeltaplus on 04/12/2020.
//

#include <iostream>
#include <vector>

#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_render_handler.h"
#include "include/cef_load_handler.h"
#include "inc/update_inky.h"

bool pgLoaded = false;


class LodHandler : public CefLoadHandler {
private:

public:
    LodHandler() = default;

    void OnLoadingStateChange( CefRefPtr< CefBrowser > browser, bool isLoading, bool canGoBack, bool canGoForward ) override {
        bool appliedCSS = false;
        CefRefPtr<CefFrame> frame = browser->GetMainFrame();
        std::string strURL = frame->GetURL();
        if (isLoading == 0) // && strURL is your page with a form
        {
            const CefString jscode =
                    "let sheet = window.document.styleSheets[0];\n"
                    "sheet.insertRule('html {  filter: grayscale(100%); }', sheet.cssRules.length);\n"
                    "sheet.insertRule('body {  background-color: #FFFFFF; }', sheet.cssRules.length);\n"
                    "sheet.insertRule('* {  color: #000000; }', sheet.cssRules.length);\n"
                    "sheet.insertRule('::-webkit-scrollbar { width: 0px; }', sheet.cssRules.length);";
            frame->ExecuteJavaScript(jscode, frame->GetURL(), 0);
            appliedCSS = true;
        }

        if(!appliedCSS) pgLoaded = true;
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
        if(pgLoaded){
            auto buf = (unsigned char*)buffer;
            auto* mono = (unsigned char*) malloc(renderWidth * renderHeight * 4 * sizeof (int));
//            std::vector<unsigned char> mono_vec(renderWidth * renderHeight * 4);
//            for (int i = 0; i < renderWidth*renderHeight; i++) {
//                mono_vec[i] = (buf[i * 4]);
//                mono[i] = (buf[i * 4]);
//            }
            UpdateInky(reinterpret_cast<const char*>(mono));
            printf("frame rendered (pixels[1-3]: (%d, %d, %d)\n", mono[0], mono[1], mono[2]);

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

    auto* osrHandler = new OSRHandler(400, 300);
    auto* lodHandler = new LodHandler();
    CefRefPtr<BrowserClient> browserClient = new BrowserClient(osrHandler, lodHandler);



    CefBrowserSettings browserSettings;
    browserSettings.windowless_frame_rate = 1;
    CefWindowInfo window_info;
    window_info.SetAsWindowless(0);


    // Dynamic HTML example: https://dmitrybaranovskiy.github.io/raphael/polar-clock.html
    // Static HTML example: https://www.magpcss.org/ceforum/index.php
    CefRefPtr<CefBrowser> browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient.get(), "https://www.magpcss.org/ceforum/index.php", browserSettings,

                                                                      nullptr, nullptr);
    CefRunMessageLoop();

    CefShutdown();
    return 0;
}