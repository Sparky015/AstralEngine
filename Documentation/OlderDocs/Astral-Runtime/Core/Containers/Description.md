## Brief Container Descriptions

------

Currently, I have two data structures I developed to use in the engine. A bitmap and a directed graph.


#### Bitmap

A Bitmap for CPU sided texture access. It has a simple and easy to use API to interact with texture
resources. You can access any pixel in the texture by querying the data at the pixel coordinates. I first used it
to convert a equirectangular image to a cube map on the CPU, but in the future I intend to use it when reading back
textures that were rendered to on the GPU.


#### Directed Graph

A directed graph meant for use in the Frame Graph / Render Graph system. When I first started to implement a Render Graph
for my Renderer, I was surprised to find that the C++ standard library did not contain an implementation for graphs data structures
even though there is implementations for many other data structures. So I used my experience gained in my data structures class, to quickly
put together a directed graph implementation specifically for the Render Graph. It is not intended to be a fully featured 
data structure but rather to support functionality for the Render Graph.


