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

void question::paintEvent(QPaintEvent *evt)
{
  QPainter painter(this);
  QPainterPath path;
  path.addRoundedRect(rect(),20,20);
  painter.setClipPath(path);
  painter.drawImage(rect(), *bg);

  QWidget::paintEvent(evt);
}
