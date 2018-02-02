#include <sdownload.h>

int i = 0;

sdownload::sdownload(QObject *parent) :
QObject(parent)
{

}

sdownload::~sdownload(){
    this->workerThread->quit();
    this->workerThread->wait();
}
//DOWNLOAD DATOTEKE - REQUEST,REPLY
void sdownload::download(const QUrl &url, const QString &file)
{
    this->naManager = new QNetworkAccessManager(this);
    this->worker=new DownloadWorker;
    this->worker->progressBar = new QProgressBar;
    this->worker->int_item=new QTableWidgetItem("");
    this->worker->speed_item=new QTableWidgetItem("");
    this->worker->red=i;

    emit everything(this->worker->progressBar,this->worker->int_item,
                    this->worker->speed_item, this->worker->red);
    i=i+1;
    this->workerThread=new QThread(this);

    this->worker->downloadTime=new QTime();
    this->worker->worker_file=file;
    this->worker->moveToThread(workerThread);

    connect(workerThread, &QThread::finished, worker,&QObject::deleteLater);
    connect(naManager, &QNetworkAccessManager::finished,worker,
            &DownloadWorker::onDownloadFinished);
    connect(worker, &DownloadWorker::downloadProgress, this,&sdownload::onDownloadProgress);
    this->workerThread->start();

    this->saveFile=new QString(file);

    this->worker->doDownload(url,this->naManager);
    this->worker->downloadTime->start();
}


void sdownload::onDownloadProgress(qint64 r, qint64 t)
{
    DownloadWorker *privremeni = qobject_cast<DownloadWorker *>(QObject::sender());
    DownloadWorker *trenutni=this->worker;

    qDebug() << privremeni << endl;
    qDebug() << trenutni << endl;

    if (privremeni){
        privremeni->progressBar->setMaximum(t);
        privremeni->progressBar->setValue(r);
    }
    if (privremeni) {
        double calc;
        QString s_calc;
        if (t<1024) {
            calc=t;
            s_calc=QString::number(calc)+" B";
        }
        else if (t<1024*1024){
            calc=t/1024;
            s_calc=QString::number(calc)+" KB";
        }
        else {
            calc=t/(1024*1024);
            s_calc=QString::number(calc)+" MB";
        }
        privremeni->int_item->setData(0,s_calc);
    }

    if (privremeni) {
        double speed = r * 1000.0 / privremeni->downloadTime->elapsed();
        QString unit;
        if (speed < 1024) {
            unit = "B/s";
        } else if (speed < 1024*1024) {
            speed /= 1024;
            unit = "KB/s";
        } else {
            speed /= 1024*1024;
            unit = "MB/s";
        }
        privremeni->speed_item->setData(0,QString::number(speed)+unit);
    }

        qDebug() << privremeni->red << endl;
}

