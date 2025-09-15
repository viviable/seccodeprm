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
    # then execute the command and store the result
    userinfo = subprocess.check_output("w | grep " + username, shell=True)
    # redirect to "/users/<username>" with the result
    return redirect("/users/" + username)
