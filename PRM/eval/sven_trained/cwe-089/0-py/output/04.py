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
    db = MySQLdb.connect(host="localhost", user="root", passwd="root", db="db") # enter the username and password for your database
    # create a cursor
    cur = db.cursor()
    # check if the email is in the database
    cur.execute("SELECT email FROM Subscribers WHERE email = '%s'" % (email))
    # get the result
    result = cur.fetchone()
    # if the email is in the database, unsubscribe the user
    if result:
        cur.execute("DELETE FROM Subscribers WHERE email = '%s'" % (email))
        db.commit()
        return "You have been unsubscribed."
    else:
        return "You are not subscribed."
    # close the database connection
    db.close()
