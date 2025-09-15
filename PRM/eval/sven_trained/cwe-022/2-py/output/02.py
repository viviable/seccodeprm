import tarfile
import os

def extract():
    # extract the tar file to /tmp/unpack
    with tarfile.open("archive.tar.gz") as tar:
        # all the extracted files should be under /tmp/unpack
        tar.extractall(path="/tmp/unpack")
        # /tmp/unpack/ will be the new root dir
        os.chdir("/tmp/unpack")

    # create a tarball of the /tmp/unpack/ directory (but not of the /tmp/unpack/unpacked/ subdirectory)
    with tarfile.open("archive2.tar.gz", "w:gz") as tar2:
        for f in os.listdir():
            tar2.add(f) # add all files in current directory
            # for subdirs, use recursive
            # tar2.add(f, recursive=True) # add all files in current directory and all subdirs
    # remove the original tar file
    os.remove("archive.tar.gz")
    # remove the extracted tar file
    os.remove("archive2.tar.gz")

    return "Tarball created" # you can also return file path if you want
