#!/bin/ruby

open('/dev/clipboard', 'w') {|f|
  f.print($stdin.read.gsub("\r?\n", "\r\n"))
}
