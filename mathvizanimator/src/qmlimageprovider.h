#ifndef QMLIMAGEPROVIDER_H
#define QMLIMAGEPROVIDER_H

#include <QQuickImageProvider>

class QmlImageProvider : public QQuickImageProvider
{
public:
    QmlImageProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    void updateImage(QImage new_image);

private:
    QImage image;
};

#endif // QMLIMAGEPROVIDER_H
