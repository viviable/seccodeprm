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
    db = MySQLdb.connect(host="localhost", user="root", passwd="root", db="db")  #connect to the database
    cursor = db.cursor()
    try:
        cursor.execute("INSERT INTO buy_orders (user_id, stock_name, stock_quantity, order_time) VALUES ((SELECT user_id FROM users WHERE username = %s),%s,%s,now())", (username, stock_name, stock_quantity))
        db.commit()
        db.close()
    except:
        return "Error in insert operation"

    return redirect("/")
