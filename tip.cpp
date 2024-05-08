#include "tip.h"
#include "qpainter.h"
#include "qpainterpath.h"
#include "resource_manager.h"
#include "Image_processing.h"
#include "ui_tip.h"

tip::tip(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::tip)
{
  ui->setupUi(this);
  this->setProperty("canMove",true);
  this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
  this->setAttribute(Qt::WA_TranslucentBackground, true);
  auto man = resource_manager::getInstance();
  bg.load(man->bg_pic_randomselect());
  ImageProcesser::GaussiamBlur(20,20, bg);
  ui->label->setStyleSheet("background-color: rgba(128,128,128,0.5);border-radius: 10px");
  QObject::connect(ui->pushButton,&QPushButton::clicked,[&](){
    emit confirm();
    this->hide();
  });
  QObject::connect(ui->pushButton_2,&QPushButton::clicked,[&](){
    emit cancel();
    this->hide();
  });
}

tip::~tip()
{
  delete ui;
}

void tip::set_content(const QString &title, const QString &content, const QString &Le, const QString &Ri)
{
  ui->label->setText(QString("tip:") + title);
  ui->label_2->setText(content);
  ui->pushButton->setText(Ri);
  ui->pushButton_2->setText(Le);
}

void tip::paintEvent(QPaintEvent *evt)
{
  QPainter painter(this);
  QPainterPath clipPath;
  clipPath.addRoundedRect(rect(), 10, 10);
  painter.setClipPath(clipPath);
  painter.drawImage(rect(), bg);
  QWidget::paintEvent(evt);
}
