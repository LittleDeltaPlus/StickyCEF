# StickyCEF
 A CEF Browser For Headless EPD Devices, created for EEE Final Year project at University of Nottingham.

The program is currently a work in progress, and is mostly functional using a Raspberry Pi 3 B+.
## Installation:
The entire project can be installed using CMake on the target device with the usual dependendencies alongside the [inky library](https://github.com/pimoroni/inky) for the display IO and `libgtkglextmm-x11-1.2-dev` to include x11. 

reccomneded build steps from the project root:
1. `$ mkdir build && cd build`
2. `$ cmake -G 'Ninja' ../`
3. `$ sudo ninja install`


## Usage:
once installed SticyCEF can be run headlessly on a raspberry Pi through the following command:

`StickyCef --headless --no-sandbox --use-gl=egl --disable-gpu-blacklist -url [url]`

this will display rendered web-content on an inky-WHAT perpetually. Unfortunately the CEF switches used in are required as compatibility work-arounds. 

Additionaly, a minimal GUI is available [here](https://github.com/LittleDeltaPlus/StickyCEF_Server).

## Notes:

- This project is inteneded to display web-pages developed with EPDs in mind, as such not all pre-existing web-pages display well (or at all) on the device.
- If  no url is specified, StickyCEF will default to [this webpage](https://lil-delta.dev/Dynamic_Test/). 
- The browser renders a page with the dimensions 400x300
- If you are using the device entirely headlessly use xvfb to create a virtual display:
    >`Xvfb -ac :99 -screen 0 1280x1024x16 &` <br>
    `export DISPLAY=:99`<br>

### References
1. browser functionality extended from:  [cef-forum](https://magpcss.org/ceforum/viewtopic.php?f=6&t=11572)
2. CMAKE setup from: [cef-project](https://bitbucket.org/chromiumembedded/cef-project/src/master/)
3. Display library from: [Pimoroni](https://github.com/pimoroni/inky)

