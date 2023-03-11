# Use ccache.
if type -q ccache
    set -gx fish_user_paths /usr/lib/ccache/bin $fish_user_paths
end

# Enable stem darkening (so fonts look more consistent in Qt and GTK apps)
set -gx FREETYPE_PROPERTIES "truetype:interpreter-version=40 cff:no-stem-darkening=0 cff:darkening-parameters=500,500,1000,350,1667,350,2333,0"
