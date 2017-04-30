#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QTextStream>
#include <QProcess>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTimer::singleShot(0, this, SLOT(handoff()));
    //handoff();
}

void MainWindow::handoff()
{
    //NOTE: arguments().at(0) is self
    int self_offset=1;
    bool url_enable=false;  //not really used--only useful for URL file syntax checking
    QString reconstructed_command;
    QProcessEnvironment environs = QProcessEnvironment::systemEnvironment();
    if (QCoreApplication::arguments().length()>self_offset) {
        if (QCoreApplication::arguments().length()==self_offset+1) {
            QString source_path = QCoreApplication::arguments().at(self_offset);
            if (source_path.toLower().endsWith(".url")) {
                this->path="firefox";
                this->ext_string="url";
                //this->args.append(theoretical_path);
                QFile inputFile(source_path);
                if (inputFile.open(QIODevice::ReadOnly))
                {
                   QTextStream in(&inputFile);
                   while (!in.atEnd())
                   {
                      QString line = in.readLine();
                      if (line.startsWith("[")) {
                          if (line.toLower().startsWith("[internetshortcut]")) url_enable=true;
                          else url_enable=false;
                      }
                      else if (line.toLower().startsWith("url=")) {
                          this->args.append(line.right(line.length()-4).trimmed());
                          break;
                      }
                   }
                   inputFile.close();
                }
            }
            else if (source_path.toLower().endsWith(".psd") || source_path.toLower().endsWith(".jpg") || source_path.toLower().endsWith(".jpeg") || source_path.toLower().endsWith(".jpe")) {
                //would be something like env WINEPREFIX="/home/owner/win32" /usr/bin/wine C:\\Program\ Files\\Adobe\\Photoshop\ Elements\ 5.0\\Photoshop\ Elements\ 5.0.exe
                //unless you have correctly edited ~/.bashrc and added something like:
                //export WINEPREFIX=/home/owner/win32
                //export WINEARCH=win32

                QString wine_prefix = QDir::home().filePath(".wine");
                if (QDir("Folder").exists(QDir::home().filePath("win32"))) {
                    wine_prefix = QDir::home().filePath("win32");
                }
                environs.insert("WINEARCH","win32");
                environs.insert("WINEPREFIX",wine_prefix);

                this->ext_string="psd";

                //this->path="/usr/bin/wine";
                this->path="wine";
                reconstructed_command="env";
                reconstructed_command+=" WINEARCH=win32";
                reconstructed_command+=" WINEPREFIX="+wine_prefix;

                //NOTE: reconstructed_command is not needed (neither does it work).
                //see also:
                //* http://stackoverflow.com/questions/4265946/set-environment-variables-for-startdetached-qprocess#4267306
                //  which (suggests putenv and) references:
                //  * http://bugreports.qt-project.org/browse/QTBUG-2284
                //  * http://code.qt.io/cgit/qt/qtbase.git/tree/src/corelib/io?h=5.5
                //  which references:
                //  * https://bugreports.qt.io/browse/QTBUG-26343
                //    which gives code similar to code below as a workaround

                //this->path="env";
                //this->args.append("WINEARCH=win32");
                //this->args.append("WINEPREFIX="+wine_prefix);

                this->args.append("C:\\Program\ Files\\Adobe\\Photoshop\ Elements\ 5.0\\Photoshop\ Elements\ 5.0.exe");
                QString root_winpath = "Z:";
                QString dest_path = root_winpath + source_path.replace("/","\\");
                this->args.append(dest_path);
                for (int i=0; i<this->args.length(); i++) reconstructed_command+=" "+args.at(i);
            }
            else {
            }
        }
        else { //too many arguments, so dump them to the screen:
            for (int i=self_offset; i<QCoreApplication::arguments().length(); i++) {
               ui->listWidget->addItem(QCoreApplication::arguments().at(i));
            }
        }
    }

    if (this->path.length()>0) {
        //QCoreApplication.exit(0);
        if (this->path.endsWith("wine")) {
            ui->listWidget->addItem("[WINEPREFIX: "+qgetenv("WINEPREFIX")+"]");
            ui->listWidget->addItem("[WINEARCH: "+qgetenv("WINEARCH")+"]");
        }
        ui->listWidget->addItem(this->path);
        ui->listWidget->addItems(this->args);
        if (this->path.endsWith("wine")) {
            QProcess process;
            process.setProcessEnvironment(environs);
            QByteArray data;
            process.start(this->path, this->args);
            if(process.waitForStarted()) {
                while(process.waitForReadyRead())
                    data.append(process.readAll());
                QFile file("/tmp/filehandoff.txt");
                file.open(QIODevice::WriteOnly);
                file.write(data);
                file.close();
                //QApplication::quit();
                this->close();
            }
        }
        else {
            QProcess::startDetached(this->path, this->args);
            //QApplication::quit();
            this->close();
        }
        //QTimer::singleShot(0, this, SLOT(QApplication::quit()));
        //QTimer::singleShot(0, this, SLOT(this->close()));
        //exit(0);
        //qApp->exit(0);
    }
    else {
        ui->listWidget->addItem("Nothing to do (the path to the original file was not found).");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
