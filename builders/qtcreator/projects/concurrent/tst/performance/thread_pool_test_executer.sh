#!/bin/bash


if [ $# -ne 2 ]; then
    echo "Syntax: $0 <binary-dir> <what-to-execute>"
    echo "    binary-dir is the directrory where the executables are"
    echo "    what-to-execute can be 'all' to execute all test"
    echo "                           a specific test number, like 044"
    echo "                           a range, like 023-031"
    exit 10
fi


dir_bin="$1"
if [ ! -d "$dir_bin" ]; then
    echo "directrory '$dir_bin' not found"
    exit 15
fi


if [ "$2" = "all" ]; then
    to_test=tenacitas.concurrent.test.performance.thread_pool_???
else
    range_sep=`echo "$2" | grep -`

    if [ "$range_sep" = "$2" ]; then
        initial_str=`echo $range_sep | cut -d '-' -f 1`
        final_str=`echo $range_sep | cut -d '-' -f 2`
        range_counter=`expr $initial_str + 0`
        final=`expr $final_str + 0`
        to_test=""
        while [ $range_counter -le $final ]; do
            test_in_range="tenacitas.concurrent.test.performance.thread_pool_$(printf "%03d" $range_counter)"
            to_test="$to_test $test_in_range"
            range_counter=$((range_counter+1))
        done

    else
        to_test="tenacitas.concurrent.test.performance.thread_pool_$2"
    fi
fi

msgs_per_thread_stats="/var/tmp/msg_per_thread.stat"
time_frame_msg="/var/tmp/time_frame_msg.stat"

rm -f "$msgs_per_thread_stats" 2> /dev/null
rm -f "$time_frame_msg" 2> /dev/null

cd "$dir_bin"

ls $to_test | while read exec; do
    echo -n "exec = $exec"
    if [ ! -x "$exec" ]; then
        echo "$exec is not executable"
    else
        test_name=`echo $exec | cut -d '.' -f 5`
        echo -n ", test_name = $test_name"
        test_num=`echo $test_name | cut -d '_' -f 3`
        echo -n ", test_num = $test_num"
        out="/var/tmp/$test_num.txt"
        ./$exec "desc" > x
        num_consumers=`cat x | cut -d ':' -f 2 | cut -d ',' -f 1 | cut -d ' ' -f 2`
        num_msgs=`cat x | cut -d ',' -f 2 | cut -d ' ' -f 2`
        work_sleep=`cat x | cut -d ',' -f 3 | cut -d ' ' -f 2`
        echo ", num_consumers = $num_consumers, num_msgs = $num_msgs, work_sleep = $work_sleep"

        ./"$exec" exec 2> "$out"

        result=`cat "$out" | grep consuming | cut -d '(' -f 2 | cut -d ',' -f 1 | wc -l`

        if [ $result != $((num_msgs)) ]; then
            echo "Expected $num_msgs, got $result"
        else
            cat "$out" | grep consuming | cut -d '|' -f 3 | sort | uniq -c | sed 's/[[:space:]]\+/,/g' |
                while read line; do
                    num_msg_handled=`echo $line | cut -d ',' -f 2`
                    num_handlers_threads=`echo $line | cut -d ',' -f 3`
                    echo "$test_num,$num_consumers,$num_msgs,$work_sleep,$num_msg_handled,$num_handlers_threads" >> "$msgs_per_thread_stats"
                done

            msg_counter=0
            while [ $msg_counter -lt $num_msgs ]; do
                msg_fmt=$(printf "%010d" $msg_counter)
                t_in=`cat $out | grep "adding ($msg_fmt" | cut -d '|' -f 2`
                t_out=`cat $out | grep "consuming ($msg_fmt" | cut -d '|' -f 2`
                echo "$test_num,$num_consumers,$num_msgs,$work_sleep,$msg_fmt,$t_in,$t_out,$((t_out-t_in))" >> "$time_frame_msg"
                msg_counter=$((msg_counter+1))
            done

        fi
    fi
done

rm -f x 2> /dev/null
