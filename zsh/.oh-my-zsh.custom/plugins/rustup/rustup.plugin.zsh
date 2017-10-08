# Vars
#
# ZSHRC_RUSTUP_ROOT : string

if [[ ! $ZSHRC_RUSTUP_ROOT ]]; then
    ZSHRC_RUSTUP_ROOT=$HOME/.cargo
fi

if [[ -d $ZSHRC_RUSTUP_ROOT ]]; then
    export PATH="$ZSHRC_RUSTUP_ROOT/bin:$PATH"
fi
