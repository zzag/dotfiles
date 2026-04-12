# Set preferred editor.
if type -q vim
    set -gx EDITOR vim
else
    set -gx EDITOR nano
end

if test -e ~/.profile.fish
    source ~/.profile.fish
end

# Add $HOME/.cargo/bin to PATH.
if test -d ~/.cargo/bin
    set -gx fish_user_paths ~/.cargo/bin $fish_user_paths
end

# Add $HOME/.local/bin to PATH.
if test -d ~/.local/bin
    set -gx fish_user_paths ~/.local/bin $fish_user_paths
end

# Load aliases.
source ~/.config/fish/aliases.fish

# Remove the greeting banner.
set -U fish_greeting

# Pure fish prompt.
set -gx pure_show_numbered_git_indicator true
