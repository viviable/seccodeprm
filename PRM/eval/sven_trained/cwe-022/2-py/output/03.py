import tarfile
import os

def extract():
    # extract the tar file to /tmp/unpack
    with tarfile.open("archive.tar.gz") as tar:
        # all the extracted files should be under /tmp/unpack
        tar.extractall(path="/tmp/unpack")
    # return the path to the extracted files
    return "/tmp/unpack"
