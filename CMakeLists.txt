cmake_minimum_required(VERSION 2.8)
project(sylar)

# 显示执行构建过程中详细的编译信息
set(CMAKE_VERBOSE_MAKEFILE ON)
# CMAKE_CXX_FLAGS
    #这种大写的变量，一般都是CMake预定义的内建变量，且他们是全局的。该变量可用于设置编译选项。直接使用set修改其值即可。
    #除了修改该变量的值之外，还可以通过add_compile_options命令来设置编译选项。它们的区别是：
    #add_compile_options命令添加的编译选项是针对所有编译器的(包括c和c++编译器)
    #CMAKE_C_FLAGS或CMAKE_CXX_FLAGS变量则是分别只针对c和c++编译器
# $ENV{}
    #获取环境变量
# CXXFLAGS
    #CXXFLAGS是C++编译器的编译选项
# -rdynamic
    # -rdynamic编译选项通知链接器将所有符号添加到动态符号表中
# -O3
    # -O3是一个优化选项，告诉编译器优化我们的代码
# -fPIC
    # -fPIC 作用于编译阶段，告诉编译器产生与位置无关代码，
    #即，产生的代码中，没有绝对地址，全部使用相对地址，故而代码可以被加载器加载到内存的任意位置，都可以正确的执行。
    #这正是共享库所要求的，共享库被加载时，在内存的位置不是固定的。
# -ggdb
    # -ggdb选项使编译器生成gdb专用的更为丰富的调试信息。
# -std=c++11
    # -std=c++11设置为使用C++11标准
# -Wall
    # -Wall选项告诉编译器 编译后显示所有警告
# -Wno-deprecated
    # 不要警告使用已弃用的功能
# -Werror
    # 告诉编译器视所有警告为错误，出现任何警告立即放弃编译
# -Wno-unused-function
    # 关闭 函数被定义了但没有被使用 而产生的警告，即，不使用的函数不警告。
# -Wno-builtin-macro-redefined
    # 如果某些内置宏被重新定义，请不要警告。这抑制了警告的重新定义__TIMESTAMP__，TIME，DATE，FILE，和__BASE_FILE__。
# -Wno-deprecated-declarations
    # 关闭使用废弃API的警告。
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

include_directories(.)

set(LIB_SRC 
    src/log.cc
    src/fiber.cc
    src/scheduler.cc
    src/util.cc
    src/thread.cc
    )

#生成库文件
add_library(sylar SHARED ${LIB_SRC}) 

set (LIB_LIB
    sylar
    pthread
    ${YAMLCPP}
)
#根据源文件生成可执行文件
add_executable(test ./tests/test.cc)
#指定依赖
add_dependencies(test sylar)
#链接动态库
target_link_libraries(test ${LIB_LIB})

#根据源文件生成可执行文件
add_executable(test_thread ./tests/test_thread.cc)
#指定依赖
add_dependencies(test_thread sylar)
#链接动态库
target_link_libraries(test_thread ${LIB_LIB})

#根据源文件生成可执行文件
add_executable(test_macro ./tests/test_macro.cc)
#指定依赖
add_dependencies(test_macro sylar)
#链接动态库
target_link_libraries(test_macro ${LIB_LIB})

#根据源文件生成可执行文件
add_executable(test_fiber ./tests/test_fiber.cc)
#指定依赖
add_dependencies(test_fiber sylar)
#链接动态库
target_link_libraries(test_fiber ${LIB_LIB})

#根据源文件生成可执行文件
add_executable(test_scheduler ./tests/test_scheduler.cc)
#指定依赖
add_dependencies(test_scheduler sylar)
#链接动态库
target_link_libraries(test_scheduler ${LIB_LIB})

#设置可执行文件的生成路径
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#设置库文件生成路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)