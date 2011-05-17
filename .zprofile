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

  #function delete_old {
  #(
    #dir=$1
    #find $dir -mindepth 1 -maxdepth 1 -type f -atime +7 -delete
    #for file in `find $dir -mindepth 1 -maxdepth 1 -type d`; do delete_old $file; done
    #if [[ `ls $dir -1 | wc -l` -eq 0 ]]; then
      #rm -r $dir
    #fi
  #)
  #}

  ## remove contents of /tmp directory if too old
  #delete_old /tmp
  #mkdir -p /tmp

  # delete sockets first, -empty deletes regular files and empty directories
  find /tmp -mindepth 1 -atime +7 -type s -delete
  find /tmp -mindepth 1 -atime +7 -empty -delete

fi