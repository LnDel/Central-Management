#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkInterface>
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>
#include <string>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

QString getDefaultGateway();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    networkManager = new QNetworkAccessManager(this);

    connect(ui->scanButton, &QPushButton::clicked, this, &MainWindow::scanNetwork);
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::startUpdate);
    connect(ui->rollbackButton, &QPushButton::clicked, this, &MainWindow::sendRollback);
    connect(ui->selectFileButton, &QPushButton::clicked, this, &MainWindow::selectUpdateFile);

    ui->statusText->append("Server started");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::scanNetwork()
{
    ui->piListWidget->clear();
    ui->statusText->append("Scanning network using /ping...");

    QString baseIp = getDefaultGateway()+".";

    for (int i = 1; i <= 254; ++i) {
        QString ip = baseIp + QString::number(i);
        QUrl url(QString("http://%1:5000/ping").arg(ip));

        QNetworkRequest request(url);
        QNetworkReply *reply = networkManager->get(request);

        connect(reply, &QNetworkReply::finished, this, [this, reply, ip]() {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray responseData = reply->readAll();
                QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
                QJsonObject jsonObj = jsonDoc.object();

                if (jsonObj["status"] == "alive" && jsonObj["device"] == "raspberry-pi") {
                    QListWidgetItem *item = new QListWidgetItem("Raspberry Pi - " + ip);
                    ui->piListWidget->addItem(item);
                    ui->statusText->append("Detected Raspberry Pi at: " + ip);
                }
            }
            reply->deleteLater();
        });
    }
}


void MainWindow::selectUpdateFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select Update File", "");
    if (!filePath.isEmpty()) {
        ui->filePathLabel->setText(filePath);
    }
}


QString getDefaultGateway() {
    QFile routeFile("/proc/net/route");
    if (!routeFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Erreur ouverture fichier:" << routeFile.errorString();
        return QString();
    }

    QByteArray fileData = routeFile.readAll();
    routeFile.close();

    QTextStream in(fileData);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split('\t', Qt::SkipEmptyParts);
        if (parts.size() >= 3) {
            if (parts[1] == "00000000") {  // Route par défaut
                bool ok;
                quint32 gateway = parts[2].toUInt(&ok, 16);
                if (!ok) continue;

                struct in_addr addr;
                addr.s_addr = htonl(gateway);
                QStringList ip_addr = QString(inet_ntoa(addr)).split('.');
                // Inversion de l'adresse IP + on garde seulement les 3 premiers octets
                return QString("%1.%2.%3")
                    .arg(ip_addr[3])
                    .arg(ip_addr[2])
                    .arg(ip_addr[1  ]);
            }
        }
    }

    return QString();
}


void MainWindow::startUpdate()
{
    if (ui->piListWidget->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select at least one Pi");
        return;
    }

    QString filePath = ui->filePathLabel->text();
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select an update file");
        return;
    }

    updateFile.setFileName(filePath);
    if (!updateFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "Could not open update file");
        return;
    }

    foreach (QListWidgetItem *item, ui->piListWidget->selectedItems()) {
        QString piAddress = item->text().split(" - ").at(1);
        QUrl url(QString("http://%1:5000/update").arg(piAddress));

        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QJsonObject json;
        json["filename"] = QFileInfo(updateFile).fileName();
        json["filedata"] = QString(updateFile.readAll().toBase64());
        updateFile.close();

        QJsonDocument doc(json);
        QByteArray data = doc.toJson();

        QNetworkReply *reply = networkManager->post(request, data);
        connect(reply, &QNetworkReply::finished, this, [this, reply, piAddress]() {
            if (reply->error() == QNetworkReply::NoError) {
                ui->statusText->append(piAddress + " updated successfully");
            } else {
                ui->statusText->append("Error sending update command to " + piAddress);
            }
            reply->deleteLater();
        });
    }
}

void MainWindow::sendRollback()
{
    if (ui->piListWidget->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select at least one Pi");
        return;
    }

    foreach (QListWidgetItem *item, ui->piListWidget->selectedItems()) {
        QString piAddress = item->text().split(" - ").at(1);
        QUrl url(QString("http://%1:5000/rollback").arg(piAddress));

        QNetworkRequest request(url);
        QNetworkReply *reply = networkManager->get(request);
        connect(reply, &QNetworkReply::finished, this, [this, reply, piAddress]() {
            if (reply->error() == QNetworkReply::NoError) {
                ui->statusText->append("Rollback command sent to " + piAddress);
            } else {
                ui->statusText->append("Error sending rollback command to " + piAddress);
            }
            reply->deleteLater();
        });
    }
}
