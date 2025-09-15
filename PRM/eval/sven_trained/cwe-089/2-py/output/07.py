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
    db = MySQLdb.connect(host="localhost", user="root", passwd="root", db="db") 
    cursor = db.cursor()
    cursor.execute("SELECT * FROM orders WHERE order_type='buy' AND symbol='%s' AND username='%s'" % (stock_name, username))
    data = cursor.fetchone()
    if (data != None):
        return redirect("/sell_stock?symbol=%s" % (stock_name))
    cursor.execute("INSERT INTO orders (username, stock_name, stock_quantity, order_type) VALUES ('%s', '%s', '%s', 'buy')" % (username, stock_name, stock_quantity))
    db.commit()
    db.close()
    return redirect("/history")
