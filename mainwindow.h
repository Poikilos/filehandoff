#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QString path;
    QString ext_string;
    QString handoff_type;
    QString browsers_list_path;
    QString myfolder_name;
    QString mydata_path;
    QStringList args;
    QStringList browser_search_paths;
    //QStringList text_editor_search_paths;
    //QStringList pro_editor_search_paths;
    QMap<QString, QStringList> editor_search_paths;
    QMap<QString, QString> convert_ext;
    Ui::MainWindow *ui;
    gid_t getGroupIdByName(const char *name);
    uid_t getUserIdByName(const char *name);
public slots:
    void handoff();
};

#endif // MAINWINDOW_H
