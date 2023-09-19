#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

#include <QColor>
#include <QFile>
#include <QJsonObject>
#include <QVector>
#include <QtQuick/QQuickPaintedItem>

class AbstractItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(QString file MEMBER m_qml_file CONSTANT)

public:
    enum class ObjectType { BEZIER, TEXT };

    AbstractItem(const QString &qml_file, QQuickItem *parent = nullptr);

    virtual ObjectType getObjectType() const = 0;

    QString name() const;
    void setName(const QString &name);

    QColor color() const;
    void setColor(const QColor &color);

    virtual QJsonObject toJson() const;
    virtual void setPropertiesFromJson(const QJsonObject &json);

private:
    QString m_name;
    QColor m_color;

    const QString m_qml_file;
};

Q_DECLARE_METATYPE(AbstractItem)

#endif // ABSTRACTITEM_H
