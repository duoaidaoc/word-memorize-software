#include "newclass_frame.h"
#include "qpainter.h"
#include "qpainterpath.h"
#include "ui_newclass_frame.h"
#include "resource_manager.h"
#include "Image_processing.h"
newclass_frame::newclass_frame(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::newclass_frame)
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

  Tip = new tip();


  QObject::connect(ui->close_btn,&QPushButton::clicked,[&](){
    ui->id_edit->clear();
    this->hide();
  });
  QObject::connect(ui->ok_btn,&QPushButton::clicked,[&](){
    bool ok = true;
    qint64 id = ui->id_edit->text().toLongLong(&ok);
    if(!ok || ui->cue_edit->text() == "" || ui->name_edit->text() == ""){
      Tip->set_content("warning","班级创建失败\nid必须为数字\n或暗号和班级名不能为空");
      Tip->show();
      ui->id_edit->clear();
      return;
    }
    QString name = ui->name_edit->text(),cue = ui->cue_edit->text();
    auto man = resource_manager::getInstance();
    auto &teacher = man->get_teacher();
    auto a = teacher.createClass(id,name,cue);

    if(a.isNull()){
      Tip->set_content("warning","id或暗号已存在");
      Tip->show();
    }
    else{
      Tip->set_content("","创建成功");
      Tip->show();
      emit UpdateClass();
    }
    ui->id_edit->clear();
    ui->cue_edit->clear();
    ui->name_edit->clear();
    this->hide();
  });
}

newclass_frame::~newclass_frame()
{
  delete ui;
}

void newclass_frame::paintEvent(QPaintEvent *evt)
{
  QPainter painter(this);
  QPainterPath clipPath;
  clipPath.addRoundedRect(rect(), 20, 20);
  painter.setClipPath(clipPath);
  painter.drawImage(rect(), bg);

  QWidget::paintEvent(evt);
}
