#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QMainWindow>
#include <QDialog>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QTableWidget>
#include <QProgressBar>
#include <QTimer>
#include <QTime>
#include <QLineEdit>
#include <QQueue>
#include <QUrl>

#include "sdownload.h"

namespace Ui {
class downloader;
}

class downloader : public QMainWindow
{
    Q_OBJECT

public:
    explicit downloader(QWidget *parent = 0);
    ~downloader();
    sdownload *sdownloader;
    Ui::downloader *ui;
    QThread *dretva;

public slots:
    void set_row(QProgressBar*, QTableWidgetItem*, QTableWidgetItem*, int);

private:
    bool urlDostupan (QString url_string);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    QString saveFileName (const QUrl &url, const QString &file);
    void check_edit_box();

signals:
    void downloadProgress(qint64, qint64);
    void accepted(const QUrl &, const QString &);
};

#endif // DOWNLOADER_H
