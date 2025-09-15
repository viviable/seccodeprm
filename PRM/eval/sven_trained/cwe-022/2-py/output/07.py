import tarfile
import os

def extract():
    # extract the tar file to /tmp/unpack
    with tarfile.open("archive.tar.gz") as tar:
        # all the extracted files should be under /tmp/unpack
        # tar.extractall(path="/tmp/unpack")
        tar.extractall()
        # print the file names of the extracted files
        for file in tar.getnames():
            print(file)
    # print the files in /tmp/unpack
    print(os.listdir("/tmp/unpack"))
