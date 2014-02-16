# .zprofile runs on login
# .zshenv -> .zprofile -> .zshrc -> .zlogin

# Cygwin ssh-agent sharing
# http://www.snowelm.com/~t/doc/tips/20030625.ja.html
if [[ $OSTYPE = "cygwin" ]]; then

  # share ssh-keygen information
  echo -n "ssh-agent: "
  source ~/.ssh-agent-info
  /usr/bin/ssh-add -l >&/dev/null
  if [ $? = 2 ] ; then
    echo -n "ssh-agent: restart...."
    /usr/bin/ssh-agent >~/.ssh-agent-info
    source ~/.ssh-agent-info
  fi

  if /usr/bin/ssh-add -l >&/dev/null ; then
    echo "ssh-agent: Identity is already stored."
  else
    /usr/bin/ssh-add
  fi
  # delete sockets first, -empty deletes regular files and empty directories
  find /tmp -mindepth 1 -atime +30 -type s -delete
  find /tmp -mindepth 1 -atime +30 -empty -delete

  # http://lists.gnu.org/archive/html/bug-gettext/2011-09/msg00006.html
  export LC_ALL="C.UTF-8"
  export LANG="C.UTF-8"
fi
