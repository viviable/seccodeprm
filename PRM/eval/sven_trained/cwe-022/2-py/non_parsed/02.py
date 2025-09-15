import tarfile
import os

def extract():
    # extract the tar file to /tmp/unpack
    with tarfile.open("archive.tar.gz") as tar:
        # all the extracted files should be under /tmp/unpack
        # if /tmp/unpack doesn't exists, create it
        if not os.path.exists("/tmp/unpack"):
            os.makedirs("/tmp/unpack")
        tar.extractall(path="/tmp/unpack")
        tar.close()import tarfile
