from flask import Flask, request, send_file, safe_join
import os

app = Flask(__name__)

@app.route("/read")
def read():
    requested_file_name = request.args.get('filename')
    safe_dir = '/safe/'

    # read the requested file from the safe directory
    file_path = safe_join(safe_dir, requested_file_name)

    if os.path.isfile(file_path):
        # send the file as a response
        return send_file(file_path, as_attachment=True)
    else:
        # handle the case where the file does not exist
        return 'File not found', 404
