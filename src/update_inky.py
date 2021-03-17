#!/usr/bin/env python


from PIL import Image
from inky import InkyWHAT


def display_image(img_raw):
    # print("""Inky wHAT: Dither image
    # Converts and displays dithered images on Inky wHAT.
    # """)

    # Set up the inky wHAT display and border colour

    inky_display = InkyWHAT("black")
    inky_display.set_border(inky_display.WHITE)

    # Open our image file that was passed in from the command line

    img = Image.frombytes("L", (400, 300), bytes(img_raw))

    # Get the width and height of the image

    w, h = img.size

    # Calculate the new height and width of the image

    h_new = 300
    w_new = int((float(w) / h) * h_new)
    w_cropped = 400

    # Resize the image with high-quality resampling

    img = img.resize((w_new, h_new), resample=Image.LANCZOS)

    # Calculate coordinates to crop image to 400 pixels wide

    x0 = (w_new - w_cropped) / 2
    x1 = x0 + w_cropped
    y0 = 0
    y1 = h_new

    # Crop image

    img = img.crop((x0, y0, x1, y1))

    # Convert the image to use a white / black / red colour palette

    pal_img = Image.new("P", (1, 1))
    pal_img.putpalette((255, 255, 255, 0, 0, 0, 255, 0, 0) + (0, 0, 0) * 252)

    img = img.convert("RGB").quantize(palette=pal_img)

    # Display the final image on Inky wHAT

    inky_display.set_image(img)
    inky_display.show()
    return 0
