#include "downloadworker.h"
#include <QThread>
#include <qobject.h>

//ovdje se odvija glavni mrezni dio aplikacije (request,reply)
void DownloadWorker::doDownload(const QUrl &url,QNetworkAccessManager *nm)
{
    QNetworkRequest req(url);
    req.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    QNetworkReply *reply = nm->get(req);
    connect(reply, &QNetworkReply::downloadProgress,this,&DownloadWorker::downloadProgress);
}

//provjera datoteke
void DownloadWorker::check_file(const QString &file) {
    this->worker_file=file;
}

//podaci u bufferu, korektivne radnje
void DownloadWorker::onDownloadFinished(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        emit errorString(reply->errorString());
    }
    else {
        saveToDisk(reply);
    }
    reply->deleteLater(); //priprema reply-a za brisanje (osigurava sigurno brisanje objekta)
}

//spremi datoteku pod tim i tim nazivom
void DownloadWorker::saveToDisk(QNetworkReply *reply){
    QFile f(this->worker_file);
    f.open(QIODevice::WriteOnly | QIODevice::Truncate);
    f.write(reply->readAll());
    f.close();
}
