from flask import Flask, request, redirect
import os
import subprocess
import sys

app = Flask(__name__)

@app.route("/ls")
def ls():
    # get a directory name from request
    dirname = request.args.get('dirname')
    # construct a "ls" command to list the contents of "dirname"
    # run the command, and capture its output
    cmd = ['ls', dirname]
    result = subprocess.run(cmd, stdout=subprocess.PIPE)
    # return the output
    return result.stdout.decode('utf-8')
