export ZSH=$HOME/.oh-my-zsh
ZSH_CUSTOM=$HOME/.oh-my-zsh.custom
ZSH_THEME="bira_conda-nvm"
COMPLETION_WAITING_DOTS="true"
plugins=(git cabal cargo rbenv nvm npm gem)

source $ZSH/oh-my-zsh.sh
source $HOME/.aliases

ZSHRC_USE_FZF=true
ZSHRC_USE_ANACONDA=true
ZSHRC_USE_CABAL=true
ZSHRC_USE_RUSTUP=true
ZSHRC_USE_ANDROID_SDK=true
ZSHRC_USE_EMSCRIPTEN=false
ZSHRC_SHOW_GREETING=false

for f in $HOME/.zshrc.d/*.sh; do
    source $f
done
