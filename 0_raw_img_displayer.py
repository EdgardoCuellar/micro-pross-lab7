import numpy as np
import matplotlib.pyplot as plt
import os



def display_image_from_file(img_infos):
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
    tmp = file_path.split('/')[1].split('.')

    # Check if the file exists
    if not os.path.exists(file_path):
        # If the file does not exist, create it
        with open(file_path, 'w') as file:
            # File is created;
            pass
    
    plt.imsave(f"images_readable/{tmp[0]}_{tmp[1]}.png", img_array, cmap='gray')
    #plt.show()
    
def update_saved_images(imgs):
    for i in range(len(imgs)):
        display_image_from_file(imgs[i])

def check_names(directory_path = "output"):

    # List to hold file names
    file_names = []

    # Loop through the directory
    for filename in os.listdir(directory_path):
        # Add each file name to the list
        file_names.append(filename)

    # Print the list of file names
    print(file_names)
    return file_names


imgs_in=[
    ['input/Amelia_256x256.raw',256,256],
    ['input/Angela_512x512.raw',512,512],
    ['input/Escher.raw',1024,1024],
    ['input/Ken_512x512.raw',512,512],
    ['input/kid.raw',1024,1024],
    ['input/parrots_512x256.raw',512,256],
    ['input/Quentin_512x512.raw',512,512]]

imgs_out=[
    ['output/Amelia_256x256.raw_out_C.raw',256,256],
    ['output/Angela_512x512.raw_out_C.raw',512,512],
    ['output/Escher.raw_out_C.raw',1024,1024],
    ['output/Ken_512x512.raw_out_C.raw',512,512],
    ['output/kid.raw_out_C.raw',1024,1024],
    ['output/parrots_512x256.raw_out_C.raw',512,256],
    ['output/Quentin_512x512.raw_out_C.raw',512,512],

    ['output/Amelia_256x256.raw_out_SIMD.raw',256,256],
    ['output/Angela_512x512.raw_out_SIMD.raw',512,512],
    ['output/Escher.raw_out_SIMD.raw',1024,1024],
    ['output/Ken_512x512.raw_out_SIMD.raw',512,512],
    ['output/kid.raw_out_SIMD.raw',1024,1024],
    ['output/parrots_512x256.raw_out_SIMD.raw',512,256],
    ['output/Quentin_512x512.raw_out_SIMD.raw',512,512]]
update_saved_images(imgs=imgs_in)
update_saved_images(imgs=imgs_out)


