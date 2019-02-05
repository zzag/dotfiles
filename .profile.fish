# Load keychain.
if type -q keychain
    keychain --quiet --noask --agents ssh id_github id_kde
end

if test -f ~/.keychain/(hostname)-fish
    source ~/.keychain/(hostname)-fish
end

# Use ccache.
if type -q ccache
    set -gx fish_user_paths /usr/lib/ccache/bin $fish_user_paths
end
