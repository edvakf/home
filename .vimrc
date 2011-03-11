set encoding=utf-8
set fileencodings=ucs-bom,iso-2022-jp-3,iso-2022-jp,eucjp-ms,euc-jisx0213,euc-jp,sjis,cp932,utf-8
set fileformats=unix,dos

" Detect Mac OS X once here
" I'm checking !has('win32unix') because calling "system" can be very slow on cygwin
let g:hasMacOSX = !has('win32unix') && (has('unix') && match(system("uname"),'Darwin') != -1)

" backspace for <LF> etc.
set backspace=indent,eol,start

" don't append LF at the end of file
" http://d.hatena.ne.jp/odz/20070111/1168558681
" this turns off expandtab, so bring it before that
set binary
set noendofline

set tabstop=2
set shiftwidth=2
set expandtab
set autoindent
set nocompatible
set smarttab
"set copyindent
set preserveindent
syntax on
filetype on
filetype indent on
filetype plugin on

" detectindent http://www.vim.org/scripts/script.php?script_id=1171
let g:detectindent_preferred_expandtab = 1
let g:detectindent_preferred_indent = 2
autocmd BufReadPost * :DetectIndent

" http://d.hatena.ne.jp/studio-m/20080117/1200552387
set listchars=tab:\|_,eol:.,extends:$
set list

"change color scheme
"http://www.t-sk.org/~chick/pukiwiki/index.php?vim%A4%CE%BF%A7%C0%DF%C4%EA
colorscheme elflord
hi Function     term=bold   ctermfg=Brown guifg=Brown

"keep backup files
"http://www.redout.net/data/vi.html
"set backup

" search ignores case
set ignorecase
" search does not ignore case if search word contains CAPITAL
set smartcase
" highlight search
set hlsearch
" clear search highlight by pressing Esc twice http://blog.appling.jp/archives/140
nnoremap <Esc><Esc> :nohlsearch<Return>

command! W w
command! WQ wq
command! Wq wq

" ; to : (no need to press shift)
noremap ; :

"http://vim-users.jp/2010/02/hack122/
"yank to the end of the line
nnoremap Y y$

" escape from insert mode by <C-l>
" http://vim.g.hatena.ne.jp/ka-nacht/20081204/1228383305
inoremap <C-l> <Esc>

" save when escaping insert mode
" http://mono.kmc.gr.jp/~yhara/d/?date=20090624#p01
" http://vim.g.hatena.ne.jp/ka-nacht/20090625/1245857600
if !has('win32')
  autocmd InsertLeave * wall
endif

" http://d.hatena.ne.jp/dayflower/20081208/1228725403
" one liner comment will not be not auto-indented
if has("autocmd")
    autocmd FileType * let &l:comments =
        \join(filter(split(&l:comments, ','), 'v:val =~ "^[sme]"'), ',')
endif

noremap j gj
noremap k gk
noremap <Down> gj
noremap <Up> gk

"closing brace, quote, etc. completion
inoremap { {}<LEFT>
inoremap [ []<LEFT>
inoremap ( ()<LEFT>
inoremap " ""<LEFT>
inoremap <expr> ' CompleteSingleQuote()

function CompleteSingleQuote()
  if getline(".")[col(".") - 2] =~ "[a-zA-Z]"
    return "'"
  else
    return "''\<LEFT>"
  end
endfunction

" surround selected area in brackets etc.
vnoremap f( s(<C-R>")<ESC>
vnoremap f[ s[<C-R>"]<ESC>
vnoremap f{ s{<C-R>"}<ESC>
vnoremap f" s"<C-R>""<ESC>
vnoremap f' s'<C-R>"'<ESC>
vnoremap f< s<<C-R>"><ESC>



" actionscript http://www.vim.org/scripts/script.php?script_id=3275
au Bufread,BufNewFile *.as set filetype=actionscript
" Python
autocmd BufEnter SConstruct set filetype=python
autocmd BufEnter SConscript set filetype=python
"yorick highlighted as cpp
au Bufenter *.i set filetype=cpp
" .lyx is highlighted as .tex
au Bufenter *.lyx set filetype=tex
" ~/.crontab is a crontab file (crontab ~/.crontab)
au Bufenter .crontab set filetype=crontab



" emacs like editing
inoremap <C-A>  <Home>
inoremap <C-E>  <End>
inoremap <C-B>  <Left>
inoremap <C-F>  <Right>
inoremap <C-N>  <Down>
inoremap <C-P>  <UP>
inoremap <C-D>  <Delete>
inoremap <C-K>  <Esc>Da
inoremap <C-Y>  <Esc>pa

"http://www.his.kanazawa-it.ac.jp/~idurumi/linux/vim/vimrc07.html
set ruler
set showcmd



if !has('win32')
  set backup
  set backupdir=~/.backup/vim
  set viewdir=~/.backup/view
  set directory=~/.backup/vim/tmp
  
  if empty(glob("~/.backup/vim/tmp"))
    call system("mkdir -p ~/.backup/vim/tmp")
  endif

  if has( "autocmd" )
    autocmd BufWritePre * call UpdateBackupFile()
    function! UpdateBackupFile()
      let dir = strftime("~/.backup/vim/%Y/%m/%d", localtime())
      if !isdirectory(dir)
        call system("mkdir -p ".dir)
        call system("chown atsushi ".dir)
      endif
      exe "set backupdir=".dir
      unlet dir
      let ext = strftime("%H_%M_%S", localtime())
      exe "set backupext=.".ext
      unlet ext
    endfunction
  endif
endif

" html output
let html_no_pre = 0
unlet html_no_pre
let html_use_css = 1
let use_xhtml = 1

" copy to clipboard
if has('win32unix') " cygwin
  " getclip/putclip is horrible with unicode, so use /dev/clipboard (~/bin/{get|put}clip.rb)
  nnoremap sc :call system("putclip", @")<CR>
  nnoremap sp :r! getclip<CR>
elseif has('gui') " gvim
  nnoremap sc :let @* = @"<CR>
  nnoremap sp "+gP
elseif hasMacOSX
  " http://www.mail-archive.com/vim-latex-devel@lists.sourceforge.net/msg00773.html
  nnoremap sc :call system("pbcopy", @")<CR>
  nnoremap sp :r! pbpaste<CR>
endif



" for windows gVim
" change font
if has('win32')
  set guifont=Consolas:h10:w5 guifontwide=MeiryoKe_Console:h10:w5
endif

" change window size
if has("gui_running")
  set lines=40 columns=120
endif

" disable bell
"":set visualbell t_vb=
autocmd VimEnter * set vb t_vb=""

" turn off ime in insert mode by default
if has('win32')
  set iminsert=0
  set imsearch=0
endif


" reopen this file with dos format
function DOS()
  edit ++ff=dos
endfunction


" enable mouse scroll http://bitheap.org/mouseterm/
if has("mouse")
  set mouse=a

  " if sc -> copy to clipboard is on, then copy selection to clipboard at left mouse button up
  if has('win32unix') || has('gui') || hasMacOSX
    vmap <LeftRelease> yscgv
  endif
endif


