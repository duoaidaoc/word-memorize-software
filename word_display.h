#ifndef WORD_DISPLAY_H
#define WORD_DISPLAY_H

#include <QWidget>
#include "word.h"
#include <QPropertyAnimation>

namespace Ui {
class word_display;
}

class word_display : public QWidget
{
    Q_OBJECT

public:
    explicit word_display(QWidget *parent = nullptr);
    ~word_display();
    void set_content(const Word &wd);
    void enter();
protected:
    void paintEvent(QPaintEvent *event)override;
private:
    Ui::word_display *ui;
    Word wd;
    QPropertyAnimation *animation;
    QPropertyAnimation* moveAnimation;
    QImage* img;
    int paint_cnt;

    QImage grabFrameImage();
    QPixmap screen_shot();


};

#endif // WORD_DISPLAY_H
