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
    # the returned value is a byte string
    command = 'getent passwd ' + username
    userinfo = subprocess.check_output(command, shell=True)
    # convert the byte string to a list of lines
    # each line is a string
    lines = userinfo.decode('utf-8').split('\n')
    # the first line is the one we are interested in
    # the first element in the line is the username
    # the second element is the homedir
    # the third element is the shell
    userinfo = lines[0].split(':')
    # the homedir and shell are optional, so if they don't exist, return / and /bin/sh respectively
    if len(userinfo) < 3:
        homedir = '/'
        shell = '/bin/sh'
    else:
        homedir = userinfo[2]
        shell = userinfo[3]
    # return the information in a json format
    return {'username': username, 'homedir': homedir, 'shell': shell}
