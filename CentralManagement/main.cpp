/**
 * @file Main.cpp
 * @brief Fichier principal de l'application Pi Central Management.
 *
 * Ce fichier contient le point d'entrée `main()` de l'application,
 * qui initialise l'interface utilisateur basée sur Qt.
 *
 * @author
 * @date 2025-05-06
 */

#include "mainwindow.h"
#include <QApplication>

/**
 * @brief Point d'entrée principal de l'application.
 * 
 * Initialise l'application Qt et affiche la fenêtre principale.
 * 
 * @param argc Nombre d'arguments en ligne de commande.
 * @param argv Valeurs des arguments en ligne de commande.
 * @return int Code de sortie de l'application.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
