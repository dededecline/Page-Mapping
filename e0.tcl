#!/usr/bin/expect

set timeout 10

expect_after {
    timeout {
        puts "----> timeout <----\r"
        exit
    }
}

spawn qemu -nographic --serial mon:stdio -hdc kernel/kernel.img

expect "*** System Shutdown ***\r"

send \001
send "x"
