"""Capture the real game renderer and convert framebuffer files for the web."""
import json
import io
import os
import subprocess
from pathlib import Path

from PIL import Image, ImageStat

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/captures"
OUTPUT = ROOT / "docs/images"
BUILD.mkdir(parents=True, exist_ok=True)
OUTPUT.mkdir(parents=True, exist_ok=True)
records = []
for level, name in enumerate(["forest", "city", "underwater"]):
    command = ["build/blue_hedgehog_capture", "--demo", "--frames", "240", "--seed", "42", "--level", str(level),
               "--screenshot", f"build/captures/{name}.ppm", "--state", f"build/captures/{name}.json"]
    result = subprocess.run(command, cwd=ROOT, env={**os.environ, "LIBGL_ALWAYS_SOFTWARE": "1"},
                            capture_output=True, text=True, check=True, timeout=45)
    state = json.loads((BUILD / f"{name}.json").read_text())
    assert state["level"] == level and state["frames"] == 240
    assert 1 <= state["lives"] <= 5 and -10.5 <= state["player_x"] <= 12
    assert state["player_x"] > -10.2, "Scripted Right input must move the actual player"
    with Image.open(BUILD / f"{name}.ppm") as image:
        assert image.size == (1280, 720)
        assert max(ImageStat.Stat(image).stddev) > 10, "Framebuffer must contain visible rendered content"
        encoded = io.BytesIO()
        image.save(encoded, format="WEBP", quality=90)
        payload = encoded.getvalue()
        assert len(payload) > 0, "WebP encoder must produce a complete image"
        target = OUTPUT / f"{name}.webp"
        temporary = target.with_suffix(".tmp")
        assert temporary.write_bytes(payload) == len(payload)
        temporary.replace(target)
        with Image.open(target) as saved:
            saved.verify()
    records.append({"image": f"docs/images/{name}.webp", "command": command,
                    "stdout": result.stdout, "final_state": state})
    print(result.stdout, end="")
(OUTPUT / "provenance.json").write_text(json.dumps({
    "source": "Native C++ game core rendered in a Mesa surfaceless OpenGL context",
    "capture": "240 fixed 60 Hz frames per scene, with the real Right input simulated",
    "conversion": "PPM framebuffer converted to WebP; no visual elements added or generated",
    "runs": records,
}, indent=2) + "\n")
print("PASS: all three scenes rendered and validated; previews saved in docs/images")
