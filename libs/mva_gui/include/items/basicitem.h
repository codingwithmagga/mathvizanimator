#ifndef BASICITEM_H
#define BASICITEM_H

#include <QObject>
#include <QQmlEngine>
#include <QQuickItem>
#include <QString>

class AbstractItem;

class BasicItem : public QQuickItem {

    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool is_initialized MEMBER m_is_initialized NOTIFY isInitializedChanged)
    Q_PROPERTY(AbstractItem* abstract_item READ abstractItem WRITE setAbstractItem NOTIFY abstractItemChanged REQUIRED)

  public:
    BasicItem(QQuickItem* parent = nullptr);

    inline AbstractItem* abstractItem() const { return m_abstract_item; }
    void setAbstractItem(AbstractItem* const new_item);

    QString abstractItemName() const;

  signals:
    void clicked(const QString& name);
    void animationAdded(const QString& name, const QString& type, const qreal start_time, const qreal duration);

    void isInitializedChanged();
    void abstractItemChanged();

  private:
    AbstractItem* m_abstract_item = nullptr;
    bool m_is_initialized = false;
};

#endif // BASICITEM_H
