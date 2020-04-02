#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Syntax: $0 <dir-above-tenacitas>"
    echo "Example: $0 /opt/dvp"
    exit 10
fi


function create_test() {
    echo -n "Creating $test_name,$consumers,$messages,$sleep..."

    # code

    if [ -d "$code_dir/$test_name" ]; then
        echo "Test $code_dir/$test_name already exists"
        return
    fi

    mkdir -p "$code_dir/$test_name"
    if [ $? -ne 0 ]; then
        echo "Error creating $test_name"
        return
    fi

    main="$code_dir/$test_name/main.cpp"

    touch "$main"

    echo "#include <concurrent/bus/thread_pool.h>" > $main
    echo "#include <concurrent/tst/performance/thread_pool_tester.h" >> $main
    echo "#include <logger/cerr/log.h>"  >> $main
    echo "#include <tester/bus/test.h>"  >> $main
    echo "" >> $main
    echo "struct $test_name {"  >> $main
    echo "  bool operator()() {"  >> $main
    echo "    thread_pool_tester<$consumers, $messages, $sleep>()();"  >> $main
    echo "    return true;"  >> $main
    echo "  }"  >> $main
    echo ""  >> $main
    echo " static std::string desc() { return \"$consumers consumers, $messages, $sleep work sleep\"; }"  >> $main
    echo ""  >> $main
    echo "  static std::string name() { return "$test_name"; }"  >> $main
    echo "};"  >> $main
    echo ""  >> $main
    echo "int main(int argc, char **argv) {"  >> $main
    echo "  logger::cerr::log::set_debug();"  >> $main
    echo "  tester::bus::test::run<$test_name>(argc, argv);" >> $main
    echo "}"  >> $main

    # build

    if [ -d "$build_dir/$test_name" ]; then
        echo "$build_dir/$test_name already exists"
        return
    fi

    mkdir -p "$build_dir/$test_name"

    if [ $? -ne 0 ]; then
        echo "Error creating $build_dir/$test_name"
        return
    fi

    performance_pro="$build_dir/performance.pro"
    rm $performance_pro 
    echo "SUBDIRS = \"" > $performance_pro
    
    pro="$build_dir/$test_name/$test_name.pro"

    echo "QT -= core" > $pro
    echo ""  >> $pro
    echo "QMAKE_CXXFLAGS += -std=c++11" >> $pro
    echo ""  >> $pro
    echo "TEMPLATE = app" >> $pro
    echo ""  >> $pro
    echo "TARGET = tenacitas.concurrent.test.$test_name"  >> $pro
    echo ""  >> $pro
    echo "CONFIG+=test" >> $pro
    echo ""  >> $pro
    echo "SOURCES += \ "  >> $pro
    echo "    ../../../../../code/concurrent/test/$test_name/main.cpp \ "  >> $pro
    echo "    ../../../../../code/concurrent/test/msa_a.cpp "  >> $pro
    echo ""  >> $pro
    echo "include (../../../common.pri)"  >> $pro
    echo ""  >> $pro
    echo "LIBS += $$libs_dir/libtenacitas.logger.cerr.$$static_lib_ext"   >> $pro
    echo "LIBS += $$libs_dir/libtenacitas.concurrent.lib.$$static_lib_ext"   >> $pro
    echo "LIBS += $$libs_dir/libtenacitas.tester.lib.$$static_lib_ext"   >> $pro
    echo ""  >> $pro
    echo "HEADERS += \ " >> $pro
    echo "    ../../../../../code/concurrent/test/msg_a.h \ "  >> $pro
    echo "    ../../../../../code/concurrent/test/thread_pool_tester.h " >> $pro

#    cat performance.pro | sed -e "s/###/   $test_name \\\ \n ### /g" > x
    #    mv x pepro
    echo "$test_name" >> $performance_pro

    echo "SUCCESS!"
}


base_dir="$1"

## code
code_dir="$base_dir/tenacitas/code/concurrent/tst/performance"

if [ ! -d "$code_dir" ]; then
    #    echo "$code_dir does not exist"
    #    exit 15
    mkdir -p $code_dir
fi

## build
build_dir="$base_dir/tenacitas/builders/qtcreator/projects/concurrent/tst/performance"



if [ ! -d "$build_dir" ]; then
    #    echo "$build_dir does not exist"
    #    exit 50
    mkdir -p $build_dir
fi



num_test=0
consumers=2
while [ $consumers -lt 13 ]; do
    messages=1
    while [ $messages -lt 10001 ]; do
        sleep=300
        while [ $sleep -lt 5301 ]; do
            test_name=thread_pool_$(printf "%03d" $num_test)

            echo "`(printf "%03d" $num_test)`,$consumers,$messages,$sleep"

            create_test

            sleep=$((sleep+2500))
            num_test=$((num_test+1))
        done
        messages=$((messages*10))
    done
    consumers=$((consumers+5))
done





