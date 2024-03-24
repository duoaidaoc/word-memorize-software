#ifndef STUDENT_MAIN_H
#define STUDENT_MAIN_H

#include <QWidget>
#include "word_frame.h"

namespace Ui {
class student_main;
}

class student_main : public QWidget
{
    Q_OBJECT

public:
    explicit student_main(QWidget *parent = nullptr);
    ~student_main();

private:
    Ui::student_main *ui;

    QVBoxLayout *layout;
    QFrame *v_line,*h_line;

    void setup();
    void setaction();
    void addword(const Word &word);
    QString randomselect(QString path);
};

#endif // STUDENT_MAIN_H
