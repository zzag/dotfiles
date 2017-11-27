# ============================================================
#                            GIT
# ============================================================

ZSH_THEME_GIT_PROMPT_PREFIX="%{\e[3m%}git%{\e[23m%} %{$FG[245]$terminfo[bold]%}"
ZSH_THEME_GIT_PROMPT_SUFFIX="%{$reset_color%}"


# ============================================================
#     USERNAME, HOSTNAME, CURRENT DIRECTORY, RETURN CODE
# ============================================================

local _user="%{$fg_bold[magenta]%}%n%{$reset_color%}"
local _host="%{$fg_bold[magenta]%}%m%{$reset_color%}"
local _current_dir="%{$fg_bold[blue]%}%~%{$reset_color%}"
local _ret_status="%(?:%{$fg_bold[white]%}❯:%{$fg_bold[red]%}❯)%{$reset_color%}"


# ============================================================
#                          PROMPT..
# ============================================================

PROMPT=$'${_user} %{\e[3m%}at%{\e[23m%} ${_host} %{\e[3m%}in%{\e[23m%} ${_current_dir} $(git_prompt_info)\n %B${_ret_status}%b '
RPS1=''


# Add new line after each command.
local prompt_foobar_precmd() {
  if [[ $ZSH_PROMPT_FOOBAR_NEW_LINE ]] then
    print ''
  else
    ZSH_PROMPT_FOOBAR_NEW_LINE=true
  fi
}

local prompt_foobar_setup() {
  autoload -U add-zsh-hook
  add-zsh-hook precmd prompt_foobar_precmd
}

prompt_foobar_setup

# vim:ft=zsh ts=2 sw=2 sts=2
