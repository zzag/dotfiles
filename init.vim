" Preferred font: Fira Mono, 14

call plug#begin('~/.local/share/nvim/plugged')
Plug 'morhetz/gruvbox'
Plug 'jiangmiao/auto-pairs'
Plug 'fatih/vim-go'
Plug 'vim-airline/vim-airline'
Plug 'vim-airline/vim-airline-themes'
Plug 'Yggdroot/indentLine'
Plug 'rust-lang/rust.vim'
Plug 'pangloss/vim-javascript'
Plug 'junegunn/fzf', { 'dir': '~/.fzf', 'do': './install --all' }
Plug 'christoomey/vim-tmux-navigator'
Plug 'scrooloose/nerdtree'
Plug 'elzr/vim-json'
Plug 'mattn/emmet-vim'
call plug#end()


"
" General
"

set nocompatible

" set history size
set history=1000

" enable filetype plugins
filetype plugin on
filetype indent on

" read a file if it has been changed outside
set autoread

" uncomment to highlight current line
set cursorline
set lazyredraw


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

syntax enable
set termguicolors
set background=dark
colorscheme gruvbox

hi Visual gui=NONE guibg=#fabd2f guifg=#282828

" uncomment to make background transparent
autocmd VimEnter * hi Normal guibg=NONE

" airline
set laststatus=2
let g:airline_powerline_fonts=1

" let g:indentLine_char = '┆'

" show double quotes in json files
let g:vim_json_syntax_conceal=0

" resize splits like a boss
"  < - decrease width
"  > - increase width
"  - - decrease height
"  + - increase height
nnoremap <C-Left>   <C-w><
nnoremap <C-Right>  <C-w>>
nnoremap <C-Up>     <C-w>+
nnoremap <C-Down>   <C-w>-

" Toggle nerd tree
map <C-n> :NERDTreeToggle<CR>

"
" Editing
"

" Use emmet with <C-Z>,
let g:user_emmet_leader_key='<C-Z>'

" smart indent
set si

set ts=4
set sw=4
set sts=4
set et

" HTML:
autocmd FileType html set ts=2 sw=2 sts=2 et
