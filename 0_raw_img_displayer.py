import numpy as np
import matplotlib.pyplot as plt
import sys

print()
def display_image_from_file(img_infos,suffixe):
    # Assuming the image is 256x256 pixels and in grayscale
    
    file_path = img_infos[0]
    img_width=img_infos[1]
    img_height=img_infos[2]

    # Read the raw image data
    with open(file_path, 'rb') as file:
        raw_data = file.read()
    
    # Convert the raw data to a NumPy array and reshape it to the image dimensions
    img_array = np.frombuffer(raw_data, dtype=np.uint8).reshape((img_height, img_width))
    
    # Display the image
    plt.imshow(img_array, cmap='gray')
    plt.axis('off')  # To not display the axes for the image
    plt.imsave(f"images_readable/{file_path.split('/')[1].split('.')[0]}_{suffixe}.png", img_array, cmap='gray')
    #plt.show()
    
def update_saved_images(imgs,suffixe):
    for i in range(len(imgs)):
        display_image_from_file(imgs[i],suffixe)

imgs_in=[
    ['input/Amelia_256x256.raw',256,256],
    ['input/Angela_512x512.raw',512,512],
    ['input/Escher.raw',1024,1024],
    ['input/Ken_512x512.raw',512,512],
    ['input/kid.raw',1024,1024],
    ['input/parrots_512x256.raw',512,256],
    ['input/Quentin_512x512.raw',512,512]]





suffixe = sys.argv[1]
#update_saved_images(imgs=imgs_in,suffixe)
if suffixe == "V1":
    imgs_out_V1=[
        ['output/Amelia_256x256.raw',256,256],
        ['output/Angela_512x512.raw',512,512],
        ['output/Escher.raw',1024,1024],
        ['output/Ken_512x512.raw',512,512],
        ['output/kid.raw',1024,1024],
        ['output/parrots_512x256.raw',512,256],
        ['output/Quentin_512x512.raw',512,512]]
    update_saved_images(imgs=imgs_out_V1,suffixe=suffixe)

elif suffixe == "V2":
    imgs_out_V2=[
        ['output/Amelia_256x256_out_C.raw',256,256],
        ['output/Angela_512x512_out_C.raw',512,512],
        ['output/Escher_out_C.raw',1024,1024],
        ['output/Ken_512x512_out_C.raw',512,512],
        ['output/kid_out_C.raw',1024,1024],
        ['output/parrots_512x256_out_C.raw',512,256],
        ['output/Quentin_512x512_out_C.raw',512,512]]
    update_saved_images(imgs=imgs_out_V2,suffixe=suffixe)


