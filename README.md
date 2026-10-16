# C++ / OpenGL Game Engine

**A personal game engine prototype by Brandon Vaquilar, demonstrated by Blue Hedgehog Kart.**

A small 2D engine built with C++ and OpenGL, with shared systems for texture loading, layered background scrolling, timing, and circular collision detection. A kart-platformer demo integrates those systems with sprite animation, keyboard input, hazards, and three scenes. GLFW supplies the desktop window and input; SOIL loads image textures.

The engine is the main project; the platformer is its demonstration application. The shared `engine_core` library and the `platformer_demo` library have separate source and header folders. Both the desktop launcher and the offscreen capture runner use those same libraries.

## Engine systems

| System | Implementation | Demonstration |
| --- | --- | --- |
| Texture loading | `GLTexture`, with reuse of already loaded texture data | Player, backgrounds, and hazard textures |
| Layered scrolling | `GLParallax` | Forest, city, and underwater background layers |
| Timing | `GLTimer`, with monotonic milliseconds and a fixed-time capture mode | Animation, scrolling, and repeatable frame captures |
| Collision detection | `GLCheckCollision` circular overlap checks | Hazard contacts and the demo's life display |

The demo owns scene composition, sprite behavior, input mapping, hazards, and lives. This is an early engine prototype: scene composition and update/render separation are still being refined. A reusable sprite-animation component is a next step.

## Platformer demo gallery

These are actual captures from the engine running the platformer demo. No installation is needed to view them.

### Forest

![Blue hedgehog in a red kart moving through a layered forest while arrows fall](docs/images/forest.webp)

Several background layers scroll at different rates while falling arrows act as hazards. Circular overlap checks reduce the player's remaining lives after a hit.

### City

![The kart in a bright city scene with rotating rainbow star hazards](docs/images/city.webp)

A separate set of background textures and rotating star hazards changes the scene's look and movement patterns.

### Underwater

![The kart in an underwater scene with bubbles, coral, and descending hooks](docs/images/underwater.webp)

The third scene combines vertical and horizontal layer movement with hook hazards. Each scene uses the same engine systems and demo player.

## Build and review features

- Separate CMake libraries for shared engine systems and the platformer demo.
- A GLFW desktop launcher with left/right keyboard movement through three environments.
- An optional Linux/Mesa capture runner that executes the same engine and demo without a desktop window.
- Reproducible screenshots and a [capture manifest](docs/images/provenance.json) containing commands and final demo state.
- Automated checks for collision behavior, demo movement, scrolling, and deterministic timing.

The platformer demo currently supports left and right movement, hazards, five lives, and scene transitions. Jumping, attacks, lap timing, and a full racing system are future work.

## Build on Ubuntu / Debian Linux

```bash
sudo apt update
sudo apt install build-essential cmake libglfw3-dev libglu1-mesa-dev libsoil-dev freeglut3-dev libegl-dev libgl1-mesa-dri
git clone https://github.com/BrandonVaquilar/Blue-Hedgog-Kart-Platformer.git
cd Blue-Hedgog-Kart-Platformer
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

The verified build uses Linux, GCC, CMake, and Mesa. The desktop launcher requires an OpenGL-capable graphical session. Other desktop platforms need equivalent GLFW, OpenGL, GLU, GLUT, and SOIL development packages; they have not been verified here.

## Run the platformer demo

Run from the repository root:

```bash
./build/blue_hedgehog --level 0
```

| Input | Action |
| --- | --- |
| Left arrow / A | Move left |
| Right arrow / D | Move right |
| Escape | Quit |
| `--level 0`, `--level 1`, `--level 2` | Start in the forest, city, or underwater scene |
| `--help` | Show capture and window options |

The demo starts with five lives. Hazards consume lives on contact; reaching the right edge advances to the next scene. Losing all lives resets the character and life count.

## Regenerate screenshots without a desktop

```bash
sudo apt install python3-pil
python3 scripts/generate_previews.py
```

The script uses `build/blue_hedgehog_capture`, renders 240 fixed 60 Hz frames in each scene with Right held, and reads the actual OpenGL framebuffer. Pillow only converts the image format. It checks scene selection, frame counts, life bounds, movement, and visible image content before saving the gallery.

For a single capture:

```bash
LIBGL_ALWAYS_SOFTWARE=1 ./build/blue_hedgehog_capture --demo --frames 240 --level 0 --screenshot build/forest.ppm --state build/forest.json
```

The capture target is built on Linux when EGL is available. It uses Mesa's surfaceless platform and needs no X server, browser, or running desktop.

## Source organization

| Path | Purpose |
| --- | --- |
| `include/engine/`, `src/engine/` | Shared texture, parallax, timing, and collision systems; no demo dependency |
| `include/demo/`, `src/demo/` | Platformer scene composition, player animation, input mapping, hazards, and helper classes |
| `src/main.cpp` | Desktop input and render loop |
| `src/capture_main.cpp`, `src/app/` | Capture options, framebuffer output, and offscreen context |
| `assets/images/` | Bundled demo art |
| `tests/` | Collision, movement, timing, and scrolling checks |
| `scripts/`, `docs/images/` | Reproducible previews and committed results |

Generated build files stay in `build/`. Texture data is reused when hazards respawn, scrolling state is initialized, and timing uses monotonic milliseconds instead of platform-dependent CPU ticks.

To extend the engine, work in `include/engine/` and `src/engine/`; engine code does not include demo headers. Demo rules and presentation belong in `include/demo/` and `src/demo/`. The two launchers share the demo library so screenshots exercise the same implementation as the desktop application.

## Next steps

- Separate simulation updates from rendering more fully.
- Make scene composition easier to reuse across demonstrations.
- Extract sprite animation from the demo into an engine component.
- Expand the platformer demo with jumping and more platform interactions.

## Assets and references

Character and background art are bundled existing assets and are not presented as original artwork. Their creators retain their rights; this repository does not grant a separate license for third-party art.

- [GLFW documentation](https://www.glfw.org/docs/latest/)
- [Mesa EGL documentation](https://docs.mesa3d.org/egl.html)
- [Khronos OpenGL reference](https://registry.khronos.org/OpenGL/)
