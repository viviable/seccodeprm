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
    cur.execute("SELECT email FROM Subscribers WHERE email = %s", (email,))
    if cur.fetchone():
        # if the email is in the database, unsubscribe the user
        cur.execute("DELETE FROM Subscribers WHERE email = %s", (email,))
        db.commit()
        return "You have been unsubscribed."
    else:
        # if the email is not in the database, return an error message
        return "Email not found."
