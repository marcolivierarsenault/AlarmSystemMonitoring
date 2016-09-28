from flask import render_template, request, abort, flash, g, redirect, url_for
from app import app
#Default Loader for Views, This class allows the interpretation of the different URL

@app.route('/')
@app.route('/index')
def index():
    return render_template('index.html')

