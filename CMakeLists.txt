cmake_minimum_required(VERSION 3.16)
project(kwin-cursor-wrap VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SYSTEMD REQUIRED IMPORTED_TARGET libsystemd)

add_executable(cursor_warper src/cursor_warper.cpp)
target_link_libraries(cursor_warper PRIVATE PkgConfig::SYSTEMD)
target_compile_options(cursor_warper PRIVATE -O2 -Wall -Wextra)

install(TARGETS cursor_warper DESTINATION bin)
install(FILES systemd/cursor-warper.service DESTINATION lib/systemd/user)
install(FILES udev/99-cursor-warper-uinput.rules DESTINATION lib/udev/rules.d)
install(DIRECTORY kwin-script/ DESTINATION share/kwin/scripts/cursor-wrap)
