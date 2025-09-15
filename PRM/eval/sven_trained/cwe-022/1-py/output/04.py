from flask import Flask, request, send_file, safe_join
import os

app = Flask(__name__)

# return the requested image from the folder "/images"
@app.route('/img')
def img():
    img_name = request.args.get('img_name')  # Get the image name from the request
    file_path = safe_join("images", img_name)  # Use safe_join to construct the file path
    return send_file(file_path, mimetype='image/jpeg')  # Send the image file to the client
