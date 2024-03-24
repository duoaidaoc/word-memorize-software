
#include "word_frame.h"

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

void word_frame::set_content(const Word &wd)
{
    content = wd;
    word_label->setText(content.eng + ' ' + content.info);
}





