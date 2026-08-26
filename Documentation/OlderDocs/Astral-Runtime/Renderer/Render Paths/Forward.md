
## Forward Rendering Path


#### Pre-Depth Pass (MSAA x4)

A pass before the main geometry pass that only writes to the depth buffer to help prevent overdraw and extra work
when calculating lighting in the Lighting pass.


#### Cascaded Shadow Map Pass

Calculates the shadows in the scene utilizing layered rendering to perform cascaded shadow mapping. 


#### Lighting Pass (MSAA x4)

Uses a metallic-roughness PBR workflow and the Cook-Torrence model to calculate lighting from point lights and directional lights.
Additionally utilizes image-based lighting for indirect global lighting.


#### Environment Map Pass (MSAA x4)

A pass to draw the environment map behind all rendered geometry.

#### Tone Mapping Pass

Transforms HDR values in the ACEScg space into SDR sRGB space.