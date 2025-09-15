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
    # run the ping command using the system command "ping" and capture the output
    output = subprocess.run(['ping', '-c', '3', url], capture_output=True)
    # check the output of the ping command
    if output.returncode == 0:
        return 'Pong!'
    else:
        return 'Ping failed!'
