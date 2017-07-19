if [[ -d "$HOME/Android/Sdk" ]]; then
    export ANDROID_HOME=$HOME/Android/Sdk
    export PATH="$PATH:$ANDROID_HOME/tools:$ANDROID_HOME/platform-tools"
fi

