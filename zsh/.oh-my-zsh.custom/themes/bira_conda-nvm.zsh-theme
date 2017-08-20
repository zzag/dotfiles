# Based on bira.zsh-theme

if [[ $UID -eq 0 ]]; then
    local _user_host="%{$terminfo[bold]$fg[red]%}%n@%m%{$reset_color%}"
    local _user_symbol="#"
else
    local _user_host="%{$terminfo[bold]$fg[green]%}%n@%m%{$reset_color%}"
    local _user_symbol="$"
fi

local _current_dir="%{$terminfo[bold]$fg[blue]%}%~%{$reset_color%}"

_python_ver () {
  if which python &> /dev/null; then
    echo "%{$terminfo[bold]$fg[blue]%}\ue73c $(python -V 2>&1 | egrep -o '([0-9]\.)+[0-9]+')%{$reset_color%}  "
  fi
}

_node_ver () {
  if which node &> /dev/null; then
    echo "%{$terminfo[bold]$fg[yellow]%}\ue781 $(node -v)%{$reset_color%}  "
  fi
}

_ruby_ver () {
  if which ruby &> /dev/null; then
    echo "%{$terminfo[bold]$fg[red]%}\ue791 $(ruby --version 2>&1 | egrep -o '([0-9]+\.)+[0-9]+')%{$reset_color%}  "
  fi
}

_git_branch () {
  echo "$(git_prompt_info)%{$reset_color%}"
}

local _return_code="%(?..%{$fg[red]%}%? ↵%{$reset_color%})"

ZSH_THEME_GIT_PROMPT_PREFIX="%{$terminfo[bold]$fg[yellow]%}\ue702 "
ZSH_THEME_GIT_PROMPT_SUFFIX=" %{$reset_color%}"

PROMPT='╭─${_user_host} ${_current_dir} $(_ruby_ver)$(_python_ver)$(_node_ver)$(_git_branch)
╰─%B${_user_symbol}%b '
RPS1="%B${_return_code}%b"
