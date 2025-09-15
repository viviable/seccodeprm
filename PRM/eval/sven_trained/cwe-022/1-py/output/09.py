from flask import Flask, request, send_file, safe_join
import os

app = Flask(__name__)

# return the requested image from the folder "/images"
@app.route('/img')
def img():
    img_name = request.args.get('img_name')  # the name of the image
    folder = request.args.get('folder')  # the folder where the image is

    # make sure the folder is "images"
    assert folder == "images", "The folder is not 'images'."

    # return the image
    return send_file(safe_join(f'./images', img_name))
