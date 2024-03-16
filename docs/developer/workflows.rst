Workflows
=========

On this page you will find some general notes how specific workflows are handled in the code.

.. contents:: Table of Contents
   :depth: 1
   :local:

Pointer handling
++++++++++++++++

Since the Qt library is used in this project, the usage of pointers cannot be prevented. For example when adding an item to the scene, see also :ref:`Adding an item`, the generated item object is a **BasicItem** Pointer containing an **AbstractItem** raw pointer as child. The **BasicItem** object itself is a child of the qml drop area. The `parent-child relationship in Qt <https://doc.qt.io/qt-6.5/objecttrees.html>`_ is also responsible for the memory management, so it is also not a good idea, to convert this raw pointers into smart pointers.

Besides the special cases explained above, smart pointers should be used when pointer usage is necessary. 

Adding an item
++++++++++++++

An item can be added by the user by drag and drop from the left list of available items. When the dragging process started, a small *shadow* item is created which follows the mouse cursor. The implementation of this process can be seen in **MVAItemDelegate.qml** file in the :ref:`MVA GUI library`. This *shadow* item will be destroyed when the item is dropped. Only the project area in the middle of the application accepts the drop and creates the item based on the items qml file, which is stored in the ListElement in **MVAItemModel.qml** file. The itemAdded signal is emitted, which is connected to **ItemHandler** object in the **MainLogic** class. Here the pointer to the item will be stored and added to the item model.

Deleting an item 
++++++++++++++++

The user request to delete an item is also sended to the **ItemHandler** class. Here the item needs to be deleted manually, otherwise it wont disappear from the project area in the middle of the application. It needs to be ensured that the item is not deleted manually, when the application is shut down. How this is done, can be seen in the **ItemModelItem** class definition in the file **itemhandler.h**.

Adding an animation
+++++++++++++++++++

An animation can be added by the user by right clicking on the specific item and choosing "Add animation" in the menu. The **BasicItem** class emits the signal **animationAdded** when the user adds an animation. The animation is then stored in the **ItemObserver** class of the corresponding item. The **ItemHandler** object in the **MainLogic** class is responsible for this action.

Deleting an animation 
+++++++++++++++++++++

An animation can be deleted by the user by right clicking the animation in the animation list on the bottom right. The user request will be forwarded to the **ItemHandler** object in the **MainLogic** class, which will delete the animation.

Render and Snapshot process
+++++++++++++++++++++++++++

Have a look at :ref:`Interface to FFmpeg`. 