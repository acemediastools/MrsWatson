Building MrsWatson from Source
==============================

To build MrsWatson on any platform, you'll need CMake (version 3.0 or greater)
to configure the project. The MrsWatson source code also contains some
submodules for third-party libraries, so before building you should run the
following commands:

    git submodule sync
    git submodule update --init --recursive

If you download the MrsWatson sources from the automatically-generated
zipfile/tarball that GitHub produces for each release, then the third-party code
will not be included. Currently it is not recommended to use the GitHub source
releases, but rather to clone the sources and build from the release tag.


CMake Build Options
-------------------

The following options can be passed to CMake to configure the build. Note that
these arguments must be prefixed with `-D` to CMake, eg `-DOPTION_NAME=ON`.

* `WITH_AUDIOFILE`: Use libaudiofile for reading/writing audio files (default:
  `ON`)
* `WITH_FLAC`: Support for FLAC files (requires libaudiofile, default: `OFF`)
* `WITH_VST_SDK`: Manually specify VST SDK zipfile location instead of
  downloading it (useful for configuring when offline, no default value)
* `VERBOSE`: Show extra build information (default: `OFF`)


Mac OSX
-------

To build MrsWatson on Mac OS X, you will need to have Apple's Developer Tools
installed. You can download Xcode either from Apple's developer website or from
the Mac App Store. You also need to install the command line tools provided by
Xcode, which are not installed by default. To do this, go to the Xcode
preferences, then to the downloads tab, and in "Components" you find a button to
install the command line tools.

[Homebrew][homebrew] is also recommended for building MrsWatson on Mac OS X.
from homebrew, you will need the CMake package, but possibly automake and
autoconf as well. See the homebrew webpage for details on installation of
packages.

If you want to use Xcode to develop MrsWatson you will need to generate a
project file like so:

    cmake -G Xcode .

This command will generate an Xcode project in the current directory which you
can use to compile the software as you would any other project. For other IDE's,
such as CLion, QT Creator, NetBeans, emacs, or vim, you can generate a regular
makefile to build the software like any other Unix package. To do that you must
generate the makefiles like so:

    cmake -G "Unix Makefiles" .
    make

Ninja and CCache are also supported, both tools should work "out of the box".


Windows
-------

On Windows, Visual Studio is used to compile the software. You will need at
least Visual Studio 2013 (aka Visual Studio 12) to build MrsWatson, since
earlier versions do not support C99.

The MrsWatson source code does not include a Visual Studio project, instead
CMake is used to generate Visual Studio project which can then be used to build
the software. After you've installed [CMake for Windows][cmake], you must
generate the build directory where the Visual Studio product will be placed.
Unlike on Unix, you cannot build the software from the same directory as the
source code.  So generating the Visual Studio project will look something like
this:

    cd C:\wherever
    mkdir build
    cd build
    cp 
    cmake -G "Visual Studio 14 2015" C:\path\to\MrsWatson

    
IF VST LIBRARY DOES NOT EXIST IN YOUR SOURCE PATH

    cd C:\path\to\MrsWatson\source\plugin & copy "C:\wherever\build\VST_SDK\vst2sdk\public.sdk\source\vst2.x\*"

Due to END OF LIFE of VST2 https://forums.steinberg.net/t/vst-2-sdk-discontinued/201774 
the folder "pluginterfaces" contained "aeffect.h" does not in the official SDK folder of Steinberg Audio. You need to find it in another folder that use old VST2 sources and copy the folder "pluginterfaces" in C:\path\to\MrsWatson\source\plugin\

Likewise, you can use the CMake GUI tool to accomplish the same task. This
command will generate a Visual Studio project in `C:\wherever\build` which can
use to build the software. Unlike on Unix, the generated Visual Studio project
cannot generate both 32- and 64-bit binaries in the same build. Instead you must
create another build directory for the 64-bit project. That is done like so:

    cd c:\wherever
    mkdir build64
    cd build64
    cmake -G "Visual Studio 14 2015 Win64" C:\path\to\MrsWatson


Linux
-----

Building MrsWatson on Linux may require a few additional packages on your
computer, including those necessary to make a 32-bit build on 64-bit machines.
By default, the software is built with your native architecture, so if you would
like to make a 32-bit build on a 64-bit machine, you will also need the
following packages (for debian-based systems):

  * g++-multilib
  * libc6-dev
  * libc6-dev-i386

This is in addition to the standard build tools (gcc, g++, etc). Otherwise,
building on Linux should be a simple matter of:

    mkdir build
    cd build
    cmake -D CMAKE_BUILD_TYPE=Debug ..
    make
    make install


[homebrew]: http://brew.sh
[cmake]: http://www.cmake.org/download/
