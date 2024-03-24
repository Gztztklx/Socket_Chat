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
    virtual QList<QVideoFrame::PixelFormat>supportedPixelFormats(QAbstractVideoBuffer::HandleType type = QAbstractVideoBuffer::NoHandle) const;
    
    virtual bool present(const QVideoFrame &frame);
signals:
    void sndImage(QImage);
};

#endif // ABSTRACTVIDEOSURFACE_H
