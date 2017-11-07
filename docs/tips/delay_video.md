Delay audio:

```
$ ffmpeg.exe -i "movie.mp4" -itsoffset 3.84 -i "movie.mp4" -map 0:v -map 1:a -vcodec copy -acodec copy "movie-audio-delayed.mp4"
```

Delay video:

```
$ ffmpeg.exe -i "movie.mp4" -itsoffset 3.84 -i "movie.mp4" -map 1:v -map 0:a -vcodec copy -acodec copy "movie-video-delayed.mp4"
```
