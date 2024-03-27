
#ifndef WORD_FRAME_H
#define WORD_FRAME_H
#include "QFrame"
#include "word.h"
#include <QHBoxLayout>
#include <qlabel.h>
#include <qpushbutton.h>
#include <QMediaPlayer>
#include <QAudioOutput>

class word_frame : public QFrame
{
    Q_OBJECT
public:
    explicit word_frame(QWidget* parent = 0);
    void set_content(const Word& wd);
private:
    QHBoxLayout* hb;
    QPushButton* sound_btn;
    QLabel* word_label;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    Word content;
};

#endif // WORD_FRAME_H
