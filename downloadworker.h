#ifndef DOWNLOADWORKER_H
#define DOWNLOADWORKER_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QString>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QTableWidget>
#include <QProgressBar>

class DownloadWorker : public QObject
{
    Q_OBJECT

private:
    void saveToDisk(QNetworkReply *reply);
public:
    void check_file(const QString &file);
    QString worker_file;
    QProgressBar *progressBar;
    QTableWidgetItem *int_item;
    QTableWidgetItem *speed_item;
    QTime *downloadTime;
    qint64 red;

public slots:
    void doDownload(const QUrl &url, QNetworkAccessManager *nm);
    void onDownloadFinished(QNetworkReply *reply);

signals:
    void downloadProgress(qint64, qint64);
    void errorString(const QString &);
};

#endif // DOWNLOADWORKER_H

