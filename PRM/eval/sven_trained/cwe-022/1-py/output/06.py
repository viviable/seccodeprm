from flask import Flask, request, send_file, safe_join
import os

app = Flask(__name__)

# return the requested image from the folder "/images"
@app.route('/img')
def img():
    img_name = request.args.get('img_name') # get the name of the image to be returned
    image_path = os.path.join(os.path.dirname(__file__), "images", img_name) # get the full path of the image
    return send_file(image_path, mimetype='image/jpeg') # return the image
