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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void scanNetwork();
    void startUpdate();
    void sendRollback();
    void selectUpdateFile();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *networkManager;
    QFile updateFile;
};

#endif // MAINWINDOW_H
