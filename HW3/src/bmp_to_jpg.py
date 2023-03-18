from PIL import Image
for i in range(1,7):
    img = Image.open('TeaPot'+str(i)+'.bmp')
    new_img = img.resize(img.size)
    new_img.save( 'TeaPot'+str(i)+'_1.png', 'png')