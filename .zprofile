# .zprofile runs on login
# .zshenv -> .zprofile -> .zshrc -> .zlogin

# Cygwin ssh-agent sharing
# http://www.snowelm.com/~t/doc/tips/20030625.ja.html
if [ $CYGWIN ]; then

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

fi