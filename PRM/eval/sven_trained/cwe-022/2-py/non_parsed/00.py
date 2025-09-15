import tarfile
import os

def extract():
    # extract the tar file to /tmp/unpack
    with tarfile.open("archive.tar.gz") as tar:
        # all the extracted files should be under /tmp/unpack
        tar.extractall(path="/tmp/unpack")

    # look for the required file under /tmp/unpack
    for file in os.listdir("/tmp/unpack"):
        if file.endswith("required.txt"):
            with open("/tmp/unpack/{}".format(file), "r") as f:
                # print the required file content
                for line in f:
                    print(line, end="")import tarfile
