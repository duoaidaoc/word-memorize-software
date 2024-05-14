#include "settings.h"
#include "qpainter.h"
#include "qpainterpath.h"
#include "ui_settings.h"
#include <QFileDialog>

#include "resource_manager.h"
#include "Image_processing.h"

Settings::Settings(QWidget *parent)
  : QWidget(parent)
  , ui(new Ui::Settings)
{
  ui->setupUi(this);

  setup_ui();
  setup_connection();
}

Settings::~Settings()
{
  delete ui;
}

void Settings::get_info(const detailed_info& info)
{
  info_ = info;
  reset_info();
}

void Settings::paintEvent(QPaintEvent *evt)
{
  QPainter painter(this);
  QPainterPath path;

  this->setStyleSheet("QLineEdit{"
                      "   background-color: rgba(255,255,255,0.6);"
                      "}");

  path.addRoundedRect(rect(), 20, 20);
  painter.setClipPath(path);
  painter.drawImage(rect(),*bg);

  QWidget::paintEvent(evt);
}

void Settings::setup_ui()
{
  this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
  this->setAttribute(Qt::WA_TranslucentBackground, true);
  this->setProperty("canMove",true);

  auto man = resource_manager::getInstance();
  QString back_ground_path = man->bg_pic_randomselect();
  bg = new QImage;
  bg->load(back_ground_path);
  ImageProcesser::GaussiamBlur(20 ,20 ,*bg);

  loaded_img = new QImage;
}

void Settings::setup_connection()
{
  QObject::connect(ui->settings_exit_btn,&QPushButton::clicked, [&](){
    hide();
  });
  QObject::connect(ui->settings_check_btn,&QPushButton::clicked, [&](){
    detailed_info info = {
        ui->settings_name_edit->text(),
        ui->settings_age_edit->text(),
        ui->settings_school_edit->text(),
        ui->settings_phone_edit->text(),
        ui->settings_say_edit->toPlainText(),
        loaded_img
    };
    emit set_outer_info(info);
    hide();
  });
  QObject::connect(ui->settings_reload_btn,&QPushButton::clicked, [&](){
    reset_info();
  });
  QObject::connect(ui->settings_upload_btn,&QPushButton::clicked, [&](){
    QString filter = "Image Files (*.png *.jpg *.img);;All Files (*.*)";
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", filter);

    // 从文件加载图片
    if (!loaded_img->load(fileName)) {
      qDebug() << "无法加载图片：" << fileName;
      return;
    }
    // 为label设置图片
    ui->settings_pic_label->setPixmap(QPixmap::fromImage(*loaded_img));

  });
}

void Settings::reset_info()
{
  ui->settings_name_edit->setText(info_.nickname);
  ui->settings_age_edit->setText(info_.age);
  ui->settings_school_edit->setText(info_.school);
  ui->settings_phone_edit->setText(info_.phone);
  ui->settings_say_edit->setText(info_.say);
  if(info_.img == nullptr || info_.img->isNull())
    return;
  ui->settings_pic_label->setPixmap(QPixmap::fromImage(*info_.img));
}
