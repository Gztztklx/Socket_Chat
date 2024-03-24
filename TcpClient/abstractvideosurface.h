#ifndef ABSTRACTVIDEOSURFACE_H
#define ABSTRACTVIDEOSURFACE_H

#include <QWidget>
#include <QVideoFrame>
#include <QAbstractVideoSurface>

class AbstractVideoSurface : public QAbstractVideoSurface
{

    Q_OBJECT
public:
    AbstractVideoSurface(QObject* parent = 0);

    virtual QList<QVideoFrame::PixelFormat>
supportedPixelFormats(QAbstractVideoBuffer::HandleType type = QAbstractVideoBuffer::NoHandle) const;//该方法是当前类的构造函数自动调用的，他是用来设置处理的图像支持哪些图像格式


    virtual bool present(const QVideoFrame &frame);

signals:
    void sndImage(QImage);

};

#endif // ABSTRACTVIDEOSURFACE_H
