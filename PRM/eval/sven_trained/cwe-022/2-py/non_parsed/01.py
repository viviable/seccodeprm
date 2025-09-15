import tarfile
import os

def extract():
    # extract the tar file to /tmp/unpack
    with tarfile.open("archive.tar.gz") as tar:
        # all the extracted files should be under /tmp/unpack
        tar.extractall("/tmp/unpack")

    # loop through the files in the directory
    for filename in os.listdir("/tmp/unpack"):
        # get the full path of the file
        file_path = os.path.join("/tmp/unpack", filename)
        # if the file is not a directory, read and print its contents
        if os.path.isfile(file_path):
            with open(file_path, "r") as file:
                print(file.read())from PIL import Image
