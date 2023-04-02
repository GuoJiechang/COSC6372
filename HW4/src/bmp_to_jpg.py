from PIL import Image
import glob
for filename in glob.glob('./*.bmp'): 
    img=Image.open(filename)
    new_img = img.resize(img.size)
    new_img.save(filename+'.png', 'png')

