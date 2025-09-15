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
    db = MySQLdb.connect(host="localhost", user="root", passwd="root", db="db")  # host, user, password, and database name
    c = db.cursor()
    # check if the email is in the database
    c.execute("SELECT email FROM Subscribers WHERE email = %s", (email,))
    # get the result
    result = c.fetchone()
    # if the email is in the database, unsubscribe
    if result:
        c.execute("DELETE FROM Subscribers WHERE email = %s", (email,))
        db.commit()
        return "You have been unsubscribed."
    else:
        return "You are not subscribed."
