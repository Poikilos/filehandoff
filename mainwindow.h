#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    QString browsers_list_path;
    QString myfolder_name;
    QString mydata_path;
    QStringList args;
    QStringList browser_search_paths;
    Ui::MainWindow *ui;
public slots:
    void handoff();
};

#endif // MAINWINDOW_H
