if not exist "C:\Program Files (x86)\Microsoft Visual Studio 8\Common7\IDE\devenv.exe" goto :VS9Compile

"C:\Program Files (x86)\Microsoft Visual Studio 8\Common7\IDE\devenv" EpServerEngine80.sln /build "Debug" /project "EpServerEngine\EpServerEngine80.vcproj" /projectconfig "Debug"
"C:\Program Files (x86)\Microsoft Visual Studio 8\Common7\IDE\devenv" EpServerEngine80.sln /build "Debug DLL" /project "EpServerEngine\EpServerEngine80.vcproj" /projectconfig "Debug DLL"
"C:\Program Files (x86)\Microsoft Visual Studio 8\Common7\IDE\devenv" EpServerEngine80.sln /build "Debug DLL Unicode" /project "EpServerEngine\EpServerEngine80.vcproj" /projectconfig "Debug DLL Unicode"
"C:\Program Files (x86)\Microsoft Visual Studio 8\Common7\IDE\devenv" EpServerEngine80.sln /build "Debug Unicode" /project "EpServerEngine\EpServerEngine80.vcproj" /projectconfig "Debug Unicode"
"C:\Program Files (x86)\Microsoft Visual Studio 8\Common7\IDE\devenv" EpServerEngine80.sln /build "Release" /project "EpServerEngine\EpServerEngine80.vcproj" /projectconfig "Release"
"C:\Program Files (x86)\Microsoft Visual Studio 8\Common7\IDE\devenv" EpServerEngine80.sln /build "Release DLL" /project "EpServerEngine\EpServerEngine80.vcproj" /projectconfig "Release DLL"
"C:\Program Files (x86)\Microsoft Visual Studio 8\Common7\IDE\devenv" EpServerEngine80.sln /build "Release DLL Unicode" /project "EpServerEngine\EpServerEngine80.vcproj" /projectconfig "Release DLL Unicode"
"C:\Program Files (x86)\Microsoft Visual Studio 8\Common7\IDE\devenv" EpServerEngine80.sln /build "Release Unicode" /project "EpServerEngine\EpServerEngine80.vcproj" /projectconfig "Release Unicode"

:VS9Compile

if not exist "C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE\devenv.exe" goto :VS10Compile

"C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpServerEngine90.sln /build "Debug" /project "EpServerEngine\EpServerEngine90.vcproj" /projectconfig "Debug"
"C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpServerEngine90.sln /build "Debug DLL" /project "EpServerEngine\EpServerEngine90.vcproj" /projectconfig "Debug DLL"
"C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpServerEngine90.sln /build "Debug DLL Unicode" /project "EpServerEngine\EpServerEngine90.vcproj" /projectconfig "Debug DLL Unicode"
"C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpServerEngine90.sln /build "Debug Unicode" /project "EpServerEngine\EpServerEngine90.vcproj" /projectconfig "Debug Unicode"
"C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpServerEngine90.sln /build "Release" /project "EpServerEngine\EpServerEngine90.vcproj" /projectconfig "Release"
"C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpServerEngine90.sln /build "Release DLL" /project "EpServerEngine\EpServerEngine90.vcproj" /projectconfig "Release DLL"
"C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpServerEngine90.sln /build "Release DLL Unicode" /project "EpServerEngine\EpServerEngine90.vcproj" /projectconfig "Release DLL Unicode"
"C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpServerEngine90.sln /build "Release Unicode" /project "EpServerEngine\EpServerEngine90.vcproj" /projectconfig "Release Unicode"

:VS10Compile

if not exist "C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\devenv.exe" goto :VS11Compile

"C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpServerEngine100.sln /build "Debug" /project "EpServerEngine\EpServerEngine100.vcxproj" /projectconfig "Debug"
"C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpServerEngine100.sln /build "Debug DLL" /project "EpServerEngine\EpServerEngine100.vcxproj" /projectconfig "Debug DLL"
"C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpServerEngine100.sln /build "Debug DLL Unicode" /project "EpServerEngine\EpServerEngine100.vcxproj" /projectconfig "Debug DLL Unicode"
"C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpServerEngine100.sln /build "Debug Unicode" /project "EpServerEngine\EpServerEngine100.vcxproj" /projectconfig "Debug Unicode"
"C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpServerEngine100.sln /build "Release" /project "EpServerEngine\EpServerEngine100.vcxproj" /projectconfig "Release"
"C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpServerEngine100.sln /build "Release DLL" /project "EpServerEngine\EpServerEngine100.vcxproj" /projectconfig "Release DLL"
"C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpServerEngine100.sln /build "Release DLL Unicode" /project "EpServerEngine\EpServerEngine100.vcxproj" /projectconfig "Release DLL Unicode"
"C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpServerEngine100.sln /build "Release Unicode" /project "EpServerEngine\EpServerEngine100.vcxproj" /projectconfig "Release Unicode"

:VS11Compile

if not exist "C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\IDE\devenv.exe" goto :Done

"C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\IDE\devenv" EpServerEngine110.sln /build "Debug" /project "EpServerEngine\EpServerEngine110.vcxproj" /projectconfig "Debug"
"C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\IDE\devenv" EpServerEngine110.sln /build "Debug DLL" /project "EpServerEngine\EpServerEngine110.vcxproj" /projectconfig "Debug DLL"
"C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\IDE\devenv" EpServerEngine110.sln /build "Debug DLL Unicode" /project "EpServerEngine\EpServerEngine110.vcxproj" /projectconfig "Debug DLL Unicode"
"C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\IDE\devenv" EpServerEngine110.sln /build "Debug Unicode" /project "EpServerEngine\EpServerEngine110.vcxproj" /projectconfig "Debug Unicode"
"C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\IDE\devenv" EpServerEngine110.sln /build "Release" /project "EpServerEngine\EpServerEngine110.vcxproj" /projectconfig "Release"
"C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\IDE\devenv" EpServerEngine110.sln /build "Release DLL" /project "EpServerEngine\EpServerEngine110.vcxproj" /projectconfig "Release DLL"
"C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\IDE\devenv" EpServerEngine110.sln /build "Release DLL Unicode" /project "EpServerEngine\EpServerEngine110.vcxproj" /projectconfig "Release DLL Unicode"
"C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\IDE\devenv" EpServerEngine110.sln /build "Release Unicode" /project "EpServerEngine\EpServerEngine110.vcxproj" /projectconfig "Release Unicode"


:Done