from flask import Flask, request, send_file, safe_join
import os

app = Flask(__name__)

# return the requested image from the folder "/images"
@app.route('/img')
def img():
    img_name = request.args.get('img_name')  # get the name of the image from the request
    if img_name:  # if the name is not empty
        path = safe_join("/images", img_name)  # join the path to the image
        if os.path.isfile(path):  # if the file exists
            return send_file(path)  # send the file
        else:
            return "Image not found", 404  # return 404 if the file does not exist
    else:
        return "No image name provided", 400  # return 400 if the name is empty
