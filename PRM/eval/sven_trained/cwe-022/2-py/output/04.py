import tarfile
import os

def extract():
    # extract the tar file to /tmp/unpack
    with tarfile.open("archive.tar.gz") as tar:
        # all the extracted files should be under /tmp/unpack
        tar.extractall("/tmp/unpack")

    # create /tmp/unpack/sub
    os.mkdir("/tmp/unpack/sub")

    # move the files in /tmp/unpack to /tmp/unpack/sub
    for file in os.listdir("/tmp/unpack"):
        os.rename("/tmp/unpack/" + file, "/tmp/unpack/sub/" + file)# extract the tar file to /tmp/unpack
