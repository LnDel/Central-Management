from flask import Flask, request, jsonify
import subprocess

app = Flask(__name__)

@app.route('/update', methods=['POST'])
def update():
    data = request.json
    filename = data['filename']
    filesize = data['filesize']


    #subprocess.run(["wget", "http://server_ip/path_to_update_file/" + filename])
    subprocess.run(["./update_script.sh", filename])

    return jsonify({"status": "Update initiated"}), 200

@app.route('/rollback', methods=['GET'])
def rollback():

    subprocess.run(["./rollback_script.sh"])

    return jsonify({"status": "Rollback initiated"}), 200
    
@app.route('/ping', methods=['GET'])
def ping():
    return jsonify({"status": "alive", "device": "raspberry-pi"}), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
