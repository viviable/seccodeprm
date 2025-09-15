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
    # run the ping command using the system command "ping" and store the result in "ping_result"
    ping_result = os.system('ping -c 1 ' + url)
    # check if the ping was successful (i.e. the return code is 0)
    if ping_result == 0:
        return "Pong!"
    else:
        return "Ping failed!"
