"""Encode native frames without a small shared palette that can discard hues."""
from PIL import Image


def encode_animation(images, output, poster=None):
    # Keep common flat colors exact, then cover the remaining color range.
    # This preserves dark backgrounds as well as less common neon greens.
    frames = []
    for image in images:
        dominant = sorted(image.getcolors(maxcolors=image.width * image.height),
                          reverse=True)[:64]
        diverse = image.quantize(colors=192, method=Image.Quantize.MAXCOVERAGE)
        colors = diverse.getpalette()[:192 * 3]
        colors += [channel for count, color in dominant for channel in color]
        colors += [0] * (768 - len(colors))
        palette = Image.new("P", (1, 1))
        palette.putpalette(colors)
        quantized = image.quantize(palette=palette, dither=Image.Dither.NONE)
        # Pillow's approximate palette lookup can alter even exact matches.
        # Restore the dominant source colors directly, without recoloring them.
        exact_indices = {color: 192 + i for i, (count, color) in enumerate(dominant)}
        indices = bytes(exact_indices.get(color, index) for color, index in
                        zip(image.getdata(), quantized.getdata()))
        frame = Image.frombytes("P", image.size, indices)
        frame.putpalette(colors)
        frames.append(frame)
    frames[0].save(output, save_all=True, append_images=frames[1:],
                   duration=100, loop=0, optimize=True, disposal=1)
    with Image.open(output) as gif:
        assert gif.size == images[0].size
        assert 25 <= gif.n_frames <= len(images)
        encoded_frames = gif.n_frames
        duration = 0
        for i in range(gif.n_frames):
            gif.seek(i)
            duration += gif.info["duration"]
        assert duration == 5000 and gif.info["loop"] == 0
        if poster:
            # An exact copy of the first decoded GIF frame avoids a color jump
            # when the home card switches from its poster to the animation.
            gif.seek(0)
            gif.convert("RGB").save(poster, lossless=True)
    return {"gif_frames": encoded_frames, "duration_ms": duration,
            "encoding": {"palette": "per-frame", "colors": 256,
                         "method": "exact dominant colors and maximum color coverage",
                         "color_adjustments": False}}
