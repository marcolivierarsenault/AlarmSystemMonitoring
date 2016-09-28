from flask import render_template, request, abort, flash, g, redirect, url_for,jsonify
from app import app
from config import *
import requests
#Default Loader for Views, This class allows the interpretation of the different URL

@app.route('/')
@app.route('/index')
def index():
    url = 'https://api.particle.io/v1/devices/'+PARTICLE_DEVICE_ID+'/ArmedStatus'
    data = dict(arg='joe', access_token=PARTICLE_DEVICE_TOKEN)
    r = requests.post(url, data=data, allow_redirects=True)
    if r.json()['return_value'] == 2:
        return render_template('unarmed.html')
    else:
        return render_template('armed.html')