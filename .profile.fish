# Use ccache.
if type -q ccache
    set -gx fish_user_paths /usr/lib/ccache/bin $fish_user_paths
end

if type -q rustup
    set -gx fish_user_paths $HOME/.cargo/bin $fish_user_paths
end

# Enable Wayland support in Firefox and Thunderbird.
set -gx MOZ_ENABLE_WAYLAND 1
