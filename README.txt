This README file pertains to the Smoldyn source distribution.

This distribution contains the Smoldyn source code, which should be able to compile and run on Mac, Linux, or Windows.  Building uses the CMake build system.  If you don't have CMake installed, you'll need to get it.

Hopefully, you can build as follows.  Change to the cmake directory (which is empty).  Enter "cmake ..", then "make", and then "sudo make install".  If this works, congratulations!

To build with Libmoleculizer (for rule-based modeling), first cd to source/libmoleculizer-1.1.2/cmake.  Enter "cmake.." and "make" (but do NOT enter "make install").  Then cd ../../../cmake to return to the main Smoldyn cmake directory.  Enter "cmake .. -DOPTION_USE_LIBMZR=ON", "make", and "sudo make install".  Note that Libmoleculizer still requires a little final debugging.

To build the utility programs wrl2smol and SmolCrowd, cd to the proper cmake directory: source/wrl2smol/cmake or source/SmolCrowd/cmake.  Enter "cmake..", "make", and "sudo make install".

The following additional things are likely to be particularly useful:

Smoldyn_doc1.pdf - the Smoldyn User Manual

examples - a folder of Smoldyn configuration files

-------- If building and installation failed ------

Most likely, you need some additional libraries, all of which are available on the web.  See the Smoldyn user manual (in particular Smoldyn_doc2.pdf).

If it appeared to compile and install, but it doesn't run, check the following:

Did the code get installed to the correct place?  Check by typing "ls /usr/local/bin" and looking for smoldyn.

Does your computer know where to look for programs?  Type "echo $PATH" to get a list of colon-separated places where the computer looks.  If /usr/local/bin isn't in this list, then you'll need to add it to your profile file.  See the Smoldyn documentation (or look on-line).

Is your system allowing you to run the code?  If you're told that permission was denied for running smoldyn, then your computer might not have realized that Smoldyn is an executable program.  Enter "chmod +x /usr/local/bin/smoldyn"

Feel free to e-mail support@smoldyn.org for assistance.

