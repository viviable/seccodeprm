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
    # run the ping command using the system command "ping" and store the results in a variable "response"
    response = subprocess.run(['ping', '-c', '1', url], stdout=subprocess.PIPE)
    # return the response code and the output of the command as a string
    return str(response.returncode) + ' ' + response.stdout.decode('utf-8')
