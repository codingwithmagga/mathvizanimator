MVA SVG library
===============

The SVG library streamlines the creation of SVG files from LaTeX input. This library simplifies the process of converting LaTeX equations into SVGs, enabling users to visualize mathematical concepts effortlessly.

LaTeX to SVG process
++++++++++++++++++++

Using ``SVGCreator::svgFromLaTeX(const QString& latex)`` starts the process to create a SVG image from a LaTeX file. First a ``latex`` process is started using the following command line arguments:

``latex -output-format=dvi -interaction=batchmode path/to/latex.tex``

* ``-output-format=dvi`` specifies that the latex process outputs a dvi file. 
* ``-interaction=batchmode`` this ensures that the LaTeX compilation process proceeds in batch mode, without requiring any user interaction.
* ``path/to/latex.tex`` Absolute file path to latex file ``latex.tex`` (or a different file name) on the file system.

Then the ``latex`` process was successful ``dvisvgm`` is used to create a svg file from the previously created dvi file, which has the same base name and is in the same place as the previously used LaTeX file: ``path/to/latex.dvi``:

``dvisvgm path/to/latex.dvi -n -o path/to/latex.svg``

* ``path/to/latex.dvi`` Absolute file path to dvi file ``latex.dvi`` (or a different file name) on the file system.
* ``-n``  Glyphs are drawn by using path elements, adjacent text elements are not merged and no CSS styles are used to reference fonts
* ``-o path/to/latex.svg`` Specifies the output file ``latex.svg``

Then the ``dvisvgm`` process was successful the ``SVGCreator::svgCreated(const QFileInfo& svg_file)`` signal is emitted there ``svg_file`` contains the information about the created SVG file.

