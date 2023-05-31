#ifndef EXPLAIN_H
#define EXPLAIN_H

#include <QWidget>
#include <QFileDialog>
#include<QDebug>
#include <QThread>

namespace Ui {
class Explain;
}

class Explain : public QWidget
{
    Q_OBJECT

public:
    explicit Explain(QWidget *parent = nullptr);
    ~Explain();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Explain *ui;
};

#endif // EXPLAIN_H
