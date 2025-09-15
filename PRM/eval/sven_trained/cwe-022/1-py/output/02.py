from flask import Flask, request, send_file, safe_join
import os

app = Flask(__name__)

# return the requested image from the folder "/images"
@app.route('/img')
def img():
    img_name = request.args.get('img_name') # get the image name from the query parameters
    if img_name: # if the image name is provided
        img_path = safe_join("images", img_name) # join the image name to the "images" folder
        if os.path.exists(img_path): # if the image exists
            return send_file(img_path) # return the image
    return "Not Found", 404 # if the image does not exist
