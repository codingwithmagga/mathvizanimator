Project Structure
=================

The project is structured in multiple libraries and an application executable using CMake. The following image should give you an overview about how the project is structured in general:

.. graphviz::

    digraph G {
        compound = true
        subgraph "cluster_mva_svg" {
            label = "MVA SVG";
            LaTeXProcess -> SVGCreator
            DvisvgmProcess -> SVGCreator    
            SVGConfig 
        }
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
        SVGCreator -> AbstractItem [ltail=cluster_mva_gui_items,style=dotted];
    }

There are three libraries :ref:`developer/mva_svg:MVA SVG library`, :ref:`developer/mva_gui:MVA GUI library` and the :ref:`developer/mva_workflow:MVA Workflow library` located in the *lib* folder of the project. The :ref:`developer/mva_svg:MVA SVG library` is responsible for the SVG creation process from LaTeX files and later should also be used for analyzing and manipulating SVG files. Have a look at :ref:`developer/mva_svg:MVA SVG library` for more information.

The :ref:`developer/mva_gui:MVA GUI library` library contains classes for the different :ref:`developer/mva_gui:Items`, like a rectangle or a circle, and :ref:`developer/mva_gui:animations`, like :mva:`FadeIn` and :mva:`FadeOut`, which the user can add to the project. Besides that other, small **qml files** are located here for the user interface. In the **MainWindow.qml** file the main application window is located. Here the whole user interface is defined. The :mva:`MainWindowHandler` class serves as wrapper between the **MainWindow.qml** and the :mva:`MainLogic` class. Data from and to the qml based main window file is processed (and maybe converted) here. 

The :ref:`developer/mva_workflow:MVA Workflow library` library contains the logic of the application and no direct GUI elements. The :mva:`ItemObserver` class stores pointers to the created items and animations from :ref:`developer/mva_gui:MVA GUI library` library and is responsible for the current state of the items. The :mva:`ItemHandler` class manages the :mva:`ItemObserver` objects and contains the models used in the list and tables in the GUI. The :mva:`Renderer` class is a small wrapper class for the **FFmpeg** library and provides an interface to create videos and snapshots. The :mva:`SaveFileHandler` class is a small class to handle the save files of the application which are stored in the JSON-format.

The :mva:`MainLogic` connects the GUI part (:mva:`MainWindowHandler`) with the workflow classes (:mva:`ItemHandler`, :mva:`Renderer`, :mva:`SaveFileHandler`). It ensures the connection and data transfer between these two parts and is responsible that the necessary Information are up-to-date in every class. The main.cpp file just creates the application and starts it. As usual in Qt based Applications it doesn't do that much more.
