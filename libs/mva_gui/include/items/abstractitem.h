#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

#include <QColor>
#include <QFile>
#include <QJsonObject>
#include <QVector>
#include <QtQuick/QQuickPaintedItem>

class AbstractItem : public QQuickPaintedItem {
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QString file MEMBER m_qml_file CONSTANT)

public:
    struct EditableProperties {
        QStringList abstract_item_properties;
        QStringList quick_item_properties;
    };

    AbstractItem(const QString& qml_file, QQuickItem* parent = nullptr);

    QString name() const;
    void setName(const QString& name);

    QColor color() const;
    void setColor(const QColor& color);

    virtual QJsonObject toJson() const;
    virtual void setPropertiesFromJson(const QJsonObject& json);

    virtual EditableProperties editableProperties() const;

signals:
    void nameChanged(const QString& new_name);
    void colorChanged(const QColor& new_color);

private:
    QString m_name;
    QColor m_color;

    QString m_qml_file;
};

Q_DECLARE_METATYPE(AbstractItem) // GCOVR_EXCL_LINE

#endif // ABSTRACTITEM_H
