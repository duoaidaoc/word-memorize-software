
#include "word_frame.h"
#include "resource_manager.h"
#include "student_main.h"
#include <QDebug>

extern word_display* glob_display;

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
    glob_display->set_content(content);
}

void word_frame::set_content(const Word &wd)
{
    content = wd;
    word_label->setText(content.eng + content.info);
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);

    QObject::connect(player, &QMediaPlayer::errorOccurred, [&](){
        qDebug() << "Error: " << player->errorString();
    });

    auto man = resource_manager::getInstance();
    auto path = man->audio_select(content.eng);
    player->setSource(path);
    player->setAudioOutput(audioOutput);

    QObject::connect(sound_btn,&QPushButton::clicked,[&](){
        player->setPosition(0);
        player->play();
        qDebug() << "triggered";
    });
}

void word_frame::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
      glob_display->enter();
      glob_display->show();
      qDebug() << "Huh?";
    }
}




