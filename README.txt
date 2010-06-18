vpx libvpx-tester
README - 06-14-2010

Note: For usage see libvpx-tester-manual.pdf

Contents:

A) vpx libvpx-tester Build Procedure Windows 32Bit
B) vpx libvpx-tester Build Procedure Linux 32Bit
C) vpx libvpx-tester Build Procedure IMac 32Bit
D) vpx libvpx-tester Build Procedure Windows 64Bit
E) vpx libvpx-tester Build Procedure Linux 64Bit
F) vpx libvpx-tester Build Procedure IMac 64Bit


  Prerequisites - Inherited prerequisites from libvpx

    * All x86 targets require the Yasm[1] assembler be installed.
    * All Windows builds require that Cygwin[2] be installed.
    * Building the documentation requires PHP[3] and Doxygen[4]. If you do not
      have these packages, you must pass --disable-install-docs to the
      configure script.

    [1]: http://www.tortall.net/projects/yasm
    [2]: http://www.cygwin.com
    [3]: http://php.net
    [4]: http://www.doxygen.org


vpx libvpx-tester Build Procedure Windows 32Bit

Prerequisites:  Inherited prerequisites from libvpx (see libvpx read me for details) and Microsoft Visual Studio 2005

1)	Pull the libvpx from git://review.webmproject.org/libvpx.git that you wish to test
2)	Pull libvpx-tester from git://review.webmproject.org/libvpx-tester.git
3)	Check out an older revision of libvpx that you wish to test the new version against to \libvpx-old\ (optional)

Build and Collect libvpx Libraries - sample configurations are provided below, for more detailed information on vp8 sdk configurations see the README located in the libvpx directory

4)	Build New VP8 Release Libararies using cygwin
•	mkdir codec-sdk-build-VP8-Win32
	cd codec-sdk-build-VP8-Win32
	chmod -R 777 ../libvpx
	../libvpx/configure --target=x86-win32-vs8 --disable-codecs --enable-vp8 --enable-static-msvcrt --enable-multithread --enable-runtime-cpu-detect
	make
•	Open .sln file located in codec-sdk-build-VP8-Win32 and build in Release Mode.
•	libvpx.lib will be located in \codec-sdk-build-VP8-Win32\Win32\Release\

5)	Build New VP8 Debug Libraries using cygwin
•	mkdir codec-sdk-build-VP8-Win32-Debug
	cd codec-sdk-build-VP8-Win32-Debug
	chmod -R 777 ../libvpx
	../libvpx/configure --target=x86-win32-vs8 --disable-codecs --enable-vp8 --enable-static-msvcrt --enable-multithread --enable-mem-tracker
	make
•	Open .sln file located in codec-sdk-build-VP8-Win32-Debug and build in Debug Mode.
•	libvpx.lib will be located in \codec-sdk-build-VP8-Win32-Debug\Win32\Debug\

6)	Build Old VP8 Release library using cygwin (or locate old release library or executable)
•	mkdir codec-sdk-build-VP8-Win32-old
	cd codec-sdk-build-VP8-Win32-old
	chmod -R 777 ../libvpx-old
	../libvpx-old/configure --target=x86-win32-vs8 --disable-codecs --enable-vp8 --enable-static-msvcrt --enable-multithread --enable-runtime-cpu-detect
	make
•	Open .sln file located in codec-sdk-build-VP8-Win32-old and build in Release Mode.
•	libvpx.lib will be located in \codec-sdk-build-VP8-Win32-old\Win32\Release\

Build Main Test Executable from newest libvpx Libraries

7)	Place libvpx.lib  from \codec-sdk-build-VP8-Win32\Win32\Release\ in \libvpx-tester\MasterFile\lib\
8)	Open .sln file located in \libvpx-tester\MasterFile\
9)	Build in Release Mode.
10)	Rename executable produced in \libvpx-tester\MasterFile\Release\ from VP8_Tester_API.exe to VP8_Tester_API_32Bit.exe.

Build Supporting Debug Executable from newest libvpx Libraries

11)	Place libvpx.lib  from \codec-sdk-build-VP8-Win32-Debug\Win32\Debug\ in \libvpx-tester\SupportingPlugInFiles\VP8DebugPlugIn\MasterFile\lib
12)	Open .sln file located in \libvpx-tester\SupportingPlugInFiles\VP8DebugPlugIn\MasterFile\
13)	Build in Debug Mode.
14)	Rename executable produced in \libvpx-tester\VP8ReleasePlugIn\MasterFile\Release\ from VP8v--_ PlugIn_DLib_DMode.exe to VP8vNewest_PlugIn_DLib_DMode.exe.

Build Supporting Release Executable from newest libvpx Libraries

15)	Place libvpx.lib  from \codec-sdk-build-VP8-Win32\Win32\Release\ in \libvpx-tester\SupportingPlugInFiles\VP8ReleasePlugIn\MasterFile\lib\
16)	Open .sln file located in \libvpx-tester\VP8ReleasePlugIn\MasterFile\
17)	Build in Release Mode.
18)	Rename executable produced in \libvpx-tester\VP8ReleasePlugIn\MasterFile\Release\ from VP8v--_PlugIn_RLib_RMode.exe to VP8vNewest_PlugIn_RLib_RMode.exe.

Build Supporting Release Executable from old libvpx Libraries

19)	Place libvpx.lib  from \codec-sdk-build-VP8-Win32-old\Win32\Release\ in \libvpx-tester\SupportingPlugInFiles\VP8ReleasePlugIn\MasterFile\lib\
20)	Open .sln file located in \libvpx-tester\VP8ReleasePlugIn\MasterFile\
21)	Build in Release Mode.
22)	Rename executable produced in \libvpx-tester\VP8ReleasePlugIn\MasterFile\Release\ from VP8v--_PlugIn_RLib_RMode.exe to VP8vOldest_PlugIn_RLib_RMode.exe.

Gather Executables

23)	Create \libvpx-tester\TestFolder_32Bit\
24)	Place \libvpx-tester \MasterFile\Release\VP8_Tester_API_32Bit.exe in \libvpx-tester\TestFolder_32Bit\
25)	Place \libvpx-tester\SupportingPlugInFiles\VP8DebugPlugIn\MasterFile\Debug\VP8vNewest_PlugIn_DLib_DMode.exe in \libvpx-tester\TestFolder_32Bit\
26)	Place \libvpx-tester\SupportingPlugInFiles\VP8ReleasePlugIn\MasterFile\Release\VP8vNewest_PlugIn_RLib_RMode.exe in \libvpx-tester\TestFolder_32Bit\
27)	Place \libvpx-tester\SupportingPlugInFiles\VP8ReleasePlugIn\MasterFile\Release\VP8vOldest_PlugIn_RLib_RMode.exe in \libvpx-tester\TestFolder_32Bit\



vpx libvpx-tester Build Procedure Linux 32Bit

Prerequisites:  Inherited prerequisites from libvpx (see libvpx read me for details)

1)	Pull the libvpx from git://review.webmproject.org/libvpx.git that you wish to test
2)	Pull libvpx-tester from git://review.webmproject.org/libvpx-tester.git
3)	Check out an older revision of libvpx that you wish to test the new version against to \libvpx-old\ (optional)

Build and Collect libvpx Libraries - sample configurations are provided below, for more detailed information on vp8 sdk configurations see the README located in the libvpx directory

4)	Build New VP8 Release Libararies
•	mkdir codec-sdk-build-VP8-Lin32
	cd codec-sdk-build-VP8-Lin32
	chmod -R 777 ../libvpx
	../libvpx/configure --target=x86-linux-gcc --disable-codecs --enable-vp8 --enable-multithread --enable-runtime-cpu-detect
	make
•	make install
•	libvpx.a will be located in /codec-sdk-build-VP8-Lin32/

5)	Build New VP8 Debug Libraries
•	mkdir codec-sdk-build-VP8-Lin32-Debug
	cd codec-sdk-build-VP8-Lin32-Debug
	chmod -R 777 ../libvpx
	../libvpx/configure --target=x86-linux-gcc --disable-install-docs --disable-codecs --enable-vp8 --enable-multithread --enable-mem-tracker --enable-debug-libs --enable-runtime-cpu-detect
	make
•	make install
•	libvpx.a will be located in /codec-sdk-build-VP8-Lin32-Debug/

6)	Build Old VP8 Release library  (or locate old release library or executable)
•	mkdir codec-sdk-build-VP8-Lin32-old
	cd codec-sdk-build-VP8-Lin32-old
	chmod -R 777 ../libvpx-old
	../libvpx-old/configure --target=x86-linux-gcc --disable-codecs --enable-vp8 --enable-multithread --enable-runtime-cpu-detect
	make
•	make install
•	libvpx.a will be located in /codec-sdk-build-VP8-Lin32-old/

Copy and Rename VP8 Libraries

7)	Copy /codec-sdk-build-VP8-Lin32/libvpx.a to /libvpx-tester/MasterFile/lib/ renaming libvpx.a to libvpx_Lin32.a
8)	Copy /codec-sdk-build-VP8-Lin32-Debug/libvpx.a to to /libvpx-tester/SupportingPlugInFiles/VP8DebugPlugIn/MasterFile/lib/ renaming libvpx.a to libvpx_MemLin32.a
9)	Copy /codec-sdk-build-VP8-Lin32/libvpx.a to /libvpx-tester /SupportingPlugInFiles/VP8ReleasePlugIn/MasterFile/newlib/ renaming libvpx.a to libvpx_NewLin32.a
10)	Copy /codec-sdk-build-VP8-Lin32-old/libvpx.a to /libvpx-tester /SupportingPlugInFiles/VP8ReleasePlugIn/MasterFile/ renaming libvpx.a to oldlib/libvpx_OldLin32.a

Build Test Executables

11)	Run make_Linux_API_32Bit located in /libvpx-tester/ Test Executables will be located in /libvpx-tester/TestFolder_32Bit/



vpx libvpx-tester Build Procedure IMac 32Bit

Prerequisites:  Inherited prerequisites from libvpx (see libvpx read me for details)

1)	Pull the libvpx from git://review.webmproject.org/libvpx.git that you wish to test
2)	Pull libvpx-tester from git://review.webmproject.org/libvpx-tester.git
3)	Check out an older revision of libvpx that you wish to test the new version against to \libvpx-old\ (optional)

Build and Collect libvpx Libraries - sample configurations are provided below, for more detailed information on vp8 sdk configurations see the README located in the libvpx directory

4)	Build New VP8 Release Libararies
•	mkdir codec-sdk-build-VP8-IMac32
	cd codec-sdk-build-VP8-IMac32
	chmod -R 777 ../libvpx
	../libvpx/configure --target=x86-darwin9-gcc --disable-codecs --enable-vp8 --enable-multithread --enable-runtime-cpu-detect
	make
•	make install
•	libvpx.a will be located in /codec-sdk-build-VP8-IMac32/

5)	Build New VP8 Debug Libraries
•	mkdir codec-sdk-build-VP8-IMac32-Debug
	cd codec-sdk-build-VP8-IMac32-Debug
	chmod -R 777 ../libvpx
	../libvpx/configure --target=x86-darwin9-gcc --disable-install-docs --disable-codecs --enable-vp8 --enable-multithread --enable-mem-tracker --enable-debug-libs --enable-runtime-cpu-detect
	make
•	make install
•	libvpx.a will be located in /codec-sdk-build-VP8-IMac32-Debug/

6)	Build Old VP8 Release library  (or locate old release library or executable)
•	mkdir codec-sdk-build-VP8-IMac32-old
	cd codec-sdk-build-VP8-IMac32-old
	chmod -R 777 ../libvpx-old
	../libvpx-old/configure --target=x86-darwin9-gcc --disable-codecs --enable-vp8 --enable-multithread --enable-runtime-cpu-detect
	make
•	make install
•	libvpx.a will be located in /codec-sdk-build-VP8-IMac32-old/

Copy and Rename VP8 Libraries

7)	Copy /codec-sdk-build-VP8-IMac32/libvpx.a to /libvpx-tester/MasterFile/lib/ renaming libvpx.a to libvpx_IMac32.a
8)	Copy /codec-sdk-build-VP8-IMac32-Debug/libvpx.a to to /libvpx-tester/SupportingPlugInFiles/VP8DebugPlugIn/MasterFile/lib/ renaming libvpx.a to libvpx_MemIMac32.a
9)	Copy /codec-sdk-build-VP8-IMac32/libvpx.a to /libvpx-tester /SupportingPlugInFiles/VP8ReleasePlugIn/MasterFile/newlib/ renaming libvpx.a to libvpx_NewIMac32.a
10)	Copy /codec-sdk-build-VP8-IMac32-old/libvpx.a to /libvpx-tester /SupportingPlugInFiles/VP8ReleasePlugIn/MasterFile/oldlib/ renaming libvpx.a to libvpx_OldIMac32.a

Build Test Executables

11)	Run make_IMac_API_32Bit located in /libvpx-tester/ Test Executables will be located in /libvpx-tester/TestFolder_32Bit/



vpx libvpx-tester Build Procedure Windows 64Bit

Prerequisites:  Inherited prerequisites from libvpx (see libvpx read me for details) and Microsoft Visual Studio 2005

1)	Pull the libvpx from git://review.webmproject.org/libvpx.git that you wish to test
2)	Pull libvpx-tester from git://review.webmproject.org/libvpx-tester.git
3)	Check out an older revision of libvpx that you wish to test the new version against to \libvpx-old\ (optional)

Build and Collect libvpx Libraries - sample configurations are provided below, for more detailed information on vp8 sdk configurations see the README located in the libvpx directory

4)	Build New VP8 Release Libararies using cygwin
•	mkdir codec-sdk-build-VP8-Win64
	cd codec-sdk-build-VP8-Win64
	chmod -R 777 ../libvpx
	../libvpx/configure --target=x86_64-win64-vs8 --disable-codecs --enable-vp8 --enable-static-msvcrt --enable-multithread --enable-runtime-cpu-detect
	make
•	Open .sln file located in codec-sdk-build-VP8-Win64 and build in Release Mode.
•	libvpx.lib will be located in \codec-sdk-build-VP8-Win64\Win64\Release\

5)	Build New VP8 Debug Libraries using cygwin
•	mkdir codec-sdk-build-VP8-Win64-Debug
	cd codec-sdk-build-VP8-Win64-Debug
	chmod -R 777 ../libvpx
	../libvpx/configure --target=x86_64-win64-vs8 --disable-codecs --enable-vp8 --enable-static-msvcrt --enable-multithread --enable-mem-tracker
	make
•	Open .sln file located in codec-sdk-build-VP8-Win64-Debug and build in Debug Mode.
•	libvpx.lib will be located in \codec-sdk-build-VP8-Win64-Debug\Win64\Debug\

6)	Build Old VP8 Release library using cygwin (or locate old release library or executable)
•	mkdir codec-sdk-build-VP8-Win64-old
	cd codec-sdk-build-VP8-Win64-old
	chmod -R 777 ../libvpx-old
	../libvpx-old/configure --target=x86_64-win64-vs8 --disable-codecs --enable-vp8 --enable-static-msvcrt --enable-multithread --enable-runtime-cpu-detect
	make
•	Open .sln file located in codec-sdk-build-VP8-Win64-old and build in Release Mode.
•	libvpx.lib will be located in \codec-sdk-build-VP8-Win64-old\Win64\Release\

Build Main Test Executable from newest libvpx Libraries

7)	Place libvpx.lib  from \codec-sdk-build-VP8-Win64\Win64\Release\ in \libvpx-tester\MasterFile\lib\
8)	Open .sln file located in \libvpx-tester\MasterFile\
9)	Build in Release Mode.
10)	Rename executable produced in \libvpx-tester\MasterFile\Release\ from VP8_Tester_API.exe to VP8_Tester_API_64Bit.exe.

Build Supporting Debug Executable from newest libvpx Libraries

11)	Place libvpx.lib  from \codec-sdk-build-VP8-Win64-Debug\Win64\Debug\ in \libvpx-tester\SupportingPlugInFiles\VP8DebugPlugIn\MasterFile\lib
12)	Open .sln file located in \libvpx-tester\SupportingPlugInFiles\VP8DebugPlugIn\MasterFile\
13)	Build in Debug Mode.
14)	Rename executable produced in \libvpx-tester\VP8ReleasePlugIn\MasterFile\Release\ from VP8v--_ PlugIn_DLib_DMode.exe to VP8vNewest_PlugIn_DLib_DMode.exe.

Build Supporting Release Executable from newest libvpx Libraries

15)	Place libvpx.lib  from \codec-sdk-build-VP8-Win64\Win64\Release\ in \libvpx-tester\SupportingPlugInFiles\VP8ReleasePlugIn\MasterFile\lib\
16)	Open .sln file located in \libvpx-tester\VP8ReleasePlugIn\MasterFile\
17)	Build in Release Mode.
18)	Rename executable produced in \libvpx-tester\VP8ReleasePlugIn\MasterFile\Release\ from VP8v--_PlugIn_RLib_RMode.exe to VP8vNewest_PlugIn_RLib_RMode.exe.

Build Supporting Release Executable from old libvpx Libraries

19)	Place libvpx.lib  from \codec-sdk-build-VP8-Win64-old\Win64\Release\ in \libvpx-tester \SupportingPlugInFiles\VP8ReleasePlugIn\MasterFile\lib\
20)	Open .sln file located in \libvpx-tester\VP8ReleasePlugIn\MasterFile\
21)	Build in Release Mode.
22)	Rename executable produced in \libvpx-tester\VP8ReleasePlugIn\MasterFile\Release\ from VP8v--_PlugIn_RLib_RMode.exe to VP8vOldest_PlugIn_RLib_RMode.exe.

Gather Executables

23)	Create \libvpx-tester\TestFolder_64Bit\
24)	Place \libvpx-tester \MasterFile\Release\VP8_Tester_API_64Bit.exe in \libvpx-tester\TestFolder_64Bit\
25)	Place \libvpx-tester\SupportingPlugInFiles\VP8DebugPlugIn\MasterFile\Debug\VP8vNewest_PlugIn_DLib_DMode.exe in \libvpx-tester\TestFolder_64Bit\
26)	Place \libvpx-tester\SupportingPlugInFiles\VP8ReleasePlugIn\MasterFile\Release\VP8vNewest_PlugIn_RLib_RMode.exe in \libvpx-tester\TestFolder_64Bit\
27)	Place \libvpx-tester\SupportingPlugInFiles\VP8ReleasePlugIn\MasterFile\Release\VP8vOldest_PlugIn_RLib_RMode.exe in \libvpx-tester\TestFolder_64Bit\



vpx libvpx-tester Build Procedure Linux 64Bit

Prerequisites:  Inherited prerequisites from libvpx (see libvpx read me for details)

1)	Pull the libvpx from git://review.webmproject.org/libvpx.git that you wish to test
2)	Pull libvpx-tester from git://review.webmproject.org/libvpx-tester.git
3)	Check out an older revision of libvpx that you wish to test the new version against to \libvpx-old\ (optional)

Build and Collect libvpx Libraries - sample configurations are provided below, for more detailed information on vp8 sdk configurations see the README located in the libvpx directory

4)	Build New VP8 Release Libararies
•	mkdir codec-sdk-build-VP8-Lin64
	cd codec-sdk-build-VP8-Lin64
	chmod -R 777 ../libvpx
	../libvpx/configure --target=x86_64-linux-gcc --disable-codecs --enable-vp8 --enable-multithread --enable-runtime-cpu-detect
	make
•	make install
•	libvpx.a will be located in /codec-sdk-build-VP8-Lin64/

5)	Build New VP8 Debug Libraries
•	mkdir codec-sdk-build-VP8-Lin64-Debug
	cd codec-sdk-build-VP8-Lin64-Debug
	chmod -R 777 ../libvpx
	../libvpx/configure --target=x86_64-linux-gcc --disable-install-docs --disable-codecs --enable-vp8 --enable-multithread --enable-mem-tracker --enable-debug-libs --enable-runtime-cpu-detect
	make
•	make install
•	libvpx.a will be located in /codec-sdk-build-VP8-Lin64-Debug/

6)	Build Old VP8 Release library  (or locate old release library or executable)
•	mkdir codec-sdk-build-VP8-Lin64-old
	cd codec-sdk-build-VP8-Lin64-old
	chmod -R 777 ../libvpx-old
	../libvpx-old/configure --target=x86_64-linux-gcc --disable-codecs --enable-vp8 --enable-multithread --enable-runtime-cpu-detect
	make
•	make install
•	libvpx.a will be located in /codec-sdk-build-VP8-Lin64-old/

Copy and Rename VP8 Libraries

7)	Copy /codec-sdk-build-VP8-Lin64/libvpx.a to /libvpx-tester/MasterFile/lib/ renaming libvpx.a to libvpx_Lin64.a
8)	Copy /codec-sdk-build-VP8-Lin64-Debug/libvpx.a to to /libvpx-tester/SupportingPlugInFiles/VP8DebugPlugIn/MasterFile/lib/ renaming libvpx.a to libvpx_MemLin64.a
9)	Copy /codec-sdk-build-VP8-Lin64/libvpx.a to /libvpx-tester /SupportingPlugInFiles/VP8ReleasePlugIn/MasterFile/newlib/ renaming libvpx.a to libvpx_NewLin64.a
10)	Copy /codec-sdk-build-VP8-Lin64-old/libvpx.a to /libvpx-tester /SupportingPlugInFiles/VP8ReleasePlugIn/MasterFile/oldlib/ renaming libvpx.a to libvpx_OldLin64.a

Build Test Executables

11)	Run make_Linux_API_64Bit located in /libvpx-tester/ Test Executables will be located in /libvpx-tester/TestFolder_64Bit/



vpx libvpx-tester Build Procedure IMac 64Bit

Prerequisites:  Inherited prerequisites from libvpx (see libvpx read me for details)

1)	Pull the libvpx from git://review.webmproject.org/libvpx.git that you wish to test
2)	Pull libvpx-tester from git://review.webmproject.org/libvpx-tester.git
3)	Check out an older revision of libvpx that you wish to test the new version against to \libvpx-old\ (optional)

Build and Collect libvpx Libraries - sample configurations are provided below, for more detailed information on vp8 sdk configurations see the README located in the libvpx directory

4)	Build New VP8 Release Libararies
•	mkdir codec-sdk-build-VP8-IMac64
	cd codec-sdk-build-VP8-IMac64
	chmod -R 777 ../libvpx
	../libvpx/configure --target=x86_64-darwin9-gcc --disable-codecs --enable-vp8 --enable-multithread --enable-runtime-cpu-detect
	make
•	make install
•	libvpx.a will be located in /codec-sdk-build-VP8-IMac64/

5)	Build New VP8 Debug Libraries
•	mkdir codec-sdk-build-VP8-IMac64-Debug
	cd codec-sdk-build-VP8-IMac64-Debug
	chmod -R 777 ../libvpx
	../libvpx/configure --target=x86_64-darwin9-gcc --disable-install-docs --disable-codecs --enable-vp8 --enable-multithread --enable-mem-tracker --enable-debug-libs --enable-runtime-cpu-detect
	make
•	make install
•	libvpx.a will be located in /codec-sdk-build-VP8-IMac64-Debug/

6)	Build Old VP8 Release library  (or locate old release library or executable)
•	mkdir codec-sdk-build-VP8-IMac64-old
	cd codec-sdk-build-VP8-IMac64-old
	chmod -R 777 ../libvpx-old
	../libvpx-old/configure --target=x86_64-darwin9-gcc --disable-codecs --enable-vp8 --enable-multithread --enable-runtime-cpu-detect
	make
•	make install
•	libvpx.a will be located in /codec-sdk-build-VP8-IMac64-old/

Copy and Rename VP8 Libraries

7)	Copy /codec-sdk-build-VP8-IMac64/libvpx.a to /libvpx-tester/MasterFile/lib/ renaming libvpx.a to libvpx_IMac64.a
8)	Copy /codec-sdk-build-VP8-IMac64-Debug/libvpx.a to to /libvpx-tester/SupportingPlugInFiles/VP8DebugPlugIn/MasterFile/lib/ renaming libvpx.a to libvpx_MemIMac64.a
9)	Copy /codec-sdk-build-VP8-IMac64/libvpx.a to /libvpx-tester /SupportingPlugInFiles/VP8ReleasePlugIn/MasterFile/newlib/ renaming libvpx.a to libvpx_NewIMac64.a
10)	Copy /codec-sdk-build-VP8-IMac64-old/libvpx.a to /libvpx-tester /SupportingPlugInFiles/VP8ReleasePlugIn/MasterFile/oldlib/ renaming libvpx.a to libvpx_OldIMac64.a

Build Test Executables

11)	Run make_IMac_API_64Bit located in /libvpx-tester/ Test Executables will be located in /libvpx-tester/TestFolder_64Bit/
