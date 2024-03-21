#ifndef LOGIN_H
#define LOGIN_H

#include "qlabel.h"
#include <QWidget>

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

private:
    Ui::login *ui;
    void SetUp();
    QString randomselect(QString path);
    void actionSet();
    QTimer *timer;
private slots:
    void label_change(int rate);
    void set_curr_time();
    void set_font(QLabel* obj,int size);
};

#endif // LOGIN_H
