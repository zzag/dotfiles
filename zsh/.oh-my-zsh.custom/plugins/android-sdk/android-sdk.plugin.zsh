if [[ ! $ANDROID_HOME ]]; then
    export ANDROID_HOME=$HOME/Android/Sdk
fi

if [[ -d $ANDROID_HOME ]]; then
    export PATH="$PATH:$ANDROID_HOME/tools:$ANDROID_HOME/platform-tools"
fi

