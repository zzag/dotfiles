# Language environment.
export LANG=en_US.UTF-8

# Preferred editor.
if which nvim > /dev/null 2>&1; then
    export EDITOR=nvim
elif which vim > /dev/null 2>&1; then
    export EDITOR=vim
else
    export EDITOR=nano
fi
