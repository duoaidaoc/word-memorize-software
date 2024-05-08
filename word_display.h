#ifndef WORD_DISPLAY_H
#define WORD_DISPLAY_H

#include <QWidget>
#include "util.h"
#include "word.h"
#include <QPropertyAnimation>
#include <QMediaPlayer>
#include <QAudioOutput>

namespace Ui {
class word_display;
}

class word_display : public QWidget
{
    Q_OBJECT

public:
    explicit word_display(QWidget *parent = nullptr);
    ~word_display();
    void set_content(const db::WordInfo &wd, int seq_ = 0);
    void enter();
    void set_nexpos(QPoint p);
protected:
    void paintEvent(QPaintEvent *event)override;
private:
    Ui::word_display *ui;
  QPropertyAnimation *alpha_animation;
    QPropertyAnimation* move_Animation;
    QImage* img;
    QPoint nexpos;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;

    db::WordInfo wd;
    int seq;

signals:
    void upd_page(int seq_);
};

#endif // WORD_DISPLAY_H
