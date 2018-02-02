#ifndef SDOWNLOAD
#define SDOWNLOAD


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
#include <QThread>

#include "downloadworker.h"


class sdownload : public QObject
{
    Q_OBJECT
public:
    explicit sdownload(QObject *parent = 0);
    ~sdownload();
    DownloadWorker *worker;
    QThread *workerThread;
public slots:
    void download(const QUrl &url, const QString &file);
    void onDownloadProgress (qint64,qint64);
signals:
    void downloadProgress(qint64, qint64);
    void everything(QProgressBar*, QTableWidgetItem*, QTableWidgetItem*, int);
private:
    QString *saveFile;
    QNetworkAccessManager *naManager;

private slots:
};

#endif // SDOWNLOAD

