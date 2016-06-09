"
" Dependencies:
"   * pathogen (https://github.com/tpope/vim-pathogen)
"   * gruvbox (https://github.com/morhetz/gruvbox)
"   * powerline-fonts (https://github.com/powerline/fonts)
"   * vim-airline (https://github.com/vim-airline/vim-airline)
"   * nerdtree (https://github.com/scrooloose/nerdtree)
"   * indentLine (https://github.com/Yggdroot/indentLine)
"

execute pathogen#infect()

" general
set t_Co=256
let loaded_matchparen=1
set laststatus=2

filetype indent on
filetype plugin on
syntax on
set smartindent
set autoindent


" softtabs
set tabstop=2
set shiftwidth=2
set shiftround
set expandtab


" max 100 characters
set textwidth=100
set colorcolumn=+1


" numbers
set number
set numberwidth=5


" display trailing whitespace
set list listchars=tab:»·,trail:·,nbsp:·


" gruvbox
set background=dark
colorscheme gruvbox


" indentLine
" let g:indentLine_leadingSpaceEnabled = 1
" let g:indentLine_leadingSpaceChar = '·'


" airline
let g:airline#extensions#tabline#enabled = 1
let g:airline_powerline_fonts = 1


" nerdtree
map <C-\> :NERDTreeToggle<CR>


" tab navigation shortcuts
if &term =~ '^screen'
  execute "set <xUp>=\e[1;*A"
  execute "set <xDown>=\e[1;*B"
  execute "set <xRight>=\e[1;*C"
  execute "set <xLeft>=\e[1;*D"
endif
nnoremap <C-Left> :tabprevious<CR>
nnoremap <C-Right> :tabnext<CR>


" splits
nnoremap <C-H> <C-W><C-H><CR>
nnoremap <C-L> <C-W><C-L><CR>
nnoremap <C-J> <C-W><C-J><CR>
nnoremap <C-K> <C-W><C-K><CR>
