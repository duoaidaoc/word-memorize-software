
#ifndef ACRYLICFRAME_H
  #define ACRYLICFRAME_H

  #include <QFrame>



class AcrylicFrame : public QFrame
{
  Q_OBJECT
public:
  AcrylicFrame(QWidget *parent = nullptr);
protected:
  void paintEvent(QPaintEvent* event) override;
private:
  QPixmap screen_shot();


  QColor tintColor;
  QColor luminosityColor;
  float noiseOpacity;
  QImage bgiamge;
  QPixmap bgpixmap;
  QImage noiseImage;
};

#endif // ACRYLICFRAME_H
