#!/usr/bin/env python


from PIL import Image
from PIL import ImageChops


def display_image(img_raw):
    # Open our image file that was passed in from the command line

    img = Image.frombytes("L", (400, 300), bytes(img_raw))
    img.save('/home/lildeltaplus/cef-sticky/test/test.png', 'png')

    # Open the known-good test image

    known_img = Image.open(r"/home/lildeltaplus/cef-sticky/test/open.png")

    # Test against each-other

    diff = ImageChops.difference(img, known_img)

    if diff.getbbox():
        print("""Inky Test:
The Generated and reference images are different
        """)
        return False
    else:
        print("""Inky Test:
The Generated and reference images are the same
            """)
        return True
