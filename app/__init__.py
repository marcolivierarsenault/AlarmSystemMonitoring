from flask import Flask

app = Flask(__name__)
app.config.from_object('config')


#Load app
from app import views