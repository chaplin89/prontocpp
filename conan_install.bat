@echo off
conan remote add conan-center https://conan.bintray.com True > nul 2>&1
conan remote add bincrafter https://api.bintray.com/conan/bincrafters/public-conan True > nul 2>&1
conan remote add inexor https://api.bintray.com/conan/inexorgame/inexor-conan True > nul 2>&1

conan install . -s build_type=Release --build missing -if ./.dep/release
conan install . -s build_type=Debug --build missing -if ./.dep/debug