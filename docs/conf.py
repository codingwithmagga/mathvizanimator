# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# http://www.sphinx-doc.org/en/master/config

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))

import subprocess, os
from pathlib import Path, PurePosixPath

def configureDoxyfile(input_dir, output_dir):

	with open('Doxyfile.in', 'r') as file :
		filedata = file.read()

	filedata = filedata.replace('@DOXYGEN_INPUT_DIR@', input_dir)
	filedata = filedata.replace('@DOXYGEN_OUTPUT_DIR@', output_dir)
	filedata = filedata.replace('@DOXYGEN_GENERATE_TAGFILE@', output_dir + '/html/tagfile.xml')

	with open('Doxyfile', 'w') as file:
		file.write(filedata)

# Check if we're running on Read the Docs' servers
read_the_docs_build = os.environ.get('READTHEDOCS', None) == 'True'

breathe_projects = {}
if read_the_docs_build:
    read_the_docs_build_folder = Path(os.environ.get('READTHEDOCS_OUTPUT', None))
    main_folder = read_the_docs_build_folder.parent.absolute()
    input_dir = '../libs'
    output_dir = main_folder / 'docs' / 'mathvizanimator'
    configureDoxyfile(input_dir, output_dir.absolute().as_posix())
    subprocess.call('doxygen', shell=False)
    subprocess.call(['doxysphinx', 'build', '.', read_the_docs_build_folder, output_dir / 'html'], shell=False)
    breathe_projects['MathVizAnimator'] = output_dir / 'xml'


# -- Project information -----------------------------------------------------

project = 'MathVizAnimator'
copyright = '2023, CodingWithMagga'
author = 'CodingWithMagga'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
#...

extensions = ["breathe", 
              "sphinx.ext.graphviz", 
              "sphinx.ext.autosectionlabel",
              "sphinxcontrib.doxylink"]

# Configure extension
autosectionlabel_prefix_document = True

if read_the_docs_build:
    read_the_docs_build_folder = PurePosixPath(os.environ.get('READTHEDOCS_OUTPUT', None))
    main_folder = read_the_docs_build_folder.parent.absolute()
    output_dir = main_folder / 'docs' / 'mathvizanimator'
    doxygen_root = output_dir.relative_to(read_the_docs_build_folder, walk_up=True).as_posix()
    doxylink = {
        "mva": ( 
            f"{doxygen_root}/html/tagfile.xml", 
            f"{doxygen_root}/html"
		)
	}
else:
    doxygen_root = "mathvizanimator" # this is just a convenience variable
    doxylink = {
		"mva": ( 
			f"{doxygen_root}/html/tagfile.xml", # the first parameter of this tuple is the tagfile
			f"{doxygen_root}/html", # the second parameter of this tuple is a relative path pointing from
										# sphinx output directory to the doxygen output folder inside the output
										# directory tree.
										# Doxylink will use the tagfile to get the html file name of the symbol you want
										# to link and then prefix it with this path to generate html links (<a>-tags).
		),
	}
# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = "sphinx_rtd_theme"

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = []

# Breathe Configuration
breathe_default_project = "MathVizAnimator"

# For code highlighting
pygments_style = 'sphinx'
