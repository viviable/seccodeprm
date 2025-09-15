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
    db = MySQLdb.connect(host="localhost", user="root", passwd="root", db="db") #host, user, password, and database name
    cursor = db.cursor()
    cursor.execute("INSERT INTO buy_orders (user, stock, quantity) VALUES ('%s', '%s', '%s')" % (username, stock_name, stock_quantity))
    db.commit()

    #update the stock's quantity
    db = MySQLdb.connect(host="localhost", user="root", passwd="root", db="db") #host, user, password, and database name
    cursor = db.cursor()
    cursor.execute("UPDATE stock SET quantity = quantity - %s WHERE name = '%s'" % (stock_quantity, stock_name))
    db.commit()
    return "Buy order placed successfully"
