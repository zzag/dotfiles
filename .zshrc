source $HOME/.antigen/antigen.zsh

# Set preferred editor.
if which nvim > /dev/null 2>&1; then
    export EDITOR=nvim
elif which vim > /dev/null 2>&1; then
    export EDITOR=vim
else
    export EDITOR=nano
fi

# Bundles from oh-my-zsh.
antigen use oh-my-zsh
antigen bundle git
antigen bundle command-not-found

# Syntax highlighting bundle.
ZSH_HIGHLIGHT_HIGHLIGHTERS=(main brackets)

typeset -A ZSH_HIGHLIGHT_STYLES
ZSH_HIGHLIGHT_STYLES[path]='fg=white'

antigen bundle zsh-users/zsh-syntax-highlighting

# Anaconda support.
antigen bundle zzag/zsh-conda

# NVM
export NVM_DIR="$HOME/.nvm"
antigen bundle zzag/zsh-nvm

# Use pure zsh prompt.
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
PURE_PROMPT_SYMBOL='$'

antigen bundle mafredri/zsh-async
antigen bundle sindresorhus/pure

# We're done.
antigen apply

source $HOME/.aliases
