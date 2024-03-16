Workflows
=========

On this page you will find some general notes how specific workflows are handled in the code.

Pointer handling
++++++++++++++++

Since the Qt library is used in this project, the usage of pointers cannot be prevented. For example when adding an item to the scene, see also :ref:`Adding an item`, the generated item object is a **BasicItem** Pointer containing an **AbstractItem** raw pointer as child. The **BasicItem** object itself is a child of the qml drop area. The `parent-child relationship in Qt <https://doc.qt.io/qt-6.5/objecttrees.html>`_ is also responsible for the memory management, so it is also not a good idea, to convert this raw pointers into smart pointers.

Besides the special cases explained above, smart pointers should be used when pointer usage is necessary. 

Adding an item
++++++++++++++

Deleting an item 
++++++++++++++++

Adding an animation
+++++++++++++++++++

Deleting an animation 
+++++++++++++++++++++

Render and Snapshot process
+++++++++++++++++++++++++++