import tarfile
import os

def extract():
    # extract the tar file to /tmp/unpack
    with tarfile.open("archive.tar.gz") as tar:
        # all the extracted files should be under /tmp/unpack
        tar.extractall("/tmp/unpack")
    # return the list of file names in the tar file
    return os.listdir("/tmp/unpack") # should return ['main.py', 'test.py', 'utils.py'] in this case
