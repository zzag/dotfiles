# Vars
#
# ZSHRC_ANACONDA_ROOT : string

if [[ ! $ZSHRC_ANACONDA_ROOT ]]; then
    ZSHRC_ANACONDA_ROOT=$HOME/Anaconda
fi

if [[ -d $ZSHRC_ANACONDA_ROOT ]]; then
    export PATH="$PATH:$ZSHRC_ANACONDA_ROOT/bin"
fi
