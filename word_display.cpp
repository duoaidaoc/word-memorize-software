#include "word_display.h"
#include "qevent.h"
#include "qpainter.h"
#include "ui_word_display.h"
#include "resource_manager.h"
#include <QGraphicsBlurEffect>
#include <QPainterPath>
#include "Image_processing.h"

word_display::word_display(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::word_display)
{
    ui->setupUi(this);
    this->setProperty("canMove",true);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    auto man = resource_manager::getInstance();
    auto path = man->bg_pic_randomselect();
    img = new QImage(path);

    this->setFont(man->get_glob_font());
    ImageProcesser::GaussiamBlur(100,100,*img);
    alpha_animation = new QPropertyAnimation(this, "windowOpacity");
    move_Animation = new QPropertyAnimation(this, "pos");

    nexpos = pos();

    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);

    // 绑定读音按钮和音频链接。
    QObject::connect(ui->sound_btn,&QPushButton::clicked,[&](){
      player->setPosition(0);
      player->play();
    });

    QObject::connect(ui->al_btn,&QPushButton::clicked,[&](){
      emit upd_page(seq - 1);
    });

    QObject::connect(ui->ar_btn,&QPushButton::clicked,[&](){
      emit upd_page(seq + 1);
    });

    QObject::connect(ui->close_btn,&QPushButton::clicked,[&](){
      this->hide();
    });

}

word_display::~word_display()
{
    delete ui;
}

void word_display::set_content(const db::WordInfo &word, int seq_)
{
    wd = word;
    seq = seq_;
    ui->word_label->setText(word.english);
    ui->mean_label->setText(word.chinese);
    ui->page_label->setText(QString::number(seq_ + 1));

    auto man = resource_manager::getInstance();
    auto path = man->audio_select(word.english);
    // TODO(): player未打开文件必须检查
    player->setSource(path);
    player->setAudioOutput(audioOutput);
}

void word_display::enter()
{
    alpha_animation->setDuration(500); // 500 ms
    alpha_animation->setStartValue(0.0); // 完全透明
    alpha_animation->setEndValue(1.0); // 完全不透明
    move_Animation->setDuration(500);
    if(alpha_animation->state() != QAbstractAnimation::Running && move_Animation->state() != QAbstractAnimation::Running){
      move_Animation->setStartValue(QPoint(nexpos.x() - 50, nexpos.y()));
      move_Animation->setEndValue(nexpos);
        alpha_animation->start();
      move_Animation->start();
    }
}

void word_display::set_nexpos(QPoint p)
{
    nexpos = p;
}


void word_display::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPainterPath clipPath;
    clipPath.addRoundedRect(rect(), 20, 20);
    painter.setClipPath(clipPath);
    painter.drawImage(rect(), *img);
}


