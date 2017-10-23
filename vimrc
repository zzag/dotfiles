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

" UI
Plug 'junegunn/fzf', { 'dir': '~/.fzf', 'do': './install --all' }
Plug 'junegunn/fzf.vim'
Plug 'christoomey/vim-tmux-navigator'
Plug 'scrooloose/nerdtree'
Plug 'Yggdroot/indentLine'
Plug 'vim-airline/vim-airline'
Plug 'junegunn/goyo.vim'
Plug 'junegunn/limelight.vim'

" Editing
Plug 'mattn/emmet-vim'
Plug 'jiangmiao/auto-pairs'
Plug 'junegunn/vim-easy-align'
Plug 'tpope/vim-surround'
Plug 'tpope/vim-commentary'
Plug 'editorconfig/editorconfig-vim'

" Themes
Plug 'morhetz/gruvbox'
Plug 'vim-airline/vim-airline-themes'
call plug#end()


" ==================================================
"                     GENERAL
" ==================================================

" Break compability with Vi. In fact, this setting do nothing.
" If file `~/.vimrc` exists, vim automatically turns on
" `nocompatible` mode.
set nocompatible

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
set cursorline
set lazyredraw

" Minimal number of screen lines to keep above and below the cursor.
set scrolloff=5

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


" ==================================================
"                      THEME
" ==================================================

" Use TrueColor.
set termguicolors

set background=dark
colorscheme gruvbox


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

" Editing helpers.
cnoremap %% <C-R>=fnameescape(expand('%:h')).'/'<cr>
map <leader>ew :e %%
map <leader>es :sp %%
map <leader>ev :vsp %%
map <leader>et :tabe %%

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


" ==================================================
"                     PLUGINS
" ==================================================

" --------------------------------------------------
" vim-airline
" --------------------------------------------------

set laststatus=2
let g:airline_powerline_fonts=1


" --------------------------------------------------
" indentLine
" --------------------------------------------------

" let g:indentLine_char = '┆'


" --------------------------------------------------
" vim-json
" --------------------------------------------------

" Show double quotes in json files.
let g:vim_json_syntax_conceal=0


" --------------------------------------------------
" emmet-vim
" --------------------------------------------------

" Use emmet with <C-Z>,
let g:user_emmet_leader_key='<C-Z>'


" --------------------------------------------------
" nerdtree
" --------------------------------------------------

" Toggle nerd tree.
map <C-n> :NERDTreeToggle<CR>


" --------------------------------------------------
" fzf.vim
" --------------------------------------------------

let g:fzf_colors =
\ { 'fg':      ['fg', 'Normal'],
  \ 'bg':      ['bg', 'Normal'],
  \ 'hl':      ['fg', 'Comment'],
  \ 'fg+':     ['fg', 'CursorLine', 'CursorColumn', 'Normal'],
  \ 'bg+':     ['bg', 'Normal'],
  \ 'hl+':     ['fg', 'Statement'],
  \ 'info':    ['fg', 'PreProc'],
  \ 'border':  ['fg', 'Ignore'],
  \ 'prompt':  ['fg', 'Conditional'],
  \ 'pointer': ['fg', 'Exception'],
  \ 'marker':  ['fg', 'Keyword'],
  \ 'spinner': ['fg', 'Label'],
  \ 'header':  ['fg', 'Comment'] }

let g:fzf_layout = { 'down': '~40%' }


" --------------------------------------------------
" goyo.vim
" --------------------------------------------------

function! s:goyo_enter()
    if exists('$TMUX')
        silent !tmux set status off
        silent !tmux list-panes -F '\#F' | grep -q Z || tmux resize-pane -Z
    endif
    set noshowmode
    set noshowcmd
    Limelight
endfunction

function! s:goyo_leave()
    if exists('$TMUX')
        silent !tmux set status on
        silent !tmux list-panes -F '\#F' | grep -q Z && tmux resize-pane -Z
    endif
    set showmode
    set showcmd
    Limelight!
endfunction

autocmd! User GoyoEnter nested call <SID>goyo_enter()
autocmd! User GoyoLeave nested call <SID>goyo_leave()


" --------------------------------------------------
" vim-easy-align
" --------------------------------------------------

" Start interactive EasyAlign in visual mode(e.g. vipga).
xmap ga <Plug>(EasyAlign)

" Start interactive EasyAlign for a motion/text object(e.g. gaip).
nmap ga <Plug>(EasyAlign)


" --------------------------------------------------
" vim-commentary
" --------------------------------------------------

" xmap gc  <Plug>Commentary
" nmap gc  <Plug>Commentary
" omap gc  <Plug>Commentary
" nmap gcc <Plug>CommentaryLine
