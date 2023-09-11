#include "qmlimageprovider.h"

QmlImageProvider::QmlImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
{}

QImage QmlImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    return image;
}

void QmlImageProvider::updateImage(QImage new_image)
{
    image = new_image;
}
