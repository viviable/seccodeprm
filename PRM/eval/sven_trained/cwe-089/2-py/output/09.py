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
    db = MySQLdb.connect(host="localhost", user="root", passwd="root", db="db") #host, user, passwd, db
    cursor = db.cursor()
    try:
        cursor.execute("""INSERT INTO Orders (username, stock_name, stock_quantity, order_type) VALUES (%s, %s, %s, 'buy')""", (username, stock_name, stock_quantity))
        db.commit()
    except:
        return "Error in insert operation"
    db.close()
    
    return redirect("/sell")
