from distutils.core import setup

setup(name="moleculizer",
      version="1.0",
      description = "Moleculizer library for generating xml files from rules files.",
      author = "Nathan Addy",
      author_email = "nathan.addy@gmail.com",

      packages = [ "moleculizer" ],
      scripts = ["mzrrulestoxmlconverter.py"]

      )
      
