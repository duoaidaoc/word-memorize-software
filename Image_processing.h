
#ifndef IMAGE_PROCESSING_H
  #define IMAGE_PROCESSING_H

#include "qbitmap.h"
#include "qimage.h"
#include "qmath.h"
#include "qpainter.h"
#include "qpainterpath.h"
#include "qpixmap.h"
class ImageProcesser{
public:
    //参数说明：模糊半径r，方差variance，QImage图像
    static void GaussiamBlur(int r, qreal variance, QImage& img){
      if(variance < 0)
      {
        throw "variance must be greater than 0";
      }
      if(r == 0)
      {
        return;
      }

      //lambda函数说明：x：模糊半径，o：方差，返回：1维高斯函数的运算结果
      auto Gaussian1D = [](qreal x, qreal variance_)->qreal{return (qExp(-(qPow(x,2)/(2*qPow(variance_,2)))))/(variance_*2*M_PI);};

      int imgw = img.width();
      int imgh = img.height();

      int kernelSize = 2 * r + 1;
      int twor = 2*r;
      QVector<qreal> weight(kernelSize);
      qreal weightsum = 0.0;

      //生成高斯核
      for (int i = 0; i <= twor; i++)
      {
        weight[i] = Gaussian1D(i-r, variance);
        weightsum += weight[i];
      }
      //权重和归一化
      for (auto& i : weight)
      {
        i /= weightsum;
      }

      //这部分使用快速高斯模糊算法：用一维高斯函数处理两次，降低时间复杂度
      //边缘处理忽略，但在一个方向上至少会处理一次
      //使用滑动窗口算法

      //横向
      for (int y = 0; y < imgh; y++)
      {
        QVector<qreal> tempred(imgw);
        QVector<qreal> tempgreen(imgw);
        QVector<qreal> tempblue(imgw);
        qreal redsum = 0;
        qreal greensum = 0;
        qreal bluesum = 0;

        for (int x = -r; x < imgw + r; x++)
        {
          int xx = (x + imgw) % imgw;
          QColor color(img.pixel(xx, y));
          int q = xx % kernelSize;
          tempred[xx] = color.red() * weight[q];
          tempgreen[xx] = color.green() * weight[q];
          tempblue[xx] = color.blue() * weight[q];

          redsum += tempred[xx];
          greensum += tempgreen[xx];
          bluesum += tempblue[xx];

          if (x >= r)
          {
            int delx = (x - twor + imgw) % imgw;
            img.setPixel(x - r, y, qRgb(redsum, greensum, bluesum));
            redsum -= tempred[delx];
            greensum -= tempgreen[delx];
            bluesum -= tempblue[delx];
          }
        }
      }
      //纵向
      for (int x = 0; x < imgw; x++)
      {
        QVector<qreal> tempred(imgh);
        QVector<qreal> tempgreen(imgh);
        QVector<qreal> tempblue(imgh);
        qreal redsum = 0;
        qreal greensum = 0;
        qreal bluesum = 0;

        for (int y = -r; y < imgh + r; y++)
        {
          int yy = (y + imgh) % imgh;
          QColor color(img.pixel(x, yy));
          int q = yy % kernelSize;
          tempred[yy] = color.red() * weight[q];
          tempgreen[yy] = color.green() * weight[q];
          tempblue[yy] = color.blue() * weight[q];

          redsum += tempred[yy];
          greensum += tempgreen[yy];
          bluesum += tempblue[yy];

          if (y >= r)
          {
            int dely = (y - twor + imgh) % imgh;
            img.setPixel(x, y - r, qRgb(redsum, greensum, bluesum));
            redsum -= tempred[dely];
            greensum -= tempgreen[dely];
            bluesum -= tempblue[dely];
          }
        }
      }
    }
    static QPixmap pixmapToCircle(const QPixmap &pixmap) {
      // 获取 QPixmap 的大小
      QSize size = pixmap.size();

      // 创建一个新的 QPixmap 用于存放圆形图像
      QPixmap circlePixmap(size);
      circlePixmap.fill(Qt::transparent); // 填充为透明

      // 创建一个 QPainter 用于绘制圆形图像
      QPainter painter(&circlePixmap);

      // 函数接受一个矩形区域并返回一个圆形区域的副本
      auto createCircleMask = [](const QSize &size) -> QBitmap {
        QBitmap mask(size);
        mask.fill(Qt::color0); // 全部填充为透明

        QPainter painter(&mask);
        painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿
        painter.setBrush(Qt::color1); // 非透明颜色
        painter.setPen(Qt::NoPen); // 没有边框

        // 绘制圆形
        painter.drawEllipse(mask.rect());

        return mask;
      };

      // 创建一个圆形的裁剪区域
      QBitmap mask = createCircleMask(size);
      painter.setClipRegion(QRegion(mask));

      // 绘制原始 QPixmap
      painter.drawPixmap(circlePixmap.rect(), pixmap);

      return circlePixmap;
    }


};

#endif // IMAGE_PROCESSING_H
