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
    # run the ping command using the system command "ping" and store the results in "ping_output"
    ping_output = subprocess.run(['ping', url], capture_output=True, text=True)
    # return the results of the ping command as a string
    return ping_output.stdout
