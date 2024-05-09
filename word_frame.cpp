
#include "word_frame.h"
#include "resource_manager.h"
#include <QDebug>

word_frame::word_frame(QWidget* parent): QFrame(parent)
{
    hb =new QHBoxLayout(this);
    sound_btn = new QPushButton(this);
    sound_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    word_label = new QLabel(this);
    hb->addWidget(sound_btn);
    hb->addWidget(word_label);
    this->setFixedHeight(100);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

}

void word_frame::set_content(const db::WordInfo &wd, int seq_)
{
    content = wd;
    word_label->setText(content.english + " " + content.chinese);
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    seq = seq_;

    QObject::connect(player, &QMediaPlayer::errorOccurred, [&](){
        qDebug() << "Error: " << player->errorString();
    });

    auto man = resource_manager::getInstance();
    auto path = man->audio_select(content.english);
    player->setSource(path);
    player->setAudioOutput(audioOutput);

    QObject::connect(sound_btn,&QPushButton::clicked,[&](){
        player->setPosition(0);
        player->play();
    });
}

void word_frame::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
      emit set_display_content(content, seq);
    }
}

void word_frame::set_btn_disabled()
{
    sound_btn->setEnabled(false);
}

db::WordInfo word_frame::get_content()
{
    return content;
}



