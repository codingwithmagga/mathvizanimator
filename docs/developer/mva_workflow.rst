MVA Workflow library
====================

JSON save file
++++++++++++++

The projects are stored in a json file which can be opened (and modified) by any text editor. The items in the project are stored in a json object with the name *item_i*, where *i* is the number of the item. All properties of the item are stored in this object. Note that the BasicItem is stored here with it's width, height, x- and y-position. All properties which belong to the containing AbstractItem object are stored with the prefix *abstract_item*, like *abstract_item.name*. With this approach, the items can easily be created when loading the json file using `QObject* QQmlComponent::createWithInitialProperties(...) <https://doc.qt.io/qt-6.5/qqmlcomponent.html#createWithInitialProperties>`_. If an item has attached animations, they are stored in a property *animation_i* where i is the i-th animation of this item.

There are also some basic information, which are stored in every json save file. These are for example the current version of the project and the project settings, as you can see in this example here:

.. code-block:: json

    "mva-version": "0.0.1",
    "project-settings": {
        "background_color": "#000000",
        "fps": 24,
        "height": 768,
        "video_length": 5,
        "width": 1024
    }

Interface to ffmpeg 
+++++++++++++++++++

The Renderer class contains the interface to ffmpeg or rather the call of the ffmpeg process using the `QProcess <https://doc.qt.io/qt-6.5/qprocess.html>`_ class. ffmpeg is started using the *-rawvideo* flag which allows to send raw images for every video frame to the process. The raw images are created using a QImage together with the QPainter class. The used method (*AbstractItem::paintItem(QPainter\* painter)*) uses the same method (`void QQuickPaintedItem::paint(QPainter *painter) <https://doc.qt.io/qt-6.5/qquickpainteditem.html#paint>`_) which draws the items to the gui.