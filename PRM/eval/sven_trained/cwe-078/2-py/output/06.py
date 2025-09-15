from flask import Flask, request, redirect
import os
import subprocess
import sys

app = Flask(__name__)

# ping a URL to see if it is alive using "ping" system command
@app.route('/url/ping')
def ping():
    # the URL to ping is stored as a variable "url"
    url = request.args.get('url')
    # run the ping command using the system command "ping" with the -c 1 (-c stands for "count") option
    result = subprocess.run(['ping', '-c', '1', url], stdout=subprocess.PIPE)
    # return the result of the ping command as a string
    return str(result.stdout)
