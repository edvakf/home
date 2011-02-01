#!/usr/bin/env ruby

if ARGV[0] == "-c"
  open('/dev/clipboard', 'w+') {|f|
    path = %x{cygpath -u "#{f.read}"}.strip
    puts "copying: " + path
    f.print path
  }
else
  print %x{cygpath -u "#{$stdin.read}"}.strip
end