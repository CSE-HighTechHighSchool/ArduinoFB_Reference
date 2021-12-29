from flask import Flask
from flask import request
from datetime import datetime
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

# Create database object ("db" represents the root node in the database)
db = firebase.database()

# Each data set will be stored under its own child node identified by a timestamp
# The timestamp for the current data set is taken when app.py is executed
timeStamp = datetime.now().strftime("%d-%m-%Y %H:%M:%S")

# Keys for key:value pairs will be integers (converted to strings for FB) 
# For each data set, keys will start from 0.  "key" variable will be incremented 
# in home() function.
key = 0

# Create server object
app = Flask(__name__)
@app.route("/")

def home():

  # Make variables "key" & "timestamp" accessible within function scope
  global key
  global timeStamp
    
  # Take parameters from Arduino request & assign value to variable "value"
  args = request.args
  value = str(args['distance'])

  #print("key, value: ", key, value)  # For debugging only

  # Update FB (don't use set() - will replace values instead of listing them)
  # The values for current data set are stored under the child node with the
  # current timestamp.
  # Keys should be strings for FB.  Values can be string or numerical datatype
  db.child(timeStamp).update({str(key):value})

  # Increment key
  key += 1 

  # Give Arduino a success response
  return "success"
    
# Run server on local IP address on port 5000 
if __name__ == "__main__":
    app.run(debug=False, host='****', port=5000)