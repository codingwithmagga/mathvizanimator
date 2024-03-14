Project Structure
=================

The project is structured in multiple libraries and an application executable using CMake. The following image should give you an overview about how the project is structured in general:

.. graphviz::

    digraph G {
        compound = true
        subgraph "cluster_mva_gui" {
            "MainWindow.qml" -> MainWindowHandler;
            "QML files" -> "MainWindow.qml"
            label = "MVA GUI";
            subgraph "cluster_mva_gui_items" {
                " ..." [style=dotted];
                AbstractItem -> " ..."
                label = "Items (QML + cpp)";
            }
            subgraph "cluster_mva_gui_animations" {
                "..." [style=dotted];
                AbstractAnimation -> "...";
                label = "Animations";
            }
        }
        subgraph "cluster_mva_app" {
            MainLogic -> "main.cpp";
            label = "MVA Application";
        }
        subgraph "cluster_mva_workflow" {
            ItemObserver -> ItemHandler; 
            Renderer;
            SaveFileHandler; 
            label = "MVA Workflow";
        }
        MainWindowHandler -> MainLogic;
        Renderer -> MainLogic [ltail=cluster_mva_workflow];
        AbstractItem -> "MainWindow.qml" [ltail=cluster_mva_gui_items];
        AbstractItem -> ItemObserver [ltail=cluster_mva_gui_items,style=dotted];
        AbstractAnimation -> ItemObserver [ltail=cluster_mva_gui_animations,style=dotted];
    }

There are two libraries "MVA GUI" and "MVA Workflow" located in the *lib* folder of the project. The "MVA GUI" library contains classes for the different :ref:`Items`, like a rectangle or a circle, and :ref:`animations`, like FadeIn and FadeOut, which the user can add to the project. Besides that other, small **qml files** are located here for the user interface. In the **MainWindow.qml** file the main application window is located. Here the whole user interface is defined. The **MainWindowHandler** class serves as wrapper between the **MainWindow.qml** and the **MainLogic** class. Data from and to the qml based main window file is processed (and maybe converted) here. 

The "MVA Workflow" library contains the logic of the application and no direct GUI elements. The **ItemObserver** class stores pointers to the created items and animations from "MVA GUI" library and is responsible for the current state of the items. The **ItemHandler** class manages the **ItemObserver** objects and contains the models used in the list and tables in the GUI. The **Renderer** class is a small wrapper class for the **ffmpeg** library and provides an interface to create videos and snapshots. The **SaveFileHandler** is a small class to handle the save files of the application which are stored in the JSON-format.

The **MainLogic** connects the GUI part (**MainWindowHandler**) with the workflow classes (**ItemHandler**, **Renderer**, **SaveFileHandler**). It ensures the connection and data transfer between these two parts and is responsible that the necessary Information are up-to-date in every class. The **main.cpp** file just creates the application and starts it. As usual in Qt based Applications it doesn't do that much more.
