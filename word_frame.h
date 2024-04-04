
#ifndef WORD_FRAME_H
#define WORD_FRAME_H
#include "QFrame"
#include "word.h"
#include <QHBoxLayout>
#include <qlabel.h>
#include <qpushbutton.h>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMouseEvent>
#include <QEvent>
#include <QWidget>
#include "word_display.h"


class word_frame : public QFrame
{
    Q_OBJECT
public:
    explicit word_frame(QWidget* parent = 0);
    void set_content(const Word& wd);
    void mousePressEvent(QMouseEvent* event) override;

private:
    QHBoxLayout* hb;
    QPushButton* sound_btn;
    QLabel* word_label;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    Word content;

};

#endif // WORD_FRAME_H
