# Use ccache.
if type -q ccache
    set -gx fish_user_paths /usr/lib/ccache/bin $fish_user_paths
end

if type -q rustup
    set -gx fish_user_paths $HOME/.cargo/bin $fish_user_paths
end

# Use portals to ensure that GTK applications use native file dialogs.
set -gx GTK_USE_PORTAL 1

# For pixel perfect scrolling in Firefox.
set -gx MOZ_USE_XINPUT2 1
