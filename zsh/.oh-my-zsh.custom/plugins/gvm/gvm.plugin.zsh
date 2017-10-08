# Vars
#
# ZSHRC_GVM_ROOT : string

if [[ ! $ZSHRC_GVM_ROOT ]]; then
    ZSHRC_GVM_ROOT=$HOME/.gvm
fi

if [[ -d $ZSHRC_GVM_ROOT ]]; then
    source $ZSHRC_GVM_ROOT/scripts/gvm
fi
