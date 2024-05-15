#ifndef STUDENT_MAIN_H
#define STUDENT_MAIN_H

#include <QWidget>
#include "class_frame.h"
#include "classcue_frame.h"
#include "settings.h"
#include "task_frame.h"
#include "word_frame.h"
#include "word_display.h"
#include "question.h"
#include "word.h"
#include "tip.h"
#include "game.h"

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
    void data_setup();
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

    classcue_frame *Ccue_frame;
    question* que_widget;

    std::vector<ques>questions;
    tip *Tip, *signaltip_1,*signaltip_2;

    CClass cls_;

    game* game_widget;

    Settings *settings;

    QImage* profile_pic;
    QString profile_pic_url;
    QPixmap* profile_pic_pix;

    void setup();
    void setaction();
    void test();
    void clearlayout(QBoxLayout *lb);
    void clearNowWord();
    void setNowWord(qint64 tid);
    void clearNowTask();
    void setNowTask();


private slots:
    void show_display(const db::WordInfo &wd, int seq);
    void update_display(int seq_ = 0);
    void update_class();
};

#endif // STUDENT_MAIN_H
