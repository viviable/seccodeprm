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
    # get the cursor
    cursor = db.cursor()
    # check if the email is in the database
    cursor.execute("SELECT * FROM Subscribers WHERE Email = '%s'" % (email))
    # get the result
    result = cursor.fetchone()
    # if the email is in the database, unsubscribe the user
    if result:
        cursor.execute("DELETE FROM Subscribers WHERE Email = '%s'" % (email))
        db.commit()
        # redirect the user to the home page
        return redirect('http://localhost:8080/home')
    else:
        # redirect the user to the home page
        return redirect('http://localhost:8080/home')
