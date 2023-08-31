# Preview

Preview is a program for viewing and editing multiple file types.

Traditionally, there has not been an easy way to view and make quick adjustments to palette, graphics, and patch files, but this program aims to fix that. It can open Text, Code, Images, Palettes (.bin, .pal and .tpl currently, with more to come), graphics (raw graphics files in the form of .gfx or .bin) and patches (coming soon!), and even edit most of them.
<img width="1192" alt="Screenshot 2023-08-31 at 2 43 22 PM" src="https://github.com/Quote58/Preview/assets/15618772/b5457dd8-8764-41ad-a048-72db77921806">

## Text
Preview can view code with syntax highlighting and code folding, to make even small edits comfortable. Currently it supports highlighting for Python, C/C++ (and any other C based syntax), and Assembly.
<img width="1192" alt="Screenshot 2023-08-31 at 2 43 46 PM" src="https://github.com/Quote58/Preview/assets/15618772/e60fd87c-6bc3-450f-ab9b-8b6513e09d1f">

## Palettes
Preview can not only view palettes, but fully edit them. You can do this with both a spinner control to adjust the exact hexidecimal value of each component (and in the case of NES palettes, the index), and with a colour picker control which uses the native system colour picker. This lets you choose any colour comfortably, and Preview will handle figuring out things like bitdepth and conversions.
<img width="1192" alt="Screenshot 2023-08-31 at 2 44 04 PM" src="https://github.com/Quote58/Preview/assets/15618772/61b561be-d967-4613-97ae-7e691e9262f3">

You can even use the colour picker to grab a colour from anywhere with the eye dropper tool! And all of this even extends to NES palettes (through .tpl) which can still use the colour picker and will find the closest matching colour in the NES palette to the one chosen by the user.
<img width="1078" alt="Screenshot 2023-08-31 at 2 44 33 PM" src="https://github.com/Quote58/Preview/assets/15618772/7367805d-b6a3-4778-8b4b-f191313b45f2">

## Graphics
Just like with Palettes, Preview can view and edit graphics with wide support for 2d graphics types. This includes direct RGB, planar (and composite), and linear (and reversed linear), at any bitdepth from 1 to 8, using any palette with any colour bitdepth. As for what the built in graphics editor can do, it has the features you would expect (dedicated zoomed in view of the tile, clicking to place a pixel from the palette, dragging to draw, tile mirroring, etc.), but it also has advanced features to make quick edits even easier. This includes the viewer dynamically changing to show multiple tiles at once (up to 8x8) based on what the user selects on the grid, allowing the user to grab entire groups of tiles and edit them contiguously, and even rotation by arbitrary degrees!
<img width="1192" alt="Screenshot 2023-08-31 at 2 46 38 PM" src="https://github.com/Quote58/Preview/assets/15618772/11f4e76e-8b59-4a08-91b9-88b9e3b6488f">
<img width="1192" alt="Screenshot 2023-08-31 at 2 47 07 PM" src="https://github.com/Quote58/Preview/assets/15618772/e73f14f1-3c78-48c1-90ff-a2a88b917abb">

## Images
Preview has support for many different image types, including the expected .png, .bmp, .jpg, etc. but it even supports .svg! It can also zoom with the scroll wheel.
<img width="1192" alt="Screenshot 2023-08-31 at 2 47 36 PM" src="https://github.com/Quote58/Preview/assets/15618772/a302cd8c-d361-4770-9de0-0fd014f7fd30">
