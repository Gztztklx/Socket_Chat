#include "abstractvideosurface.h"

AbstractVideoSurface::AbstractVideoSurface(QObject* parent):
    QAbstractVideoSurface(parent)
{

}

QList<QVideoFrame::PixelFormat> AbstractVideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const
{
//  返回一个QList<QVideoFrame::PixelFormat>对象，里面存放了所有支持的像素格式
/*
    QList<QVideoFrame::PixelFormat> list;
    list.append(QVideoFrame::Format_RGB32);
    return list;
*/
    return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB32;
}

bool AbstractVideoSurface::present(const QVideoFrame &frame)
{
//  摄像头截取到的每一帧图像都会发送到该函数里来处理
    QVideoFrame fm = frame;
    fm.map(QAbstractVideoBuffer::ReadOnly);
//  由于qt当中能够承载图像的是QLabel，QLabel能够设置图像的只有QPixmap，所以我们现在尽量的要将QVideoFrame转换成QPixmap
/*
    寻找转换方法有2种思路：
        ① 在QPixmap寻找构造函数或者静态方法，参数为QVideoFrame，返回值为QPixmap的
        ② 在QViderFrame里面的公开方法寻找 返回为QPixmap，没有参数，函数名类似为 toPixmap这样的函数
    寻找过后我们发现是没有这样的函数
    但是，我们再QPixmap找到了和QImage的转换方式
        所以，现在就想办法，从QVideoFrame转换成QImage
        QImage构造函数QImage(const uchar *data, int width, int height, int bytesPerLine, Format format)里面的所有参数，我们都可以从QVideFrame当中去获取
*/
    QImage image(fm.bits(),fm.width(),fm.height(),fm.bytesPerLine(),fm.imageFormatFromPixelFormat(fm.pixelFormat()));

    image = image.mirrored(1);
    emit sndImage(image);
    return 0;
}
