from flask import Flask, request, jsonify
import subprocess
import base64

app = Flask(__name__)

@app.route('/update', methods=['POST'])
def update():

    data = request.json
    # filename = data['filename']
    filedata = data['filedata']

    decoded_data = base64.b64decode(filedata).decode('utf-8')

    packages = decoded_data.splitlines()

    for package in packages:
        subprocess.run(["sudo", "apt-get", "install", package])

    # Sauvegarde pour rollback
    with open("rollbackFile", 'w') as f:
        for package in packages:
            result = subprocess.run(["dpkg-query", "-W", "-f=${Version}", package], capture_output=True, text=True)
            version = result.stdout.strip() if result.returncode == 0 else "unknown"
            f.write(f"{package}={version}\n")
        
    return jsonify({"status": "Update completed"}), 200


@app.route('/rollback', methods=['GET'])
def rollback():

    with open("rollbackFile", 'r') as f:
        packages = f.read().splitlines()

    for package in packages:
        package_name, version = package.split('=')
        subprocess.run(["sudo", "apt-get", "install", f"{package_name}={version}"])
    
    print(f"rollbacked:\n{packages}")
    return jsonify({"status": "Rollback completed"}), 200
    
@app.route('/ping', methods=['GET'])
def ping():
    return jsonify({"status": "alive", "device": "raspberry-pi"}), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
