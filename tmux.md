# tmux

## Sessions

Session is a set of windows.


Keyboard shortcuts:

```
C-b (     previous session
C-b )     next session
C-b s     switch session
C-b $     rename session
C-b d     detach from a session
```

#### Create session

```
tmux new -s <session_name>
```

#### Attach to an existing session

```
tmux attach -t <session_name>
```

#### Detach from a session

```
tmux detach
```

#### Switch to another session

```
tmux switch -t <session_name>
```

#### List sessions

```
tmux list-sessions
```

### Rename session

```
tmux rename <session_name>
```


## Windows

Keyboard shortcuts:

```
C-b c       new window
C-b &       kill window
C-b ,       rename window
C-b 123     switch to window
C-b p       previous window
C-b n       next window
C-b w       choose window from a list
```


#### New window

```
tmux new-window
```

#### Switch to another window

```
tmux select-window -t <window_number>
```

#### Rename window

```
tmux rename-window <window_name>
```


## Pane

Rectangular area of a window.

Keyboard shortcuts:

```
C-b "       split vertically
C-b %       split horizontally
C-b left    move to the next pane on the left
C-b right                                right
C-b up
C-b down
C-b x       kill pane
C-b q       display pane numbers
C-b {       swap panes
C-b }
```

#### Split vertically

```
tmux split-window
```

#### Split horizontally

```
tmux split-window -h
```

#### Swap panes

```
tmux swap-panes -UDLR
```

#### Move to the next pane

```
tmux select-pane -UDLR
```

or

```
tmux select-pane -t <pane_number>
```
