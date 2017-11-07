# Based on bira.zsh-theme

# ============================================================
#                          SEGMENTS
# ============================================================

# _segments shows all segments specified in the
# ZSH_THEME_FOOBAR_SEGMENTS. A segment is any text
# surrounded by '‹' and '›'.
local _segments () {
  local sep=${ZSH_THEME_FOOBAR_SEGMENTS_SEP:-" "}
  for s ($ZSH_THEME_FOOBAR_SEGMENTS); do
    eval "_segment_$s"
  done
}

# _segment prints single segment. The first argument is style of the
# segment(e.g. bold), the second argument is color of the segment
# and the third argument is body of the segment.
# TODO: choose better name
local _segment () {
  echo -n "%{$terminfo[$1]$fg[$2]%}"
  [[ -n $3 ]] && echo -n $3
  echo -n "%{$reset_color%} "
}

# _segment_conda shows currently activated conda environment.
local _segment_conda () {
  if [[ $CONDA_DEFAULT_ENV ]]; then
    _segment bold blue "‹$CONDA_DEFAULT_ENV›"
  fi
}

# _segment_nvm shows currently activated "node.js" version.
local _segment_nvm () {
  if [[ $commands[nvm] ]]; then
    _segment bold yellow "‹$(nvm current)›"
  fi
}

# _segment_rbenv shows currently activated rbenv version.
local _segment_rbenv () {
  if [[ $commands[rbenv] ]]; then
    _segment bold red "‹$(rbenv version-name)›"
  fi
}

# _segment_gvm shows currently activated gvm version.
local _segment_gvm () {
  if [[ $commands[gvm-prompt] ]]; then
    local go_env=$(gvm-prompt)
    if [[ $go_env != "" ]]; then
      _segment bold cyan "‹$go_env›"
    fi
  fi
}

# _segment_git shows git branch.
ZSH_THEME_GIT_PROMPT_PREFIX="‹"
ZSH_THEME_GIT_PROMPT_SUFFIX="›"
ZSH_THEME_GIT_PROMPT_DIRTY="*"

local _segment_git () {
  _segment bold yellow "$(git_prompt_info)"
}


# ============================================================
#     USERNAME, HOSTNAME, CURRENT DIRECTORY, RETURN CODE
# ============================================================

# TODO: refactor

if [[ $UID -eq 0 ]]; then
    local _user_host="%{$terminfo[bold]$fg[red]%}%n@%m%{$reset_color%}"
    local _user_symbol="#"
else
    local _user_host="%{$terminfo[bold]$fg[green]%}%n@%m%{$reset_color%}"
    local _user_symbol="$"
fi

local _current_dir="%{$terminfo[bold]$fg[blue]%}%~%{$reset_color%}"

local _return_code () {
  echo "%(?..%{$terminfo[bold]$fg[red]%}%? ↵%{$reset_color%})"
}


# ============================================================
#                          PROMPT..
# ============================================================

PROMPT='╭─${_user_host} ${_current_dir} $(_segments)
╰─%B${_user_symbol}%b '
RPS1='%B$(_return_code)%b'

# vim:ft=zsh ts=2 sw=2 sts=2
