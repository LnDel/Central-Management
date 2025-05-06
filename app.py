##
# @file app.py
# @brief API Flask pour la gestion des mises à jour et des rollbacks sur Raspberry Pi.
#
# Ce service expose trois routes :
# - `/update` (POST) : pour installer une liste de paquets.
# - `/rollback` (GET) : pour restaurer les paquets à des versions précédentes.
# - `/ping` (GET) : pour tester la disponibilité de l'appareil.
#
# Utilisé avec une interface Qt pour la gestion centralisée.
# Le fichier "rollbackFile" est utilisé pour stocker les versions des paquets installés.
#
# @date 2025-05-05
##

from flask import Flask, request, jsonify
import subprocess
import base64

app = Flask(__name__)

@app.route('/update', methods=['POST'])
def update():
    """
    @brief Installe une liste de paquets à partir d'un fichier encodé en base64.
    
    Le fichier fourni doit contenir un nom de paquet par ligne.
    Chaque paquet est installé via `apt-get install`.
    Les versions installées sont enregistrées pour permettre un rollback ultérieur.

    @return Réponse JSON indiquant le succès de l'opération.
    """
    data = request.json
    filedata = data['filedata']
    decoded_data = base64.b64decode(filedata).decode('utf-8')

    packages = decoded_data.splitlines()

    for package in packages:
        subprocess.run(["sudo", "apt-get", "install", package])

    # Sauvegarde des versions actuelles
    with open("rollbackFile", 'w') as f:
        for package in packages:
            result = subprocess.run(["dpkg-query", "-W", "-f=${Version}", package], capture_output=True, text=True)
            version = result.stdout.strip() if result.returncode == 0 else "unknown"
            f.write(f"{package}={version}\n")

    return jsonify({"status": "Update completed"}), 200


@app.route('/rollback', methods=['GET'])
def rollback():
    """
    @brief Restaure les paquets à leurs versions précédentes enregistrées.

    Lit le fichier "rollbackFile" généré lors de la mise à jour précédente
    et installe chaque paquet à la version correspondante.

    @return Réponse JSON indiquant le succès de l'opération.
    """
    with open("rollbackFile", 'r') as f:
        packages = f.read().splitlines()

    for package in packages:
        package_name, version = package.split('=')
        subprocess.run(["sudo", "apt-get", "install", f"{package_name}={version}"])
    
    print(f"rollbacked:\n{packages}")
    return jsonify({"status": "Rollback completed"}), 200


@app.route('/ping', methods=['GET'])
def ping():
    """
    @brief Vérifie si l'appareil est en ligne.

    @return Réponse JSON indiquant que le serveur est actif
    avec un champ "device" renseignant "raspberry-pi".
    """
    return jsonify({"status": "alive", "device": "raspberry-pi"}), 200


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
