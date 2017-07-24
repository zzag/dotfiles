if $ZSHRC_SHOW_GREETING &&
        which cowsay > /dev/null 2>&1 &&
        which lolcat > /dev/null 2>&1; then
    cowsay -f dragon-and-cow "I'm Mr.Dragon. Look at me!" | lolcat
fi

