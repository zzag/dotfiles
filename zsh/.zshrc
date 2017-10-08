export ZSH=$HOME/.oh-my-zsh
ZSH_CUSTOM=$HOME/.oh-my-zsh.custom
ZSH_THEME="bira_conda-nvm"
COMPLETION_WAITING_DOTS="true"

# User defined plugins:
#  - android-sdk
#  - anaconda
#  - fzf
#  - gvm
#  - rustup

plugins=(git cargo rbenv nvm npm gem
         gvm anaconda rustup fzf)

source $ZSH/oh-my-zsh.sh
source $HOME/.aliases

# The greeting dependencies:
#  - cowsay
#  - lolcat
ZSHRC_SHOW_GREETING=false

for f in $HOME/.zshrc.d/*.sh; do
    source $f
done
