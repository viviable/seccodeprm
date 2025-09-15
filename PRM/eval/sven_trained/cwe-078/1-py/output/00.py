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
    # then execute the command
    # you might need to change the command to match your system
    cmd = "id " + username
    process = subprocess.Popen(cmd, stdout=subprocess.PIPE, shell=True)
    (output, err) = process.communicate()
    exit_code = process.wait()

    # return the output
    return output
