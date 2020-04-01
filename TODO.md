# ToDo

## 3D

- Add perspective camera to moci

## Application

- Add events.
  - Lost/Gain Focus

## Components

- MouseListener

## Fonts

- Scaling?
- Font Atlas (multiple fonts)

## Graphics Backend

### Metal

- Improve GLFW example.
  - Load shader from file
  - Uniforms
  - View Project
  - Model Loading
- 2D Batch Renderer.

### OpenGL

- Select shader file based on api. GL4 or ES2.
- TextureCube resize based on original resolution & max on es2/ARM
- Fix GL4 texture upload
- Add abstraction for texture color format
  - Auto-detect

## Misc

- Fix Rectangle origin to top left. Same as GLFW.

## Textures

- Load highest resolution from disk. Scale for device.
