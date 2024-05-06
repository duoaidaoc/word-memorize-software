#ifndef STUDENT_MAIN_H
#define STUDENT_MAIN_H

#include <QWidget>
#include "class_frame.h"
#include "classcue_frame.h"
#include "task_frame.h"
#include "word_frame.h"
#include "word_display.h"
#include "question.h"

namespace Ui {
class student_main;
}

class student_main : public QWidget
{
    Q_OBJECT

public:
    explicit student_main(QWidget *parent = nullptr);
    ~student_main();
    void move_display();
private:
    Ui::student_main *ui;

    QVBoxLayout *word_layout,*class_layout,*task_layout;
    std::vector<word_frame* >word_frames;
    std::vector<class_frame* >class_frames;
    std::vector<task_frame* >task_frames;

    QFrame *v_line,*h_line;
    word_display *w_display;

    QVBoxLayout* student_fixed_layout;
    QHBoxLayout* student_appending_layout[2];


    classcue_frame *c_frame;
    question* que_widget;
    void setup();
    void setaction();
    void addword(const Word &word);
private slots:
    void show_display(const Word &wd, int seq);
    void update_display(int seq_ = 0);
};

#endif // STUDENT_MAIN_H
