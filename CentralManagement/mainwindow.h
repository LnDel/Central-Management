/**
 * @file MainWindow.h
 * @brief Déclaration de la classe MainWindow pour la gestion des Raspberry Pi.
 *
 * Ce fichier définit l'interface de la classe MainWindow qui permet
 * de scanner le réseau local, de lancer des mises à jour et de faire un rollback
 * sur des Raspberry Pi via une interface Qt.
 *
 *
 * @date 2025-05-05
 */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief La classe MainWindow gère l'interface utilisateur principale pour la gestion des Raspberry Pi.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur de MainWindow.
     * @param parent Le widget parent.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructeur de MainWindow.
     */
    ~MainWindow();

private slots:
    /**
     * @brief Scanne le réseau local à la recherche de Raspberry Pi.
     */
    void scanNetwork();

    /**
     * @brief Lance la mise à jour des Raspberry Pi sélectionnés.
     */
    void startUpdate();

    /**
     * @brief Envoie une commande de rollback aux Raspberry Pi sélectionnés.
     */
    void sendRollback();

    /**
     * @brief Ouvre une boîte de dialogue pour sélectionner le fichier de mise à jour.
     */
    void selectUpdateFile();

private:
    Ui::MainWindow *ui;                       ///< Interface utilisateur.
    QNetworkAccessManager *networkManager;   ///< Gestionnaire des requêtes réseau.
    QFile updateFile;                         ///< Fichier de mise à jour à envoyer.
};

#endif // MAINWINDOW_H
