
## Deferred Rendering Path


#### Geometry Pass

Iterates over all the geometry viewable to the camera and populates their material properties into the GBuffer.

#### Cascaded Shadow Map Pass

Calculates the shadows in the scene utilizing layered rendering to perform cascaded shadow mapping.

#### Lighting Pass

Uses a metallic-roughness PBR workflow and the Cook-Torrence model to calculate lighting from point lights and directional lights.
Additionally utilizes image-based lighting for indirect global lighting.

#### Environment Map Pass

A pass to draw the environment map behind all rendered geometry.

#### Tone Mapping Pass

Transforms HDR values in the ACEScg space into SDR sRGB space.

#### FXAA Pass

Performs FXAA for anti-aliasing 