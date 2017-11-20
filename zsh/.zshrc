export ZSH=$HOME/.oh-my-zsh
ZSH_CUSTOM=$HOME/.oh-my-zsh.custom
COMPLETION_WAITING_DOTS="true"

# Available segments:
#  - conda
#  - rbenv
#  - nvm
#  - gvm
#  - git
#
ZSH_THEME_FOOBAR_SEGMENTS=(gvm conda git)
ZSH_THEME="foobar"

# User defined plugins:
#  - android-sdk
#  - conda
#  - fzf
#  - gvm
#  - rustup
#
plugins=(git git-prompt     # git
         nvm npm            # js: node.js
         gvm                # go: gvm
         conda              # python: anaconda
         rustup             # rust: rustup
         )

source $ZSH/oh-my-zsh.sh
source $HOME/.aliases

# The greeting dependencies:
#  - cowsay
#  - lolcat
#
ZSHRC_SHOW_GREETING=false

for f in $HOME/.zshrc.d/*.sh; do
    source $f
done
