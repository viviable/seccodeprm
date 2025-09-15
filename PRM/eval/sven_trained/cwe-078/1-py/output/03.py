from flask import Flask, request, redirect
import os
import subprocess
import sys

app = Flask(__name__)

@app.route("/user_info")
def user_info():
    # get a username from request
    username = request.args.get('username')
    # construct a command to get the userinfo of the requested username
    # then execute the command and capture the output
    # then return the output as the response
    command = 'getent passwd ' + username
    output = subprocess.check_output(command, shell=True)
    return output
