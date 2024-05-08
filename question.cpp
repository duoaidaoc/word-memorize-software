#include <QEvent>
#include <QPainter>
#include <QPainterpath>

#include "question.h"
#include "ui_question.h"
#include "resource_manager.h"
#include "Image_processing.h"
question::question(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::question)
{
  ui->setupUi(this);

  auto man = resource_manager::getInstance();

  this->setAttribute(Qt::WA_TranslucentBackground,true);
  this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
  this->setProperty("canMove",true);
  QFont font;
  font.setPointSize(20);
  ui->eng_edit->setFont(font);

  bg = new QImage(man->bg_pic_randomselect());
  ImageProcesser::GaussiamBlur(20, 20, *bg);

  QObject::connect(ui->close_btn,&QPushButton::clicked,[&](){
    ui->eng_edit->clear();
    this->hide();
  });
  QObject::connect(ui->close_btn_2,&QPushButton::clicked,[&](){
    this->hide();
  });
  QObject::connect(ui->eng_edit,&QLineEdit::editingFinished,[&](){
    if(ui->eng_edit->text() != ""){

    }
  });


}

question::~question()
{
  delete ui;
}

void question::set_selection_question(const QString &english, const QString &A, const QString &B, const QString &C, int _correctitem)
{
  ui->eng_label->setText(english);
  ui->A_btn->setText(QString("A: ") + A);
  ui->B_btn->setText(QString("B: ") + B);
  ui->C_btn->setText(QString("C: ") + C);
  correctitem = _correctitem;
  ui->stackedWidget->setCurrentIndex(1);
}

void question::set_fillword_question(const QString &english, const QString &chinese)
{
  ui->chn_label->setText(chinese);
  correct_english = english;
  ui->stackedWidget->setCurrentIndex(0);
}

void question::paintEvent(QPaintEvent *evt)
{
  QPainter painter(this);
  QPainterPath path;
  path.addRoundedRect(rect(),20,20);
  painter.setClipPath(path);
  painter.drawImage(rect(), *bg);

  QWidget::paintEvent(evt);
}

void question::set_connection()
{
  QObject::connect(ui->A_btn,&QPushButton::clicked,[&](){
    emit done(correctitem == 1);
  });
  QObject::connect(ui->B_btn,&QPushButton::clicked,[&](){
    emit done(correctitem == 2);
  });
  QObject::connect(ui->B_btn,&QPushButton::clicked,[&](){
    emit done(correctitem == 3);
  });
  QObject::connect(ui->eng_edit,&QLineEdit::editingFinished,[&](){
    if(ui->eng_edit->text() != ""){
      emit done(ui->eng_edit->text() == correct_english);
    }
  });

}
