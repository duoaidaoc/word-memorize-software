#ifndef STUDENT_MAIN_H
#define STUDENT_MAIN_H

#include <QWidget>
#include "word_frame.h"
#include "word_display.h"

namespace Ui {
class student_main;
}

class student_main : public QWidget
{
    Q_OBJECT

public:
    explicit student_main(QWidget *parent = nullptr);
    ~student_main();
    void movedisplay();
private:
    Ui::student_main *ui;

    QVBoxLayout *layout;
    QFrame *v_line,*h_line;
    word_display *display;
    std::vector<word_frame* >word_frames;

    void setup();
    void setaction();
    void addword(const Word &word);
private slots:
    void show_display(const Word &wd, int seq);
    void update_display(int seq_ = 0);
};

#endif // STUDENT_MAIN_H
