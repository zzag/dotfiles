# Based on bira.zsh-theme

if [[ $UID -eq 0 ]]; then
    local _user_host="%{$terminfo[bold]$fg[red]%}%n@%m%{$reset_color%}"
    local _user_symbol="#"
else
    local _user_host="%{$terminfo[bold]$fg[green]%}%n@%m%{$reset_color%}"
    local _user_symbol="$"
fi

local _current_dir="%{$terminfo[bold]$fg[blue]%}%~%{$reset_color%}"

_conda_env () {
  if [[ -n "${CONDA_DEFAULT_ENV}" ]]; then
    echo "%{$fg[blue]%}‹${CONDA_DEFAULT_ENV}›%{$reset_color%} "
  fi
}

_nvm_ver () {
  if which nvm &> /dev/null; then
    echo "%{$fg[yellow]%}‹$(nvm current)›%{$reset_color%} "
  fi
}

_rbenv_ver () {
  if which rbenv &> /dev/null; then
    echo "%{$fg[red]%}‹$(rbenv version | sed -e "s/ (set.*$//")›%{$reset_color%} "
  fi
}

_git_branch () {
  echo "$(git_prompt_info)%{$reset_color%}"
}

local _return_code="%(?..%{$fg[red]%}%? ↵%{$reset_color%})"

ZSH_THEME_GIT_PROMPT_PREFIX="%{$fg[yellow]%}‹"
ZSH_THEME_GIT_PROMPT_SUFFIX="› %{$reset_color%}"

PROMPT='╭─${_user_host} ${_current_dir} $(_conda_env)$(_nvm_ver)$(_git_branch)
╰─%B${_user_symbol}%b '
RPS1="%B${_return_code}%b"
