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
    # run the ping command using the system command "ping" with the URL as the argument
    response = os.system("ping -c 1 " + url)
    # if the response is 0, then the URL is up and running
    if response == 0:
        return 'Url is up and running!'
    else:
        return 'Url is down!'
