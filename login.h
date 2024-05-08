#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "tip.h"
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
    void actionSet();
    QTimer *timer;
    QString label_path;
    tip *Tip;
private slots:
    void label_change(int rate);
    void set_curr_time();
signals:
    void turn_to(bool);
};

#endif // LOGIN_H
