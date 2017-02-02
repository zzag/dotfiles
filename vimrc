call plug#begin('~/.vim/plugged')
Plug 'morhetz/gruvbox'
Plug 'jiangmiao/auto-pairs'
Plug 'fatih/vim-go'
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


"
" Editing
"

" smart indent
set si

"
" Linux:
"

autocmd FileType c call ConfigureLinuxStyle()
autocmd FileType diff set ts=8
autocmd FileType kconfig call ApplyLinuxCodingStyle()
autocmd FileType dts call ApplyLinuxCodingStyle()

function! ConfigureLinuxStyle()
	call ApplyLinuxCodingStyle()
	call AddLinuxKeywords()
endfunction

function! ApplyLinuxCodingStyle()
	set tabstop=8
	set softtabstop=8
	set shiftwidth=8
	set textwidth=80
	set noexpandtab
endfunction

function! AddLinuxKeywords()
	syn keyword cOperator likely unlikely
	syn keyword cType u8 u16 u32 u64
	syn keyword cType s8 s16 s32 s64
	syn keyword cType __u8 __u16 __u32 __u64
	syn keyword cType __s8 __s16 __s32 __s64
endfunction

" Python:
autocmd FileType python set ts=4 sw=4 sts=4 et

" HTML:
autocmd FileType html set ts=2 sw=2 sts=2 et

" JavaScript:
autocmd FileType javascript set ts=4 sw=4 sts=4 et
