export ZSH=$HOME/.oh-my-zsh
ZSH_THEME=""
COMPLETION_WAITING_DOTS="true"
plugins=(git cargo rbenv nvm npm gem)

source $ZSH/oh-my-zsh.sh
source $HOME/.aliases

# Use custom theme.
ZSH_THEME_GIT_PROMPT_PREFIX=" [%{%F{blue}%}"
ZSH_THEME_GIT_PROMPT_SUFFIX="%{%f%k%b%F{green}%}]"
ZSH_THEME_GIT_PROMPT_DIRTY=" %{%F{yellow}%}*%{%f%k%b%}"
ZSH_THEME_GIT_PROMPT_CLEAN=""
PROMPT='%{%f%k%b%} %{%b%F{yellow}%}%~%{%F{green}%}$(git_prompt_info)%E%{%f%k%b%} %# %{%f%k%b%}'

for f in $HOME/.zshrc.d/*.sh; do
    source $f
done
