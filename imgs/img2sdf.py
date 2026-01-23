#!/usr/bin/env python3
from PIL import Image
from pathlib import Path
import math
from argparse import ArgumentParser

argParser = ArgumentParser()
argParser.add_argument("image")

args = argParser.parse_args()
imgFile = Path(args.image)

if not imgFile.exists():
    print(f"The specified file does not exist.")
    exit(1)

im = Image.open(imgFile)
loaded = im.load()
if not loaded:
    print("Failed to load image.")
    exit()
width, height = im.size

pixels = [[] for _ in range(width)]
for x in range(width):
    for y in range(height):
        pixels[x].insert(y, loaded[x, y] == 255 or loaded[x, y] == (255, 255, 255))

print(f"Converting the following {width}x{height} bitmap to a distance field:")
for y in range(height):
    for x in range(width):
        print("O" if pixels[x][y] else "_", end="")
    print()

# Someone please tell me if there is a better way to do this.

distanceField = [[] for _ in range(width)]

def calcDistanceField(pixelX: int, pixelY: int) -> float:
    minDistance = math.inf
    for x in range(width):
        for y in range(height):
            if x == pixelX and y == pixelY:
                continue

            distance = math.sqrt((pixelX - x)**2 + (pixelY - y)**2)
            if pixels[x][y] and not pixels[pixelX][pixelY] and distance < minDistance:
                minDistance = distance
            elif not pixels[x][y] and pixels[pixelX][pixelY] and distance < minDistance:
                minDistance = distance

    if pixels[pixelX][pixelY]:
        # Account for screen edge as falsey pixels too
        if pixelX == 0 or pixelY == 0 or pixelX == width - 1 or pixelY == height - 1:
            minDistance = 0
        else:
            minDistance = -minDistance + 1

    return minDistance

for x in range(width):
    for y in range(height):
        distanceField[x].insert(y, calcDistanceField(x, y))

print("\nC++ Output: \n")

for y in range(height):
    for x in range(width):
        print(str(distanceField[x][y]), end=", ")
    print()