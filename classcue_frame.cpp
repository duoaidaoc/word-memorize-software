#include <QPainter>
#include <QPainterPath>

#include "classcue_frame.h"
#include "ui_classcue_frame.h"
#include "resource_manager.h"
#include "Image_processing.h"

classcue_frame::classcue_frame(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::classcue_frame)
{
  ui->setupUi(this);
  this->setProperty("canMove",true);
  this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
  this->setAttribute(Qt::WA_TranslucentBackground, true);

  auto man = resource_manager::getInstance();
  bg.load(man->bg_pic_randomselect());
  if(bg.isNull()){
    throw std::runtime_error("background of class_display is not availble.");
  }
  ImageProcesser::GaussiamBlur(20, 20, bg);
  bg.save("bg.png");
  QObject::connect(ui->close_btn,&QPushButton::clicked,[&](){
    ui->lineEdit->clear();
    this->hide();
  });
  QObject::connect(ui->lineEdit,&QLineEdit::editingFinished,[&](){
    if(ui->lineEdit->text() != ""){
      qDebug()<<"ok";
      /*
        查询班级是否存在并且加入班级。
      */
      /*
        若存在，更新班级显示窗口的内容。
      */
      /*
        不存在，弹出班级不存在的错误信息。
      */
    }
  });

}

classcue_frame::~classcue_frame()
{
  delete ui;
}

void classcue_frame::paintEvent(QPaintEvent *evt)
{
  QPainter painter(this);
  QPainterPath clipPath;
  clipPath.addRoundedRect(rect(), 20, 20);
  painter.setClipPath(clipPath);
  painter.drawImage(rect(), bg);
  QWidget::paintEvent(evt);
}
