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
#include "inc/pyhelper.hpp"

bool pgLoaded = false;
signed char frameMultiplier = 0;
InkyHelper inkyHelper;

CPyObject pInkyFunction;

static int find_keyword(int argc, char *argv[], const char * keyword);

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
                    "sheet.insertRule('body {  background-color: #FFFFFF; overflow: hidden !important;}', sheet.cssRules.length);\n"
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
    }

    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override {
        if(pgLoaded and frameMultiplier > 15){
            auto buf = (unsigned char*)buffer;
            auto* mono = (unsigned char*) malloc(renderWidth * renderHeight * sizeof (unsigned char));
            for (int i = 0; i < renderWidth*renderHeight; i++) {
                mono[renderWidth * renderHeight - (1 + i)] = (buf[i * 4]);
            }
            UpdateInky(pInkyFunction, reinterpret_cast<const char*>(mono));
            printf("frame rendered (pixels[1-3]: (%d, %d, %d)\n", mono[0], mono[1], mono[2]);
            frameMultiplier = 0;
        } else {
            frameMultiplier++;
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

    const int urlIndex = find_keyword(argc, argv, "-url");
    CefStringUTF16 loadURL;
    if(urlIndex < 0){
        loadURL = "https://lil-delta.dev/Static_Test";
    } else {
        loadURL = argv[urlIndex+1];
    }

    CefMainArgs main_args(argc, argv);

    pInkyFunction = StartInky();

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

    CefRefPtr<CefBrowser> browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient.get(), loadURL, browserSettings,
                                                                      nullptr, nullptr);
    CefRunMessageLoop();

    CefShutdown();
    CloseInky();
    return 0;
}

static int find_keyword(int argc, char *argv[], const char * keyword)
{
    for (int i=0; i<argc; i++)
    {
        if (strcmp(argv[i], keyword) == 0) return i;
    }
    return -1;
}