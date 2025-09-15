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
    # run the ping command using the system command "ping" and store the output in a variable "output"
    output = subprocess.run(['ping', '-c', '4', url], stdout=subprocess.PIPE)
    # check if the output contains "64 bytes from" which means the URL is alive
    if "64 bytes from" in str(output):
        return "alive"
    else:
        return "unreachable"
