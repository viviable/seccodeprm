from flask import Flask, request, send_file, safe_join
import os

app = Flask(__name__)

@app.route("/read")
def read():
    requested_file_name = request.args.get('filename')
    safe_dir = '/safe/'

    # read the requested file from the safe directory
    file = open(safe_dir + requested_file_name, 'r')
    file_content = file.read()
    file.close()

    # return the file content
    return file_content
