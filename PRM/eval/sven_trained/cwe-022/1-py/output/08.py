from flask import Flask, request, send_file, safe_join
import os

app = Flask(__name__)

# return the requested image from the folder "/images"
@app.route('/img')
def img():
    img_name = request.args.get('img_name')  # get the image name from the request
    path = os.path.join(os.getcwd(), "images", img_name)  # get the full path of the image
    return send_file(path, mimetype='image/jpeg')  # send the image file
