function ls
    command ls -vhF --color --group-directories-first $argv
end

function tree
    command tree --dirsfirst $argv
end

function mkdir
    command mkdir -pv $argv
end

function rm
    command rm -iv $argv
end

function mv
    command mv -v $argv
end

function cp
    command cp -v $argv
end

function purge
    ls -A1 | xargs rm -rf $argv
end

function ports
    sudo netstat -vatnp $argv
end

function pacman-delete-orphans
    if set -l orphans (pacman -Qtdq)
        sudo pacman -Rns $orphans
    end
end

function gpl
    git pretty-log $argv
end
