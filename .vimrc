if has('nvim')
  call plug#begin('~/.local/share/nvim/plugged')
else
  call plug#begin('~/.vim/plugged')
endif

" Languages
Plug 'rust-lang/rust.vim'
Plug 'pangloss/vim-javascript'
Plug 'fatih/vim-go'
Plug 'elzr/vim-json'
Plug 'peterhoeg/vim-qml'

" UI
Plug 'christoomey/vim-tmux-navigator'
Plug 'preservim/nerdtree'
Plug 'Yggdroot/indentLine'
Plug 'joshdick/onedark.vim'
Plug 'sheerun/vim-polyglot'

" Editing
Plug 'junegunn/vim-easy-align'
Plug 'tpope/vim-surround'
Plug 'editorconfig/editorconfig-vim'
Plug 'preservim/nerdcommenter'
Plug 'Raimondi/delimitMate'
Plug 'chiel92/vim-autoformat'

call plug#end()


" ==================================================
"                     GENERAL
" ==================================================

" Break compability with Vi. In fact, this setting do nothing.
" If file `~/.vimrc` exists, vim automatically turns on
" `nocompatible` mode.
set nocompatible

" Disable swap files.
set noswapfile

" Set history size.
set history=1000

" Enable filetype plugins.
filetype plugin on
filetype indent on

" Enable syntax highlighting.
syntax on

" Read a file if it has been changed outside.
set autoread

" Highlight current line.
set lazyredraw
set cursorline

" Minimal number of screen lines to keep above and below the cursor.
set scrolloff=3

" Show line numbers.
set relativenumber
set numberwidth=5

" Highlight matching brackets.
set showmatch
set mat=3

" Turn on wildmenu.
set wildmenu
set wildmode=longest:list:full

" Turn off error sounds.
set noerrorbells
set novisualbell
set t_vb=
set tm=500

" Show command in status line.
set showcmd

" Display invisible characters.
set list listchars=tab:»·,trail:·,nbsp:·

" Quickly time out on key codes, but never time out on mappings.
set notimeout
set ttimeout
set ttimeoutlen=300

" Set custom line continuation character.
set showbreak=…

" Enable mouse.
if has('mouse')
  set mouse=a
endif

" Enable loading project specific config files.
" set exrc
" set secure

" Override italics escape sequence.
"
" YOU SHOULDN'T OVERRIDE IT. INSTEAD, CHANGE YOUR
" TERM DATABASE, SEE BELOW.
"
" Make sure your terminal supports italics:
"   $ echo -e "\e[3mfoobar\e[23m"
"
" Check values of sitm and ritm:
"   $ infocmp $TERM | grep sitm
"   $ infocmp $TERM | grep ritm
"
" it should be \E[3m and \E[23m, respectively.
" If not then make a new TERM database with correct
" sitm and ritm values:
"   $ tic $TERM-italic.terminfo
"
" and set TERM to the new value:
"   $ echo "$TERM-italic" >> ~/.zshrc # or .bashrc
"
set t_ZH=[3m
set t_ZR=[23m

" Enable TrueColor support.
" (available on Vim>=8.0 or Neovim)
if has("termguicolors")
  let &t_8f="\<Esc>[38;2;%lu;%lu;%lum"
  let &t_8b="\<Esc>[48;2;%lu;%lu;%lum"
  set termguicolors
endif


" ==================================================
"                      INDENT
" ==================================================

set cindent
set cinoptions+=g0  " Don't indent access modifiers
set cinoptions+=:0  " Don't indent case labels
set cinoptions+=l1  " Don't align case statements
set cinoptions+=N-s " Don't indent inside namespaces
set cinoptions+=E-s " Don't indent inside extern


" ==================================================
"                      THEME
" ==================================================

set background=dark
colorscheme onedark

" Enable italics.
let g:onedark_terminal_italics = 1

" Show comments in italics.
" (should be set after colorscheme)
highlight Comment cterm=italic


" ==================================================
"                      SEARCH
" ==================================================

" Ignore case when searching.
set ignorecase

" Use smartcase.
set smartcase

" Highlight search results.
set hlsearch

" Turn on incremental search.
set incsearch


" ==================================================
"                     EDITING
" ==================================================

set smartindent
set tabstop=4
set shiftwidth=4
set softtabstop=4
set expandtab

" Delete over autoindent, line breaks and the start of insert.
set backspace=indent,eol,start

" ==================================================
"                   KEY MAPPINGS
" ==================================================

" Map <leader> to the ','(default is '\').
let mapleader = ','

" Clear search highlighting.
nmap <silent> <leader>c :nohlsearch<CR>

" Yank from the cursor to the end of the line.
nnoremap Y y$

" Resize splits like a boss.
"  < - decrease width
"  > - increase width
"  - - decrease height
"  + - increase height
nnoremap <C-Left>   <C-w><
nnoremap <C-Right>  <C-w>>
nnoremap <C-Up>     <C-w>+
nnoremap <C-Down>   <C-w>-

" Disable arrow keys in normal mode.
" (arrow keys are still useful in insert mode)
noremap <Left>   <Nop>
noremap <Right>  <Nop>
noremap <Up>     <Nop>
noremap <Down>   <Nop>


" ==================================================
"                       MISC
" ==================================================

" Jump to the last known cursor position.
autocmd BufReadPost *
      \ if line("'\"") >= 1 && line("'\"") <= line("$") |
      \   execute "normal! g`\"" |
      \ endif

" Enable spellchecking for git commit messages.
function ConfigureGitCommit()
  if has('spell')
    setlocal spell
  endif
endfunction

autocmd FileType gitcommit call ConfigureGitCommit()


" ==================================================
"                     PLUGINS
" ==================================================

" --------------------------------------------------
" indentLine
" --------------------------------------------------

" let g:indentLine_char = '│'


" --------------------------------------------------
" vim-json
" --------------------------------------------------

" Show double quotes in json files.
let g:vim_json_syntax_conceal=0


" --------------------------------------------------
" nerdtree
" --------------------------------------------------

" Toggle nerd tree.
map <C-n> :NERDTreeToggle<CR>


" --------------------------------------------------
" vim-easy-align
" --------------------------------------------------

" Start interactive EasyAlign in visual mode(e.g. vipga).
xmap ga <Plug>(EasyAlign)

" Start interactive EasyAlign for a motion/text object(e.g. gaip).
nmap ga <Plug>(EasyAlign)


" --------------------------------------------------
" nerdcommenter
" --------------------------------------------------

" Add spaces after comment delimiters by default.
let g:NERDSpaceDelims=1

" Use compact syntax for prettified multi-line comments.
let g:NERDCompactSexyComs=1

" Align line-wise comment delimiters flush left instead of following
" code indentation.
let g:NERDDefaultAlign='left'

" Allow commenting and inverting empty lines.
let g:NERDCommentEmptyLines=1

" Enable trimming of trailing whitespace when uncommenting.
let g:NERDTrimTrailingWhitespace=1


" --------------------------------------------------
" vim-autoformat
" --------------------------------------------------

" Format when F3 is pressed.
noremap <F3> :Autoformat<CR>

" Format C/C++ files on save.
" autocmd BufWrite *.h :Autoformat
" autocmd BufWrite *.c :Autoformat
" autocmd BufWrite *.hh :Autoformat
" autocmd BufWrite *.cc :Autoformat
" autocmd BufWrite *.hpp :Autoformat
" autocmd BufWrite *.cpp :Autoformat


" ==================================================
"                       EOF
" ==================================================

" vim:ft=vim ts=2 sw=2 sts=2
