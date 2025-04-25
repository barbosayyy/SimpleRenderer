# SimpleRenderer

A multi-threaded software renderer utilizing the Win32 api.

## Method
The goal of this project was to go as low and close to the legacy Windows API as possible.

Currently, the pipeline consists mainly of creating, combining and blitting a color and depth framebuffer (frame width * frame height * 4 bytes (rgb + depth)) while staying true to a format that can be used by the GDI window viewport.

The partition of the screen into multiple tiles allows for simple multi-threaded software rendering and therefore better performance. These tiles will be assigned to render tasks via a custom job system specifically made for this purpose.

The processing of the tiles themselves determine if the fragments are respectively within the bounding box barycenter of the triangle vertices.

The final result gives us somewhat decent depth checking and a flat coloring of the triangles.

## Screenshot
![Renderer](https://github.com/user-attachments/assets/35ed6f36-da68-41bb-81aa-218632b898e9)
