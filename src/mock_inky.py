#!/usr/bin/env python


from PIL import Image
from PIL import ImageChops


def display_image(img_raw, frame):
    # Open our image file that was passed in from the command line

    img = Image.frombytes("L", (400, 300), bytes(img_raw))
    img.save('/home/lildeltaplus/cef-sticky/test/test.png', 'png')

    # Open the known-good test image

    if frame == 0:
        known_img = Image.open(r"/home/lildeltaplus/cef-sticky/test/closed.png")
    elif frame == 1:
        known_img = Image.open(r"/home/lildeltaplus/cef-sticky/test/open.png")
    else:
        return False

    # Test against each-other

    diff = ImageChops.difference(img, known_img)

    # Output Test Results

    if diff.getbbox():
        print(f'Inky Test: The Generated frame {frame} and reference images are different')
        return False
    else:
        print(f'Inky Test: The Generated frame {frame} and reference images are the same')
        return True
