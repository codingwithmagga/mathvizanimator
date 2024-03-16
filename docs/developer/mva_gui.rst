MVA GUI library
===============

Items
+++++

Items are objects which the user can add to his video. They are all derived from the **AbstractItem** class, which itself is derived from `QQuickPaintedItem <https://doc.qt.io/qt-6/qquickpainteditem.html>`_.

.. graphviz::

    digraph G {
        QQuickPaintedItem -> AbstractItem;
        AbstractItem -> GeometryItem;
        AbstractItem -> TextItem;
        GeometryItem -> CircleItem;
        GeometryItem -> RectangleItem;
    }

The **AbstractItem** class is the base of all items and contains the *paintItem(QPainter\* painter)* item function, which paints the item for the renderer. It calls the pure virtual function `void QQuickPaintedItem::paint(QPainter *painter) <https://doc.qt.io/qt-6.5/qquickpainteditem.html#paint>`_ which has to be implemented in every derived item class, like **CircleItem** or **RectangleItem**. Using the Qt class `QPainter <https://doc.qt.io/qt-6.5/qpainter.html>`_ nearly everything can be drawn easily. 

Since a `QQuickPaintedItem <https://doc.qt.io/qt-6/qquickpainteditem.html>`_ can't be used as root object in a qml file, every item has a  **BasicItem** parent object, which is derived from `QQuickItem <.https://doc.qt.io/qt-6.5/QQuickItem.html>`_. A qml file can now looks like this, where CircleItem is a class derived from **AbstractItem**:

.. code-block:: qml

    import QtQuick

    import cwa.mva.gui 

    BasicItem {
        id: rootMVACircle

        abstract_item: circleItem

        CircleItem {
            id: circleItem

            name: "circle"
            borderColor: "blue"

            anchors.fill: parent

            MVAMouseArea {
                anchors.fill: parent
                basicItem: rootMVACircle

                onLeftClicked: {
                    rootMVACircle.clicked(circleItem.name)
                }

                onAnimationAdded: (type, start_time, duration) => {
                                    rootMVACircle.animationAdded(circleItem.name,
                                                                type,
                                                                start_time,
                                                                duration)
                                }
            }
        }
    }

It is important to set abstract_item property in the BasicItem element to the id of the containing **AbstractItem** element. Also note the MVAMouseArea which is necessary for selecting an item. 

Animations
++++++++++

Explain AbstractAnimation interface

.. graphviz::

    digraph G {
        QObject -> AbstractAnimation;
        AbstractAnimation -> PropertyAnimation;
        PropertyAnimation -> RealPropertyAnimation;
        RealPropertyAnimation -> FadeIn;
        RealPropertyAnimation -> FadeOut;
    }