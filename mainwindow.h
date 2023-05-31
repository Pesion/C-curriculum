#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include<QString>
#include<QFile>
#include<QTextStream>
#include <QFileDialog>
#include<QDebug>
#include <QThread>
#include"explain.h"
#include"graphicedit.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Explain *e;
    GraphicEdit *g;
public slots:
    void time_update(void);

private slots:
    void on_action_1_triggered();
    void on_action_2_triggered();
    void on_action_3_triggered();
};
#endif // MAINWINDOW_H
