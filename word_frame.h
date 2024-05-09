
#ifndef WORD_FRAME_H
#define WORD_FRAME_H
#include "QFrame"
#include "util.h"
#include "word.h"
#include <QHBoxLayout>
#include <qlabel.h>
#include <qpushbutton.h>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMouseEvent>
#include <QEvent>
#include <QWidget>


class word_frame : public QFrame
{
    Q_OBJECT
public:
    explicit word_frame(QWidget* parent = 0);
    void set_content(const db::WordInfo& wd, int seq_ = 0);
    void mousePressEvent(QMouseEvent* event) override;
    void set_btn_disabled();
    db::WordInfo get_content();
signals:
    void set_display_content(const db::WordInfo &wd ,int _seq);

private:
    QHBoxLayout* hb;
    QPushButton* sound_btn;
    QLabel* word_label;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    db::WordInfo content;
    int seq;
};

#endif // WORD_FRAME_H
