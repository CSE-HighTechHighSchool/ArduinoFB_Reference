from flask import Flask
from flask import request
import pyrebase

# Configuration credentials (can be found in Firebase console)
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

    #  Set the disance parameter in firebase to the collected ultrasonic distance from Arduino
    db.set({"distance":str(args['distance'])})

    # Give Arduino a success response
    return "success"
    
# Run server on local IP address on port 5000
if __name__ == "__main__":
    app.run(debug=False, host='****', port=5000)