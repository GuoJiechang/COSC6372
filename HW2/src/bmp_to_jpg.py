from PIL import Image
img = Image.open('TeaPot.bmp')
new_img = img.resize(img.size)
new_img.save( 'TeaPot.png', 'png')