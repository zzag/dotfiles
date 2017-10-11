# Based on bira.zsh-theme

#
# user & hostname
#

if [[ $UID -eq 0 ]]; then
    local _user_host="%{$terminfo[bold]$fg[red]%}%n@%m%{$reset_color%}"
    local _user_symbol="#"
else
    local _user_host="%{$terminfo[bold]$fg[green]%}%n@%m%{$reset_color%}"
    local _user_symbol="$"
fi

local _current_dir="%{$terminfo[bold]$fg[blue]%}%~%{$reset_color%}"


#
# conda env name
#

local _conda_env () {
  if [[ $CONDA_DEFAULT_ENV ]]; then
    echo "  %{$terminfo[bold]$fg[blue]%}‹\ue73c $CONDA_DEFAULT_ENV›%{$reset_color%}"
  fi
}


#
# nvm version
#

local _nvm_ver () {
  if [[ $commands[node] ]]; then
    echo "  %{$terminfo[bold]$fg[yellow]%}‹\ue781 $(node -v)›%{$%{$reset_color%}"
  fi
}


#
# rbenv version
#

local _rbenv_ver () {
  if [[ $commands[rbenv] ]]; then
    echo "  %{$terminfo[bold]$fg[red]%}‹\ue791 $(rbenv version-name)›%{$%{$reset_color%}"
  fi
}


#
# git branch
#

local _git_branch () {
  echo "$(git_prompt_info)%{$reset_color%}"
}

ZSH_THEME_GIT_PROMPT_PREFIX="%{$terminfo[bold]$fg[yellow]%}‹\ue0a0 "
ZSH_THEME_GIT_PROMPT_SUFFIX="› %{$reset_color%}"
ZSH_THEME_GIT_PROMPT_DIRTY="*"


#
# return code
#

local _return_code() {
  echo "%(?..%{$fg[red]%}%? \ue231%{$reset_color%})"
}


PROMPT='╭─${_user_host} ${_current_dir} $(_git_branch)
╰─%B${_user_symbol}%b '
RPS1='%B$(_return_code)$(_conda_env)%b'
