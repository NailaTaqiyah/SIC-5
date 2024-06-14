import json
from flask import Flask, request, jsonify
from datetime import datetime

app = Flask(__name__)

sound_DATA = []

@app.route("/")
def root_route():
    return "Hello world!"

@app.route("/sound")
def get_sound():
    return jsonify(sound_DATA)

@app.route("/submit-sound", methods=["POST"])
def post_sound():
    pesan = request.data.decode("utf8")
    timestamp = datetime.now().isoformat()
    sound_DATA.append({"sound": float(pesan), "timestamp": timestamp})
    print(f"Received sound: {pesan} at {timestamp}")
    return f"Received sound {pesan} at {timestamp}"

@app.route("/submit", methods=["POST"])
def post_data():
    pesan = request.data.decode("utf8")
    pesan = json.loads(pesan)
    timestamp = datetime.now().isoformat()
    sound_DATA.append({"sound": float(pesan["sound"]), "timestamp": timestamp})
    print(f"Received data: {pesan} at {timestamp}")
    return f"Received data {pesan} at {timestamp}"

if __name__ == "__main__":
    app.run(debug=True, host='0.0.0.0')
