#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QTextStream>
#include <QProcess>
#include <QTimer>
#include <QDebug>
#include <QStandardPaths>
#include <QtGlobal>  // qEnvironmentVariableIsSet etc
#include <pwd.h>
#include <grp.h>

//see https://stackoverflow.com/questions/1009254/programmatically-getting-uid-and-gid-from-username-in-unix
gid_t MainWindow::getGroupIdByName(const char *name)
{
    struct group *grp = getgrnam(name); /* don't free, see getgrnam() for details */
    if(grp == NULL) {
        //throw runtime_error(std::string("Failed to get groupId from groupname : ") + name);
        throw std::runtime_error(std::string("Failed to get groupId from groupname : ") + name);
    }
    return grp->gr_gid;
}

//see https://stackoverflow.com/questions/1009254/programmatically-getting-uid-and-gid-from-username-in-unix
uid_t MainWindow::getUserIdByName(const char *name)
{
    struct passwd *pwd = getpwnam(name); /* don't free, see getpwnam() for details */
    if(pwd == NULL) {
        //throw runtime_error(std::string("Failed to get userId from username : ") + name);
        throw std::runtime_error(std::string("Failed to get userId from username : ") + name);
    }
    return pwd->pw_uid;
}

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
            QString source_path_original = source_path;
            if (source_path.toLower().startsWith("smb://") || source_path.startsWith("\\\\")) {
                int server_start = 6;
                QString slash = "/";
                if (source_path.startsWith("\\\\")) {
                    server_start = 2;
                    slash="\\";
                }
                int server_end = source_path.indexOf(slash, server_start);
                if (server_end > -1) {
                    int share_i = server_end + 1;
                    int share_end = source_path.indexOf(slash, share_i);
                    QString slash_sub_path = "";
                    if (share_end<=-1) share_end = source_path.length();
                    else {
                        slash_sub_path = source_path.mid(share_end);
                        if (slash=="\\") slash_sub_path = slash_sub_path.replace("\\", "/");
                    }
                    try {
                        QString share_name = source_path.mid(share_i, share_end-share_i).toLower();
                        //if (qEnvironmentVariableIsSet("UID")) {
                        //NOTE: UID environment variable doesn't work in this context (probably since GUI)
                        QString server_name = source_path.mid(server_start, server_end-server_start).toLower();
                        //QString uid_string = QString::fromLocal8Bit( qgetenv("UID") );
                        QString this_user_name = qgetenv("USER");
                        QString uid_string = QString::number( getUserIdByName(this_user_name.toStdString().c_str()) );

                        source_path = "/run/user/" + uid_string + "/gvfs/smb-share:server=" + server_name + ",share=" + share_name + slash_sub_path;
                        qDebug() << "source_path changed from \"" << source_path_original << " to " << source_path;
                        //}
                        //else qDebug() << "missing environment variable UID";
                    }
                    //see <https://stackoverflow.com/questions/4661883/qt-c-error-handling>
                    catch (std::exception &e) {
                        //qFatal("Error %s sending event %s to object %s (%s)",
                            //e.what(), typeid(*event).name(), qPrintable(receiver->objectName()),
                            //typeid(*receiver).name());
                        qFatal("Could not finish: %s sending event to object",
                               e.what());
                        qDebug() << "Could not finish: " << e.what();
                    } catch (...) {
                        //qFatal("Error <unknown> sending event %s to object %s (%s)",
                            //typeid(*event).name(), qPrintable(receiver->objectName()),
                            //typeid(*receiver).name());
                        qFatal("Could not finish <unknown> sending event to object");
                        qDebug() << "Could not finish for unknown reason";
                    }
                }
            }
            if (source_path.toLower().endsWith(".url")) {
                this->myfolder_name="filehandoff";
                QString homeLocation = QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory);
                QString apps_data_path =  QDir::cleanPath(homeLocation + QDir::separator() + ".config");
                if (!QDir(apps_data_path).exists()) QDir().mkdir(apps_data_path);
                this->mydata_path = QDir::cleanPath(apps_data_path + QDir::separator() + this->myfolder_name);
                if (!QDir(this->mydata_path).exists()) QDir().mkdir(this->mydata_path);

                this->browsers_list_path = QDir::cleanPath(this->mydata_path + QDir::separator() + "browsers.txt");

                this->path = "";

                QFile browsersFile(this->browsers_list_path);
                QString try_path;
                if (browsersFile.open(QIODevice::ReadOnly)) {
                   QTextStream in(&browsersFile);
                   while (!in.atEnd()) {
                      QString line = in.readLine();
                      line = line.trimmed();
                      if (line.length()>0) {
                          try_path = line;
                          QFileInfo try_file(try_path);
                          this->path = try_path;
                          if (try_file.exists()) break;
                      }
                   }
                   browsersFile.close();
                }
                else {
                    this->browser_search_paths.append("/usr/bin/waterfox");
                    this->browser_search_paths.append("/usr/bin/firefox-kde");
                    this->browser_search_paths.append("/usr/bin/firefox-nightly");
                    this->browser_search_paths.append("/usr/bin/firefox-developer");
                    this->browser_search_paths.append("/usr/bin/firefox-beta");
                    this->browser_search_paths.append("/usr/bin/firefox");
                    this->browser_search_paths.append("/usr/bin/chromium");
                    this->browser_search_paths.append("/usr/bin/google-chrome");
                    this->browser_search_paths.append("/usr/bin/icecat");
                    this->browser_search_paths.append("/usr/bin/firefox"); //firefox again here to write to browsers.txt if none above exist
                    for (int i = 0; i < this->browser_search_paths.size(); ++i) {
                        //cout << this->browser_search_paths.at(i).toLocal8Bit().constData() << endl;
                        try_path = this->browser_search_paths.at(i);
                        QFileInfo try_file(try_path);
                        this->path = try_path;
                        if (try_file.isFile()) break; //uses /usr/bin/firefox (or whatever is last above) if all else fails
                    }
                    QFile caFile(this->browsers_list_path);
                    caFile.open(QIODevice::WriteOnly | QIODevice::Text);
                    if(!caFile.isOpen()){
                        qDebug() << "ERROR unable to open" << this->browsers_list_path << "for output";
                    }
                    QTextStream outStream(&caFile);
                    outStream << this->path;
                    caFile.close();
                }

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
                          QString url_string = line.right(line.length()-4).trimmed();
                          if (this->path=="/usr/bin/firefox") this->args.append("-new-tab "+url_string);
                          else this->args.append(url_string);
                          break;
                      }
                   }
                   inputFile.close();
                }
            }
            else if (source_path.toLower().endsWith(".mtl")) {
                this->text_editor_search_paths.append("/usr/bin/kate");
                this->text_editor_search_paths.append("/usr/bin/gedit");
                this->text_editor_search_paths.append("/usr/bin/leafpad");
                this->text_editor_search_paths.append("/usr/bin/mousepad");
                QString try_path;
                for (int i = 0; i < this->text_editor_search_paths.size(); ++i) {
                    //cout << this->browser_search_paths.at(i).toLocal8Bit().constData() << endl;
                    try_path = this->text_editor_search_paths.at(i);
                    QFileInfo try_file(try_path);
                    this->path = try_path;
                    this->args.append(source_path);
                    if (try_file.isFile()) break; //uses /usr/bin/firefox (or whatever is last above) if all else fails
                }

                this->ext_string="mtl";
            }
            else if (source_path.toLower().endsWith(".fpp")) {
                QStringList fpp_bin_search_paths;
                fpp_bin_search_paths.append("/usr/bin/flashprint");
                fpp_bin_search_paths.append("/usr/local/bin/flashprint");
                QString try_path;
                for (int i = 0; i < fpp_bin_search_paths.size(); ++i) {
                    //cout << this->browser_search_paths.at(i).toLocal8Bit().constData() << endl;
                    try_path = fpp_bin_search_paths.at(i);
                    QFileInfo try_file(try_path);
                    if (try_file.isFile()) {
                        this->path = try_path;
                        this->args.append(source_path);
                        break; //uses /usr/bin/firefox (or whatever is last above) if all else fails
                    }
                }

                this->ext_string="fpp";
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

                //this->args.append("C:\\Program\ Files\\Adobe\\Photoshop\ Elements\ 5.0\\Photoshop\ Elements\ 5.0.exe");
                this->args.append("C:\\Program Files\\Adobe\\Photoshop Elements 5.0\\Photoshop Elements 5.0.exe");
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
