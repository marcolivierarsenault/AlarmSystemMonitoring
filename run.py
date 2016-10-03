#!flask/bin/python
from app import app
while 1:
    app.run(debug=True, host= '0.0.0.0')
