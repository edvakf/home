" Detect Mac OS X once here
" I'm checking !has('win32unix') because calling "system" can be very slow on cygwin
let g:hasMacOSX = has("macunix") || (!has('win32unix') && (has('unix') && match(system("uname"),'Darwin') != -1))

let mapleader = ","

" vim-pathogen
call pathogen#runtime_append_all_bundles()


" don't append LF at the end of file
" http://d.hatena.ne.jp/odz/20070111/1168558681
" this turns off expandtab, so bring it before that
"set binary
"set noendofline

set encoding=utf-8
set fileencodings=utf-8
set fileformats=unix
set ambiwidth=double

set tabstop=2
set shiftwidth=2
set expandtab
set autoindent
set nocompatible
set smarttab
"set copyindent
set preserveindent

" always show statusline, customize statusline
" default is %<%f\ %h%m%r%=%-14.(%l,%c%V%)\ %P
set laststatus=2
set statusline=%<%f\ %h%m%r%=%-7.(%l,%c%V%)\ %y%w%{'['.(&fenc!=''?&fenc:&enc).']['.&ff.'][et,sw,ts='.&et.','.&sw.','.&ts.']'}

syntax on
filetype on
filetype indent on
filetype plugin on

" backspace for <LF> etc.
set backspace=indent,eol,start

"http://www.his.kanazawa-it.ac.jp/~idurumi/linux/vim/vimrc07.html
set ruler
set showcmd

" http://d.hatena.ne.jp/studio-m/20080117/1200552387
set listchars=tab:\|_,trail:_,extends:$
set list

"change color scheme
"http://www.t-sk.org/~chick/pukiwiki/index.php?vim%A4%CE%BF%A7%C0%DF%C4%EA
colorscheme elflord
hi Function     term=bold   ctermfg=Brown guifg=Brown

let g:neocomplcache_enable_at_startup = 1

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

" increase history cache, <C-p> to backword search etc.
set history=1000
cnoremap <Up> <C-p>
cnoremap <Down> <C-n>

" ; to : (no need to press shift)
noremap ; :

"http://vim-users.jp/2010/02/hack122/
"yank to the end of the line
nnoremap Y y$

" escape from insert mode by <C-l>
" http://vim.g.hatena.ne.jp/ka-nacht/20081204/1228383305
noremap! <C-l> <Esc>
vnoremap <C-l> <Esc>


noremap j gj
noremap k gk
noremap <Down> gj
noremap <Up> gk

" search selection
vnoremap <silent> * "vy/\V<C-r>=substitute(escape(@v,'\/'),"\n",'\\n','g')<CR><CR>

" emacs like editing
noremap! <C-A>  <Home>
noremap! <C-E>  <End>
noremap! <C-B>  <Left>
noremap! <C-F>  <Right>
noremap! <C-N>  <Down>
noremap! <C-P>  <UP>
noremap! <C-D>  <Delete>
inoremap <C-K>  <Esc>Da
inoremap <C-Y>  <Esc>pa


" window/tab related
set splitright
set splitbelow

nnoremap <C-h> <C-w>h
nnoremap <C-j> <C-w>j
nnoremap <C-k> <C-w>k
nnoremap <C-l> <C-w>l
nnoremap <C-w>w :vsplit<CR>

nnoremap ] <C-w>>
nnoremap [ <C-w><



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
        "call system("chown atsushi ".dir)
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

" turn off ime in insert mode by default
if has('win32')
  set iminsert=0
  set imsearch=0
endif


" reopen this file with dos format
command! DOS edit ++ff=dos


" enable mouse scroll http://bitheap.org/mouseterm/
if has("mouse")
  set mouse=vi

  " if sc -> copy to clipboard is on, then copy selection to clipboard at left mouse button up
  if has('win32unix') || has('gui') || hasMacOSX
    vmap <LeftRelease> yscgv
  endif
endif


" save when escaping insert mode
" http://mono.kmc.gr.jp/~yhara/d/?date=20090624#p01
" http://vim.g.hatena.ne.jp/ka-nacht/20090625/1245857600
" also save when there has not been any action for some time
if !has('win32')
  autocmd InsertLeave * wall
  set autowrite
  autocmd CursorHold * wall
endif

" http://d.hatena.ne.jp/dayflower/20081208/1228725403
" one liner comment will not be not auto-indented
autocmd FileType * let &l:comments =
    \join(filter(split(&l:comments, ','), 'v:val =~ "^[sme]"'), ',')


" detectindent http://www.vim.org/scripts/script.php?script_id=1171
let g:detectindent_preferred_expandtab = 1
let g:detectindent_preferred_indent = 2
autocmd BufReadPost * :DetectIndent
autocmd BufReadPost *.php set shiftwidth=4
autocmd BufReadPost *.php set tabstop=4
autocmd BufReadPost *.php set expandtab
autocmd BufReadPost *.tpl set noexpandtab


" https://github.com/jiangmiao/simple-javascript-indenter
let g:SimpleJsIndenter_BriefMode = 1


" https://github.com/nathanaelkane/vim-indent-guides
" turn on indent-guide by <Leader>ig
let g:indent_guides_auto_colors = 0
let g:indent_guides_start_level = 2
let g:indent_guides_guide_size = 1
autocmd VimEnter,Colorscheme * :hi IndentGuidesOdd  guibg=Black   ctermbg=Black
autocmd VimEnter,Colorscheme * :hi IndentGuidesEven guibg=DarkGray ctermbg=White


" disable bell
"":set visualbell t_vb=
autocmd VimEnter * set vb t_vb=""


" Python
autocmd BufEnter SConstruct set filetype=python
autocmd BufEnter SConscript set filetype=python
" ~/.crontab is a crontab file (crontab ~/.crontab)
au Bufenter .crontab set filetype=crontab

autocmd Bufenter .php set tabstop=4
autocmd Bufenter .php set shiftwidth=4
autocmd Bufenter .tpl set noet

