call plug#begin('~/.local/share/nvim/plugged')
Plug 'morhetz/gruvbox'
Plug 'jiangmiao/auto-pairs'
Plug 'fatih/vim-go'
Plug 'vim-airline/vim-airline'
Plug 'Yggdroot/indentLine'
Plug 'rust-lang/rust.vim'
Plug 'junegunn/fzf', { 'dir': '~/.fzf', 'do': './install --all' }
call plug#end()


"
" General
"

" set history size
set history=1000

" enable filetype plugins
filetype plugin on
filetype indent on

" read a file if it has been changed outside
set autoread

" uncomment to highlight current line
"  set cursorline
"  set lazyredraw


"
" UI
"

" 5 lines below and above the cursor
set so=5

" turn on wildmenu
set wildmenu
set wildmode=longest:list:full

" ignore case when searching
set ignorecase

" use smartcase
set smartcase

" highlight search results
set hlsearch

" turn on incremental search
set incsearch

" highlight matching brackets
set showmatch
set mat=3

" turn off error sounds
set noerrorbells
set novisualbell
set t_vb=
set tm=500

" always show current position
set ruler

" show command in status line
set showcmd

" show line numbers
set number
set numberwidth=7

" display invisible characters
set list listchars=tab:»·,trail:·,nbsp:·

set termguicolors
set background=dark
colorscheme gruvbox

highlight Normal guibg=none

" airline
set laststatus=2
let g:airline_powerline_fonts=1

" let g:indentLine_char = '┆'


"
" Editing
"

" smart indent
set si

set ts=4
set sw=4
set sts=4
set et

" HTML:
autocmd FileType html set ts=2 sw=2 sts=2 et
