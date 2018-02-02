#include "downloader.h"
#include "ui_downloader.h"

qint64 max = 0;

//pocetak (inicijalizaija glavnoga prozora)
downloader::downloader(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::downloader)
{
    ui->setupUi(this);
    this->setWindowTitle("QT Primjer - Downloader");
    ui->tableWidget->horizontalHeader()->resizeSection(3, 200);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->lineEdit->setText("POSTAVITE URL LINK");
    ui->lineEdit_2->setDisabled(true);
    ui->lineEdit_2->setText("KLIKNITE NA 'CHOOSE'");

    //postavke prozora
    connect(ui->lineEdit,&QLineEdit::textEdited,this,&downloader::check_edit_box);
    connect(ui->lineEdit_2,&QLineEdit::textEdited,this,&downloader::check_edit_box);

    //rad u pozadini
    sdownloader=new sdownload;
    connect(sdownloader,&sdownload::everything,this,&downloader::set_row);
    connect(this,&downloader::accepted,sdownloader,&sdownload::download);
}

//desktruktor
downloader::~downloader()
{
    delete ui;
}

//postavljanje elemenata u tablicu
void downloader::set_row(QProgressBar *bar, QTableWidgetItem *v, QTableWidgetItem *sp, int poz)
{
    qDebug() << poz << endl;
    ui->tableWidget->setCellWidget(poz,2,bar);
    ui->tableWidget->setItem(poz,1,v);
    ui->tableWidget->setItem(poz,3,sp);
    ui->tableWidget->resizeColumnsToContents();
}

//spremanje datoteke pod određenim imenom (osiguranje da ne dolazi do overwrite-a dateteke)
QString downloader::saveFileName(const QUrl &url, const QString &file)
{
        QString path = url.path();
        QString basename = QFileInfo(path).fileName();

        if (basename.isEmpty())
            basename = "download";

        int i = 0;
        QString j = "";
        while (QFile::exists(file+"/"+j+basename)) {
                ++i;
                j=QString::number(i);
        }
        if (i != 0) {
            basename = QString::number(i) + basename;
        }

        return basename;
}

//odabir foldera za spremanje datoteke
void downloader::on_pushButton_clicked()
{
    QString str = QFileDialog::getExistingDirectory(this);
    QString new_name = str+"/"+saveFileName(QUrl(ui->lineEdit->text()),str);
    ui->lineEdit_2->setText(new_name);
}

//klik na ok button (pokreće se preiuzimanje konkretne datoteke preko url linka)
void downloader::on_pushButton_2_clicked()
{
    QString httpHelp;
    QString fullPath;
    QString directory;

    //zamijeni https sa http zbog ograničenja QNetworkAccessManager-a
    if (ui->lineEdit->text().startsWith("https")){
        httpHelp = (ui->lineEdit->text()).replace("https","http");
        ui->lineEdit->setText(httpHelp);
    }

    //dohvat basename-a iz url linka
    QString basename = ui->lineEdit->text().mid(ui->lineEdit->text().lastIndexOf('/'),-1);
    directory = ui->lineEdit_2->text().mid(0,ui->lineEdit_2->text().lastIndexOf('/'))+"/";
    if (basename.contains('.')) {
        fullPath = directory + saveFileName(QUrl(ui->lineEdit->text()),directory);
    }
    else {
        fullPath = directory + saveFileName(QUrl(ui->lineEdit->text()+".html"),directory);
    }
    qDebug() << directory;

    //provjeriti dostupnost url-a te valjanost lokacije za spremanje na disk
    if (ui->lineEdit->text().isEmpty() || !urlDostupan(ui->lineEdit->text())) {
       ui->lineEdit->setText("NIJE UNESEN ISPRAVAN URL!!!");
       return;
    }
    if (ui->lineEdit_2->text().isEmpty() || QDir(directory).exists()==false) {
       ui->lineEdit_2->setText("NIJE ODABRAN FOLDER!!!");
       return;
    }

    //zapiši praznu datoteku
    ui->lineEdit_2->setText(directory);
    QFile f1(fullPath);
    f1.open( QIODevice::WriteOnly|QIODevice::Append );

    //dodavanje retka u tablicu
    int table_postion = (ui->tableWidget->rowCount());
    QTableWidgetItem *item = new QTableWidgetItem(fullPath);
    ui->tableWidget->insertRow(table_postion);
    ui->tableWidget->setItem(table_postion,0,item);

    //važno!!!
    emit accepted(QUrl(ui->lineEdit->text()),fullPath);
}

void downloader::check_edit_box(){ //grafički dio
    if (ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty()) {
        ui->pushButton_2->setEnabled(false);
    }
    else if (!ui->lineEdit->text().startsWith("http")) {
        ui->pushButton_2->setEnabled(false);
    }
    else {
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
    }
    if (ui->lineEdit->text().isEmpty()) {
        ui->pushButton->setEnabled(false);
    }
}

//provjera dostupnosti url linka
bool downloader::urlDostupan (QString url) {
    QUrl urlReal = QUrl(url);
    QTcpSocket socket;
    socket.connectToHost(urlReal.host(), 80);
    if (socket.waitForConnected()) {
            socket.write("HEAD " + urlReal.path().toUtf8() + " HTTP/1.1\r\n"
                         "Host: " + urlReal.host().toUtf8() + "\r\n"
                         "\r\n");
        if (socket.waitForReadyRead()) {
            QByteArray bytes = socket.readAll();
        if (bytes.contains("200 OK"))
            return true;
        }
    }
    return false;
}
