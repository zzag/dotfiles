export ZSH=$HOME/.oh-my-zsh

ZSH_CUSTOM=$HOME/.oh-my-zsh.custom
fpath=("$HOME/.oh-my-zsh.custom/functions" $fpath)

COMPLETION_WAITING_DOTS="true"

# Don't use oh-my-zsh themes.
ZSH_THEME=""

# Set preferred editor.
if which nvim > /dev/null 2>&1; then
    export EDITOR=nvim
elif which vim > /dev/null 2>&1; then
    export EDITOR=vim
else
    export EDITOR=nano
fi

# Available zsh-syntax-highlight highlighters:
#  - main
#  - brackets
#  - pattern
#  - cursor
#  - root
#  - line
#
ZSH_HIGHLIGHT_HIGHLIGHTERS=(main brackets)

typeset -A ZSH_HIGHLIGHT_STYLES
ZSH_HIGHLIGHT_STYLES[path]='fg=white'

# User defined plugins:
#  - android-sdk
#  - conda
#  - fzf
#  - gvm
#  - rustup
#
plugins=(git                      # git
         nvm npm                  # js: node.js
         gvm                      # go: gvm
         conda                    # python: anaconda
         rustup                   # rust: rustup
         zsh-syntax-highlighting  # fish shell like syntax highlighting
         command-not-found        # suggest what to install when running
                                  # an unknown command. On Arch Linux requires
                                  # `pkgfile` package:
                                  #   $ sudo pacman -S pkgfile
                                  #   $ sudo pkgfile --update
         )

source $ZSH/oh-my-zsh.sh
source $HOME/.aliases

# Load pure prompt.
#
# Available options:
#   - PURE_CMD_MAX_EXEC_TIME
#   - PURE_GIT_PULL
#   - PURE_GIT_UNTRACKED_DIRTY
#   - PURE_GIT_DELAY_DIRTY_CHECK
#   - PURE_PROMPT_SYMBOL
#     Default: '❯'
#   - PURE_GIT_DOWN_ARROW
#     Default: '⇣'
#   - PURE_GIT_UP_ARROW
#     Default: '⇡'
#
autoload -U promptinit; promptinit
prompt pure
