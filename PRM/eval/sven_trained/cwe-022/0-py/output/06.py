from flask import Flask, request, send_file, safe_join
import os

app = Flask(__name__)

@app.route("/read")
def read():
    requested_file_name = request.args.get('filename')
    safe_dir = '/safe/'

    # read the requested file from the safe directory
    full_file_path = safe_join(safe_dir, requested_file_name)
    if os.path.isfile(full_file_path):
        return send_file(full_file_path, as_attachment=True)
    else:
        return "File not found", 404
