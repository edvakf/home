## Environment variable configuration
#
# LANG
#
#export LANG=ja_JP.UTF-8

## Default shell configuration
#
# set prompt
#
autoload colors
colors
case ${UID} in
0)
  PROMPT="%{${fg[yellow]}%}%D{%R} %B%{${fg[green]}%}%/#%{${reset_color}%}%b "
  PROMPT2="%B%{${fg[green]}%}%_#%{${reset_color}%}%b "
  SPROMPT="%B%{${fg[green]}%}Do you mean : %r ? [n,y,a,e]:%{${reset_color}%}%b "
  [ -n "${REMOTEHOST}${SSH_CONNECTION}" ] && 
    PROMPT="%{${fg[white]}%}${HOST%%.*} ${PROMPT}"
  ;;
*)
  PROMPT="%{${fg[yellow]}%}%D{%R} %{${fg[green]}%}%/%%%{${reset_color}%} "
  PROMPT2="%{${fg[green]}%}%_%%%{${reset_color}%} "
  SPROMPT="%{${fg[green]}%}Do you mean : %r ? [n,y,a,e]:%{${reset_color}%} "
  [ -n "${REMOTEHOST}${SSH_CONNECTION}" ] && 
    PROMPT="%{${fg[white]}%}${HOST%%.*} ${PROMPT}"
  ;;
esac

# directory name without "cd" command automatically changes directory
#
setopt auto_cd

# auto directory pushd that you can get dirs list by cd -[tab]
#
setopt auto_pushd
setopt pushd_ignore_dups

# command correct edition before each completion attempt
#
setopt correct

# no beep sound when complete list displayed
#
setopt nolistbeep

# no remove postfix slash of command line
#
setopt noautoremoveslash

## Keybind configuration
#
# emacs like keybind (e.x. Ctrl-a goes to head of a line and Ctrl-e goes 
#   to end of it)
#
bindkey -e

# historical backward/forward search with linehead string binded to ^P/^N
#
autoload history-search-end
zle -N history-beginning-search-backward-end history-search-end
zle -N history-beginning-search-forward-end history-search-end
bindkey "^P" history-beginning-search-backward-end
bindkey "^N" history-beginning-search-forward-end

## Command history configuration
#
HISTFILE=~/.zsh_history
HISTSIZE=10000
SAVEHIST=10000
setopt hist_ignore_dups     # ignore duplication command history list
setopt hist_ignore_all_dups # never store duplicate command
setopt share_history        # share command history data
setopt hist_ignore_space    # ignore line starting with a space

## Completion configuration
#
fpath=(~/.zsh/functions ${fpath})
autoload -U compinit
compinit -u

## don't logout by Ctrl-D 
setopt ignore_eof

## load user .zshrc configuration file
#
[ -f ~/.zshrc.my ] && source ~/.zshrc.my
