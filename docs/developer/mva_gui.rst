MVA GUI library
===============

Items
+++++

Items are objects which the user can add to his video. They are all derived from the :mva:`AbstractItem` class, which itself is derived from `QQuickPaintedItem <https://doc.qt.io/qt-6/qquickpainteditem.html>`_.

.. graphviz::

    digraph G {
        QQuickPaintedItem -> AbstractItem;
        AbstractItem -> GeometryItem;
        AbstractItem -> TextItem;
        GeometryItem -> CircleItem;
        GeometryItem -> RectangleItem;
    }

The :mva:`AbstractItem` class is the base of all items and contains the :mva:`AbstractItem::paintItem` function, which paints the item for the renderer. It calls the pure virtual function `QQuickPaintedItem::paint() <https://doc.qt.io/qt-6.5/qquickpainteditem.html#paint>`_ which has to be implemented in every derived item class, like :mva:`CircleItem` or :mva:`RectangleItem`. Using the Qt class `QPainter <https://doc.qt.io/qt-6.5/qpainter.html>`_ nearly everything can be drawn easily. 

Since a `QQuickPaintedItem <https://doc.qt.io/qt-6/qquickpainteditem.html>`_ can't be used as root object in a qml file, every item has a :mva:`BasicItem` parent object, which is derived from `QQuickItem <.https://doc.qt.io/qt-6.5/QQuickItem.html>`_. A qml file can now looks like this, where :mva:`CircleItem` is a class derived from :mva:`AbstractItem`:

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

It is important to set :mva:`BasicItem::abstract_item` property in the :mva:`BasicItem` element to the id of the containing :mva:`AbstractItem` element. Also note the MVAMouseArea which is necessary for selecting an item. 

Animations
++++++++++

Animations can be added by the user to items, specifying the starting time and duration. All animations are all derived from the :mva:`AbstractAnimation` class, which itself is derived from `QObject <https://doc.qt.io/qt-6/qobject.html>`_.

.. graphviz::

    digraph G {
        QObject -> AbstractAnimation;
        AbstractAnimation -> PropertyAnimation;
        PropertyAnimation -> RealPropertyAnimation;
        RealPropertyAnimation -> FadeIn;
        RealPropertyAnimation -> FadeOut;
    }

Derived from the :mva:`AbstractAnimation` class is the :mva:`PropertyAnimation` class, which is the base class for animations which are based on item properties. The :mva:`RealPropertyAnimation` class, derived from :mva:`PropertyAnimation`, is the base class for real property based animations like :mva:`FadeIn` and :mva:`FadeOut`. Both animations are changing the opacity of an item from 0 to 1 or reverse.