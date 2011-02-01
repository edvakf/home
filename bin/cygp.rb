#!/usr/bin/env ruby

if ARGV[0] == "-c"
  path = ''
  open('/dev/clipboard', 'r') {|f|
    path = %x{cygpath -u "#{f.read}"}.strip
  }
  puts "copying: " + path
  open('/dev/clipboard', 'w') {|f|
    f.print path
  }
else
  print %x{cygpath -u "#{$stdin.read}"}.strip
end