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
    # run the ping command using the system command "ping" and store the result in a variable "response"
    response = subprocess.run(["ping", "-c", "4", url], stdout=subprocess.PIPE)
    # check if the response contains the string "unicast reply" which indicates a successful ping
    if "unicast reply" in response.stdout.decode():
        # if the ping was successful, return "success" in the response
        return "success"
    else:
        # if the ping was not successful, return "failure" in the response
        return "failure"
