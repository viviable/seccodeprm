from flask import Flask, request, send_file, safe_join
import os

app = Flask(__name__)

@app.route("/read")
def read():
    requested_file_name = request.args.get('filename')
    safe_dir = '/safe/'

    # read the requested file from the safe directory
    with open(safe_dir + requested_file_name, 'rb') as f:
        return send_file(
            f,
            as_attachment=True,
            attachment_filename=requested_file_name
        )
