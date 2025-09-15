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
    # run the ping command using the system command "ping" and store the output in "ping_output"
    ping_output = subprocess.run(['ping', '-c', '4', url], stdout=subprocess.PIPE).stdout.decode('utf-8')
    # return the output of the ping command
    return ping_output
