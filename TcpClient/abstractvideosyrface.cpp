#include "abstractvideosyrface.h"

AbstractVideoSurface::AbstractVideoSurface(QObject* parent):
    QAbstractVideoSurface(parent)
{
    
}

QList<QVideoFrame::PixelFormat> AbstractVideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const
{
    return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB32;
}

bool AbstractVideoSurface::present(const QVideoFrame &frame)
{
    QVideoFrame fm = frame;
    fm.map(QAbstractVideoBuffer::ReadOnly);
    
    QImage image(fm.bits(),fm.width(),fm.height(),fm.bytesPerLine(),fm.imageFormatFromPixelFormat(fm.pixelFormat()));
    image = image.mirrored(1);
    emit sndImage(image);
    return 0;
}
