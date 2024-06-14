from flask import Flask, request, jsonify

app = Flask(__name__)


@app.route("/data", methods=["POST"])
def receive_data():
    data = request.get_json()
    temperature = data.get("temperature")
    humidity = data.get("humidity")

    print(f"Received data - Temperature: {temperature}, Humidity: {humidity}")

    response = {"status": "success", "message": "Data received", "received_data": data}
    return jsonify(response), 200


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
