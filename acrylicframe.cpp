
#include "acrylicframe.h"
#include "qpainter.h"
#include "resource_manager.h"

AcrylicFrame::AcrylicFrame(QWidget *parent) : QFrame(parent)
{
  auto man = resource_manager::getInstance();
  noiseImage = QImage(man->get_noise_pic());
  tintColor = man->get_color();
  luminosityColor = {255, 255, 255, 63};
  noiseOpacity = 0.05;
}

void AcrylicFrame::paintEvent(QPaintEvent *event)
{
  auto acrylicTexture = QImage(64, 64, QImage::Format_ARGB32_Premultiplied);
  acrylicTexture.fill(luminosityColor);

  auto painter = QPainter(&acrylicTexture);
  painter.fillRect(acrylicTexture.rect(), tintColor);

  painter.setOpacity(noiseOpacity);
  painter.drawImage(acrylicTexture.rect(), noiseImage);

  auto acrylicBrush = QBrush(acrylicTexture);
  auto painter_2 = QPainter(this);
  painter_2.fillRect(rect(), acrylicBrush);
}
