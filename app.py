# Install flask:  python -m pip install flask
from flask import Flask   
from flask import request

# Install Pyrebase
# For Win10:  python -m pip install pyrebase4
# For Mac:  python3 -m pip install pyrebase
import pyrebase

# Configuration credentials (can be found in Firebase console) - replace asterisks with your db info
config = {
  "apiKey": "****",
  "authDomain": "****-test.firebaseapp.com",
  "databaseURL": "****",
  "storageBucket": "****.appspot.com"
}

# Initialize firebase connection
firebase = pyrebase.initialize_app(config)

# Create database object
db = firebase.database()

# Server object
app = Flask(__name__)
@app.route("/")

def home():
    # Take parameters from Artuino request
    args = request.args

    #  Set the distance parameter in firebase to the collected ultrasonic distance from Arduino
    #  If using db.set, it's recommended that you use a timestamp for the key parameter 
    #  You can also use db.push to write data, which will generate a unique key for each value.
    db.set({"distance":str(args['distance'])})

    # Give Arduino a success response
    return "success"
    
# Run server on local IP address on port 5000
if __name__ == "__main__":
    app.run(debug=False, host='****', port=5000)