"
" Quick reference
"
"  * map: recursive mapping
"  * noremap: non-recursive mapping
"  * [m]map: recursive mapping that works in m mode
"  * [m]noremap: non-recursive mapping that works in m mode
"     Mode letters:
"       - n: normal mode
"       - i: insert mode
"       - v: visual mode
"       - ...
"


""" no backup or swap files
set nobackup
set noswapfile

""" use vim defaults
set nocompatible

""" switch between multiple buffers
"""   without saving a changed buffer
set hidden

""" show cursor line
set cursorline

""" parens
set showmatch

""" syntax
filetype indent on
filetype plugin on
syntax on
set smartindent
set autoindent

""" tabs
set tabstop=2
set shiftwidth=2
set shiftround
set expandtab

""" max text width
set textwidth=120
set colorcolumn=120

""" line numbers
set number
set numberwidth=5

""" display invisible characters
set list listchars=eol:¬,tab:»·,trail:·,nbsp:·

""" tab completion
set wildmenu
set wildmode=longest:list:full

""" case insensitive search
set ignorecase
set smartcase

""" split navigation
nnoremap <C-H> <C-W><C-H><CR>
nnoremap <C-L> <C-W><C-L><CR>
nnoremap <C-J> <C-W><C-J><CR>
nnoremap <C-K> <C-W><C-K><CR>

""" tab navigation in tmux
if &term =~ '^screen'
  execute "set <xUp>=\e[1;*A"
  execute "set <xDown>=\e[1;*B"
  execute "set <xRight>=\e[1;*C"
  execute "set <xLeft>=\e[1;*D"
endif
nnoremap <C-Left> :tabprevious<CR>
nnoremap <C-Right> :tabnext<CR>

""" show command
set showcmd

""" colors...
if has('nvim')
  let $NVIM_TUI_ENABLE_TRUE_COLOR=1
elseif $TERM == "xterm-256color" || $TERM == "screen"
  set t_Co=256
endif

""" gvim
if has('gui_running')
  set guifont=Hack\ 9
  set guioptions-=m " menubar
  set guioptions-=T " toolbar
  set guioptions-=L " left-hand scrollbar
  set guioptions-=r " right-hand scrollbar
  set guicursor=n-i-v-c:block-Cursor
endif

""" plugins
if has('nvim')
  set rtp+=~/.config/nvim/plugged
else
  set rtp+=/.vim/plugged
endif

call plug#begin()
Plug 'morhetz/gruvbox'
Plug 'vim-airline/vim-airline'
Plug 'junegunn/fzf', { 'dir': '~/.fzf', 'do': './install --all' }
Plug 'junegunn/fzf.vim'
Plug 'rust-lang/rust.vim'
Plug 'tpope/vim-commentary'
Plug 'tpope/vim-surround'
Plug 'tpope/vim-fugitive'
Plug 'lervag/vimtex'
Plug 'junegunn/vim-easy-align'
Plug 'fatih/vim-go'
Plug 'christoomey/vim-tmux-navigator'
Plug 'neomake/neomake'
Plug 'godlygeek/tabular'
Plug 'airblade/vim-gitgutter'
Plug 'majutsushi/tagbar'
Plug 'Yggdroot/indentLine'
call plug#end()

""" color scheme
set background=dark
colorscheme gruvbox

""" airline
set laststatus=2
let g:airline#extensions#tabline#enabled=1
let g:airline_powerline_fonts=1

""" tagbar
nnoremap <F8> :TagbarToggle<CR>
