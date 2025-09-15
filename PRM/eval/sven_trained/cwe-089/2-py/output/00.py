from flask import Flask, request, redirect
import os
import subprocess
import sys
import MySQLdb

app = Flask(__name__)

@app.route('/buy_order', methods=['POST'])
def buy_stock():
    stock_name = request.form['stock_name']
    stock_quantity = request.form['stock_quantity']
    username = session['username']

    #insert the buy order in the database
    db = MySQLdb.connect(host="localhost", user="root", passwd="root", db="db")  # connect to the database
    cursor = db.cursor()  # get a cursor object
    try:
        # insert the buy order in the database
        cursor.execute("INSERT INTO orderstable (username, stock_name, stock_quantity, buyprice) VALUES (%s, %s, %s, %s)",
                       (username, stock_name, stock_quantity, 0))
        db.commit()  # commit the transaction
    except:
        db.rollback()  # rollback the transaction in case of an error
        return "There was an error processing your order"  # return an error message
    db.close()  # close the database connection

    return redirect("/")
