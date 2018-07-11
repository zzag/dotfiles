export GOPATH=$HOME/Go
export PATH=$GOPATH/bin:$PATH
export PATH=$HOME/.local/bin:$PATH

export SSH_ASKPASS=/usr/bin/ksshaskpass
eval $(keychain --eval --quiet --noask --agents ssh id_github id_kde)
