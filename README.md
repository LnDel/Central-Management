# Système de Mise à Jour et de Rollback pour Raspberry Pi

Ce projet permet de gérer les mises à jour et les rollbacks de plusieurs Raspberry Pi à partir d'une interface graphique Qt. Les Raspberry Pi exécutent un serveur Flask qui reçoit les commandes de mise à jour et de rollback via des requêtes HTTP.

## Prérequis

- Qt 5.15 ou supérieur
- Python 3.x
- Flask
- `wget` (pour télécharger les fichiers de mise à jour)

## Configuration de l'Application Qt

### Étape 1 : Installer Qt

### Étape 2 : Configurer le Projet Qt

1. Clonez ce dépôt sur votre machine.
2. Ouvrez le fichier `.pro` avec Qt Creator.
3. Configurez le projet en sélectionnant le kit approprié.
4. Compilez et exécutez le projet.

### Étape 3 : Utiliser l'Application

1. **Scanner le Réseau** : Cliquez sur le bouton "Scan Network" pour détecter les Raspberry Pi sur le réseau local.
2. **Sélectionner un Fichier de Mise à Jour** : Cliquez sur "Select Update File" pour choisir le fichier de mise à jour.
3. **Lancer la Mise à Jour** : Sélectionnez un ou plusieurs Raspberry Pi dans la liste et cliquez sur "Start Update".
4. **Lancer le Rollback** : Sélectionnez un ou plusieurs Raspberry Pi dans la liste et cliquez sur "Send Rollback".

## Configuration des Raspberry Pi

### Étape 1 : Installer Python et Flask

Assurez-vous que Python 3.x est installé sur chaque Raspberry Pi. Installez Flask en exécutant la commande suivante :

~ sudo pip install flask

### Étape 2 : Configurer le Serveur Flask

1. Copiez les fichiers app.py, update_script.sh, et rollback_script.sh sur chaque Raspberry Pi.
2. Rendez les scripts exécutables : chmod +x update_script.sh rollback_script.sh

### Étape 3 : Lancer le Serveur Flask
Sur chaque Raspberry Pi, lancez le serveur Flask avec la commande suivante : python3 app.py
Le serveur Flask écoutera sur le port 5000 et sera prêt à recevoir des commandes de mise à jour et de rollback.

### Scripts de Mise à Jour et de Rollback
1. update_script.sh
Ce script est exécuté lorsqu'une commande de mise à jour est reçue. Il télécharge le fichier de mise à jour et exécute les opérations nécessaires.

2. rollback_script.sh
Ce script est exécuté lorsqu'une commande de rollback est reçue. Il restaure le système à son état précédent.

### Remarques
Assurez-vous que les Raspberry Pi et la machine exécutant l'application Qt sont sur le même réseau local.

### Problèmes rencontrés
Nous avons d'abord essayé avec MQTT, car nous étions plus familier avec cet outil. Cependant, on s'est vite rendu compte qu'installer MQTT sur Qt était extrêmement laborieux, cela nous a fait perdre pas mal de temps donc on a décidé de se tourner avec des serveurs HTTP.

