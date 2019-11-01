# PhotoShop
Simple PhotoShop tools for process images in ppm format. This project processes images by manipulating every individual pixels and their corresponding RGB values. The core algorithm is implemented in C. Tested with many different images that support the ppm file format. 
There is an interactive command prompt with detailed instructions for the PhotoShop options, also listed below.

## Commands and Options
        r <filename> - read image from <filename>
        w <filename> - write image to <filename>
        s - swap color channels
        br <amt> - change brightness (up or down) by the given amount
        c <x1> <y1> <x2> <y2> - crop image to the box with the given corners
        g - convert to grayscale
        cn <amt> - change contrast (up or down) by the given amount
        bl <sigma> - Gaussian blur with the given radius (sigma)
        sh <sigma> <amt> - sharpen by given amount (intensity), with radius (sigma)
        e <sigma> <threshold> - detect edges with intensity gradient above given threshold
        q - quit
