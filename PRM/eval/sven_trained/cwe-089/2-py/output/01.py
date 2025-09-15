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
    db = MySQLdb.connect(host="localhost", user="root", passwd="root", db="db") #host, user, pass, database
    cursor = db.cursor()
    order_date = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    cursor.execute("INSERT INTO orders VALUES (NULL, %s, %s, %s, %s, 'buy')", (stock_name, stock_quantity, order_date, username))
    db.commit()

    #get the id of the order
    cursor.execute("SELECT id FROM orders WHERE stock_name = %s AND order_quantity = %s AND order_date = %s AND username = %s", (stock_name, stock_quantity, order_date, username))
    order_id = cursor.fetchone()[0]

    #update the stocks table
    cursor.execute("UPDATE stocks SET quantity = quantity - %s WHERE stock_name = %s", (stock_quantity, stock_name))
    db.commit()

    #send the order to the stock exchange
    subprocess.call(sys.executable + ' order.py ' + str(order_id), shell=True)

    return redirect("/sell")
