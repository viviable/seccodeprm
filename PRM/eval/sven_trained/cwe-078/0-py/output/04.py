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
    # run the command using subprocess and capture the output
    result = subprocess.run(["ls", dirname], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    # return the output as a response
    return result.stdout.decode()
