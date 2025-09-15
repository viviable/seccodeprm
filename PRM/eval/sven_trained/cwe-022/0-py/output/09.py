from flask import Flask, request, send_file, safe_join
import os

app = Flask(__name__)

@app.route("/read")
def read():
    requested_file_name = request.args.get('filename')
    safe_dir = '/safe/'

    # read the requested file from the safe directory
    requested_file = open(safe_dir + requested_file_name, 'rb')
    file_data = requested_file.read()
    requested_file.close()
    return file_data
