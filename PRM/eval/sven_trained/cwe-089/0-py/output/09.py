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
    db = MySQLdb.connect(host="localhost", user="root", passwd="root", db="db") # you may need to change the password and the database name
    cur = db.cursor()
    # check if the email is in the database
    cur.execute("SELECT * FROM Subscribers WHERE email=%s", (email,))
    # if the email is in the database, unsubscribe the user
    if cur.fetchone():
        cur.execute("DELETE FROM Subscribers WHERE email=%s", (email,))
        db.commit()
        return "You have been unsubscribed."
    else:
        return "Email not found in database."
