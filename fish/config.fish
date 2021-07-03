# Set preferred editor.
if type -q vim
    set -gx EDITOR vim
else
    set -gx EDITOR nano
end

# Add $HOME/.local/bin to PATH.
if test -d ~/.local/bin
    set -gx fish_user_paths ~/.local/bin $fish_user_paths
end

# Load aliases.
source ~/.config/fish/aliases.fish

# Load profile.
if test -f ~/.profile.fish
    source ~/.profile.fish
end

# Customize default colors.
set -g fish_color_command normal
set -g fish_color_param green

# Customize pure prompt.
set -g pure_color_success (set_color green)
