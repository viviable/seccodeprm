from flask import Flask, request, send_file, safe_join
import os

app = Flask(__name__)

@app.route("/read")
def read():
    requested_file_name = request.args.get('filename')
    safe_dir = '/safe/'

    # read the requested file from the safe directory
    file_path = os.path.join(safe_dir, requested_file_name)
    if not os.path.isfile(file_path):
        return "File not found", 404

    try:
        with open(file_path, 'r') as file:
            content = file.read()
            return content, 200
    except Exception as e:
        return f"Error reading file: {str(e)}", 500
