# SimpleRenderer

A simple software renderer utilizing the Win32 api.

Currently features multithreaded triangle rasterization on a Win32 window with color and depth. This data is written to a frame buffer that is then copied  to the window's GDI bitmap.