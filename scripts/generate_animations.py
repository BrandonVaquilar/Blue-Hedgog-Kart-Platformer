"""Export five-second GIFs from real OpenGL frames with seeded keyboard inputs."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import random
import subprocess

from PIL import Image, ImageStat
from animation_encoding import encode_animation

ROOT = Path(__file__).resolve().parents[1]
FRAMES = 300  # Five seconds of the engine's fixed 60 Hz simulation.
WARMUP = 180  # Move into the scene before the five-second recording begins.
STRIDE = 6    # 50 GIF frames at 10 fps; each frame is exactly 100 ms.
SIZE = (640, 360)


def input_plan(seed):
    rng = random.Random(seed)
    segments = []

    def add(action, direction, count):
        segments.append({"action": action, "direction": direction, "frames": count})

    add("hold right", 1, rng.randint(80, 100))
    middle = ["hold left", "tap right", "tap left", "idle"]
    rng.shuffle(middle)
    for action in middle:
        if action == "idle":
            add(action, 0, rng.randint(24, 36))
        elif action == "hold left":
            add(action, -1, rng.randint(30, 42))
        else:
            direction = 1 if action == "tap right" else -1
            for _ in range(3):
                add(action, direction, rng.randint(3, 6))
                add("release", 0, rng.randint(4, 7))
    add("hold right", 1, FRAMES - sum(s["frames"] for s in segments))
    assert segments[-1]["frames"] > 0
    inputs = [s["direction"] for s in segments for _ in range(s["frames"])]
    assert len(inputs) == FRAMES and set(inputs) == {-1, 0, 1}
    return segments, inputs


def capture(binary, level, name, seed):
    directory = ROOT / "build/animations" / name
    directory.mkdir(parents=True, exist_ok=True)
    # Remove only this script's prior frame files before recording a fresh run.
    for pattern in ("[0-9][0-9][0-9][0-9][0-9].ppm", "[0-9][0-9][0-9][0-9][0-9].json"):
        for path in directory.glob(pattern):
            path.unlink()
    segments, inputs = input_plan(seed)
    script = directory / "inputs.txt"
    script.write_text("\n".join(map(str, [1] * WARMUP + inputs)) + "\n")
    executable = str(binary.relative_to(ROOT)) if binary.is_relative_to(ROOT) else str(binary)
    command = [executable, "--frames", str(WARMUP + FRAMES), "--level", str(level),
               "--seed", str(seed), "--width", str(SIZE[0]), "--height", str(SIZE[1]),
               "--input-script", str(script.relative_to(ROOT)),
               "--frame-directory", str(directory.relative_to(ROOT)),
               "--capture-every", str(STRIDE),
               "--state", str((directory / "final.json").relative_to(ROOT))]
    result = subprocess.run(command, cwd=ROOT,
                            env={**os.environ, "LIBGL_ALWAYS_SOFTWARE": "1"},
                            capture_output=True, text=True, check=True, timeout=120)
    images, states = [], []
    for index in range(FRAMES // STRIDE):
        sample_index = WARMUP // STRIDE + index
        with Image.open(directory / f"{sample_index:05d}.ppm") as image:
            assert image.size == SIZE and max(ImageStat.Stat(image).stddev) > 10
            images.append(image.convert("RGB"))
        state = json.loads((directory / f"{sample_index:05d}.json").read_text())
        assert state["frames"] == WARMUP + index * STRIDE + 1 and state["level"] == level, (name, index, state)
        assert 1 <= state["lives"] <= 5 and -10.5 <= state["player_x"] <= 12
        states.append(state)
    deltas = [b["player_x"] - a["player_x"] for a, b in zip(states, states[1:])]
    assert any(.04 < dx < .2 for dx in deltas), "Real player must move right"
    assert any(-.2 < dx < -.04 for dx in deltas), "Real player must move left"
    assert any(abs(dx) < .001 for dx in deltas), "Released input must stop the player"
    output = ROOT / "docs/images" / f"{name}.gif"
    poster = ROOT / "docs/images/city-poster.webp" if name == "city" else None
    encoding = encode_animation(images, output, poster)
    final = json.loads((directory / "final.json").read_text())
    assert final["level"] == level and final["frames"] == WARMUP + FRAMES
    print(f"{name}: {output.stat().st_size:,} bytes, {encoding['gif_frames']} GIF frames, 5,000 ms; left/right/idle verified", flush=True)
    return {"image": f"docs/images/{name}.gif", "seed": seed,
            "command": command, "stdout": result.stdout,
            "warmup_frames": WARMUP, "warmup_input": "hold right",
            "simulation_frames": WARMUP + FRAMES, "recorded_frames": FRAMES,
            "sampled_frames": FRAMES // STRIDE, **encoding,
            "input_segments": segments, "final_state": final,
            "sha256": hashlib.sha256(output.read_bytes()).hexdigest()}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--binary", default="build/blue_hedgehog_capture")
    args = parser.parse_args()
    binary = (ROOT / args.binary).resolve()
    runs = [capture(binary, level, name, seed)
            for level, (name, seed) in enumerate([("forest", 1107), ("city", 2203), ("underwater", 3301)])]
    (ROOT / "docs/images/animations.json").write_text(json.dumps({
        "source": "Actual framebuffer output from the shared native C++/OpenGL engine and demo",
        "capture": "180 fixed 60 Hz warmup frames, then 300 recorded simulation frames; every sixth framebuffer encoded at 10 fps, 640x360",
        "inputs": "Seeded randomized held keys, short left/right presses, releases, and idle periods",
        "conversion": "Full 256-color per-frame GIF palettes; no color grading, drawn, or generated gameplay",
        "runs": runs,
    }, indent=2) + "\n")


if __name__ == "__main__":
    main()
