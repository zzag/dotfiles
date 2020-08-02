# Use ccache.
if type -q ccache
    set -gx fish_user_paths /usr/lib/ccache/bin $fish_user_paths
end

set -gx GTK_USE_PORTAL 1
set -gx MOZ_USE_XINPUT2 1
