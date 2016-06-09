export ZSH=/home/vlad/.oh-my-zsh
ZSH_THEME="robbyrussell"
COMPLETION_WAITING_DOTS="true"

plugins=(git github grunt nvm)

# User configuration
export PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
export MANPATH="/usr/local/man:$MANPATH"
source $ZSH/oh-my-zsh.sh
source $HOME/.aliases

# export CC="ccache clang"
# export CXX="ccache clang++"
# export PATH="/usr/lib/ccache/:$PATH"
# export CCACHE_CPP2=yes
