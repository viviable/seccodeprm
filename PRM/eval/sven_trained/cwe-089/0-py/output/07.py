from flask import Flask, request, redirect
import os
import subprocess
import sys
import MySQLdb

app = Flask(__name__)

# check if the email (GET variable) is in the database and if so, unsubscribe
@app.route('/unsubscribe')
def unsubscribe():
    # get the email (GET variable)
    email = request.args.get('email')
    # get the database connection
    db = MySQLdb.connect(host="localhost", user="root", passwd="root", db="db") 
    cur = db.cursor()
    # check if the email is in the database
    cur.execute("SELECT * FROM Subscribers WHERE Email = '%s'" % (email))
    # if the email is in the database, unsubscribe
    if cur.fetchone():
        cur.execute("DELETE FROM Subscribers WHERE Email = '%s'" % (email))
        db.commit()
        return "You have been unsubscribed."
    else:
        return "You are not subscribed."
