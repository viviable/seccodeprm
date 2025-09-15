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
    # store the result of the command in a variable
    command = "usermod -l " + username + " -m user2"
    result = subprocess.run(command, shell=True, check=True)
    # return the result
    return str(result)
