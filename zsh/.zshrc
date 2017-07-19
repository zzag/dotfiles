export ZSH=$HOME/.oh-my-zsh
ZSH_THEME="blinks"
COMPLETION_WAITING_DOTS="true"
plugins=(git cargo rbenv nvm npm gem)

source $ZSH/oh-my-zsh.sh

for f in $HOME/.zsh_scripts/*.sh; do
    source $f
done

source $HOME/.aliases
