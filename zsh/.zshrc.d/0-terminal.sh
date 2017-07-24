if $ZSHRC_BLUR_BEHIND_TERMINAL; then
    terminal=$(xdotool getactivewindow)
    if [ "$terminal" != "" ]; then
        xprop -f _KDE_NET_WM_BLUR_BEHIND_REGION 32c -set _KDE_NET_WM_BLUR_BEHIND_REGION 0 -id $terminal
    fi
fi

