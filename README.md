ploot
=====
`git:/bitreich.org/ploot` - [1.0](gopher://bitreich.org/9/releases/ploot/ploot-v1.0.tar.gz)

Plain text plotting tools.  See also the [man pages](/tool/ploot/man/).

ploot-text
----------
Show plain text string using larger font thanks to braille characters:

```
# ./ploot-text "The quick brown fox jumps over the lazy dog"
⠉⡏⠁⡧⢄⢀⣤⡄⠀⢀⠤⡄⡄⢠⠠⡅⢀⠤⠄⡇⡠⠀⠀⡧⢄⢠⡠⠄⡠⢄⢠⢀⢠⢠⠤⡀⠀⢰⡉⢀⠤⡀⢄⡠⠀⠀⢬⠀⡄⢠⢠⢤⢄⢠⠤⡀⣠⠤⠀⠀⡠⢄⢠⠀⢠⢀⣤⡄⣄⠤⠀⠀⡧⠄⡧⢄⢀⣤⡄⠀⠈⡇⢀⠤⡄⠤⡤⢠⠀⡄⠀⢀⠤⡇⡠⢄⢀⠤⡄
⠀⠃⠀⠃⠘⠈⠒⠂⠀⠈⠒⡇⠑⠚⠀⠓⠈⠒⠂⠋⠑⠀⠀⠓⠊⠘⠀⠀⠑⠊⠈⠊⠊⠘⠀⠃⠀⠘⠀⠈⠒⠁⠊⠑⠀⠀⡸⠀⠑⠚⠘⠘⠘⢸⠒⠁⠒⠋⠀⠀⠑⠊⠀⠑⠁⠈⠒⠂⠃⠀⠀⠀⠑⠂⠃⠘⠈⠒⠂⠀⠐⠓⠈⠒⠃⠚⠒⠈⣒⠇⠀⠈⠒⠃⠑⠊⠈⣒⠇

# ./ploot-text -2 "The quick brown fox jumps over the lazy dog"
⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀
⠉⡏⠁⡧⠤⡀⡠⠤⡀⠀⠀⠀⡠⠤⡄⡄⠀⡄⠠⡅⠀⡠⠤⡀⡇⢀⠄⠀⠀⠀⡧⠤⡀⣄⠤⠄⡠⠤⡀⡄⡀⡄⡤⠤⡀⠀⠀⠀⣸⣀⠀⡠⠤⡀⡄⠀⡄⠀⠀⠀⠀⢬⠀⡄⠀⡄⡤⡤⡀⡤⠤⡀⡠⠤⠄⠀⠀⠀⡠⠤⡀⡄⠀⡄⡠⠤⡀⣄⠤⠄⠀⠀⠀⢼⠤⠀⡧⠤⡀⡠⠤⡀⠀⠀⠀⠀⡇⠀⠠⠤⡀⠤⠤⡄⡄⠀⡄⠀⠀⠀⡠⠤⡇⡠⠤⡀⡠⠤⡄
⠀⡇⠀⡇⠀⡇⡗⠒⠃⠀⠀⠀⡇⠀⡇⡇⠀⡇⠀⡇⠀⡇⠀⡀⡗⢅⠀⠀⠀⠀⡇⠀⡇⡇⠀⠀⡇⠀⡇⡇⡇⡇⡇⠀⡇⠀⠀⠀⢸⠀⠀⡇⠀⡇⡨⠪⡀⠀⠀⠀⠀⢸⠀⡇⠀⡇⡇⡇⡇⡇⠀⡇⠑⠒⡄⠀⠀⠀⡇⠀⡇⢱⢰⠁⡗⠒⠃⡇⠀⠀⠀⠀⠀⢸⠀⠀⡇⠀⡇⡗⠒⠃⠀⠀⠀⠀⡇⠀⡔⠒⡇⡠⠊⠀⡇⠀⡇⠀⠀⠀⡇⠀⡇⡇⠀⡇⡇⠀⡇
⠀⠁⠀⠁⠀⠁⠈⠉⠀⠀⠀⠀⠈⠉⡇⠈⠉⠁⠈⠉⠀⠈⠉⠀⠁⠀⠁⠀⠀⠀⠉⠉⠀⠁⠀⠀⠈⠉⠀⠈⠈⠀⠁⠀⠁⠀⠀⠀⠈⠀⠀⠈⠉⠀⠁⠀⠁⠀⠀⠀⢄⡸⠀⠈⠉⠁⠁⠁⠁⡏⠉⠀⠉⠉⠀⠀⠀⠀⠈⠉⠀⠀⠁⠀⠈⠉⠀⠁⠀⠀⠀⠀⠀⠀⠉⠁⠁⠀⠁⠈⠉⠀⠀⠀⠀⠈⠉⠀⠈⠉⠁⠉⠉⠁⣈⣉⠇⠀⠀⠀⠈⠉⠁⠈⠉⠀⣈⣉⠇
```

ploot-braille
--------------
Show graph in the form of braille characters:

```
# ./ploot-braille <example.csv
 shortterm
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀├ 0.600
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣧⣾⡀⠀├ 0.400
⠀⠀⠀⠀⠀⠀⡀⠀⡀⠀⡀⠀⢀⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠀⢀⣰⣆⠀⠀⠀⠀⠀⠀⠀⠀⣰⣀⡆⠀⠀⢠⣆⠀⢀⢀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣷⣧├ 0.200
⠀⠚⠛⠓⠛⠛⠓⠚⠓⠚⠛⠓⠛⠛⠓⠓⠛⠛⠛⠛⠓⠒⠒⠒⠚⠛⠛⠛⠛⠛⠚⠒⠓⠓⠚⠒⠚⠒⠛⠛⠛⠒⠒⠛⠛⠛⠛⠛⠓⠚⠚⠛⠒⠒⠛⠛⠛⠛⠛⠛├ 0.000
 midterm
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀├ 0.600
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣤⣾⡄⠀├ 0.400
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠀⠀⠀⣠⡄⠀⠀⠀⠀⠀⠀⠀⠀⢀⠀⡀⠀⠀⢀⡄⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣷⣧├ 0.200
⠀⠛⠛⠛⠛⠛⠓⠚⠓⠒⠛⠛⠓⠛⠓⠓⠛⠛⠛⠛⠓⠒⠒⠒⠛⠛⠛⠚⠛⠛⠛⠒⠒⠛⠚⠓⠚⠓⠛⠛⠛⠛⠓⠛⠛⠛⠛⠚⠛⠛⠒⠛⠒⠒⠛⠛⠛⠛⠛⠛├ 0.000
 longterm
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀├ 0.600
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⡄⠀├ 0.400
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠀⠀⠀⢀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿⣿⣶├ 0.200
⠀⠒⠛⠛⠛⠛⠒⠚⠓⠒⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠓⠒⠒⠒⠛⠛⠛⠛⠛⠛⠛⠓⠒⠒⠒⠒⠒⠓⠚⠛⠛⠛⠓⠛⠛⠛⠛⠓⠛⠛⠒⠒⠒⠒⠛⠛⠛⠛⠛⠛├ 0.000
             ^00:00:00           ^10:00:00          ^20:00:00
```

ploot-farbfeld
--------------
*ploot-farbfeld* reads collectd-style comma separated values (CSV)
and produces a plot in the [farbfeld](https://tools.suckless.org/farbfeld/)
image format (pipe it to ff2png). It is an alternative to
[RRDtool](https://oss.oetiker.ch/rrdtool/).

It is targetting at generating monitoring graph, and it always read unix
timestamp as first column on standard input.  The first line determines the
name of the curves.

ploot-feed
----------
*ploot-feed* also reads collectd-style comma separated values (CSV) but produces
a plain text continuous waterfall chart for live monitoring in the terminal. it
is an alternative to [grafana](https://grafana.com/).

```
% plootxt 1 1 1 <load-average.csv
          │shortterm           │midterm             │longterm            │
17:34:00 _│⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣯⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
18/05/01  │⣟⡁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
          │⣛⣂⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
20:34:00 _│⣧⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣧⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
18/05/01  │⣧⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣇⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
          │⣟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⡃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
23:34:00 _│⣿⡒⠒⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣷⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
18/05/01  │⡧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⡷⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
          │⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
05:44:41 _│⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⡉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⡉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
18/05/02  │⣛⣁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣟⡁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
          │⣷⠶⠶⠶⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⡷⠶⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⣷⠆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
08:44:41 _│⡗⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⡟⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
18/05/02  │⡯⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
          │⠗⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⡷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
11:44:41 _│⠗⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⡷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⡧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
18/05/02  │⡿⠶⠒⠒⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣷⠖⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣷⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
          │⠖⠒⠒⠒⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣷⠒⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
14:44:41 _│⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣟⡁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
18/05/02  │⣿⠟⠓⠒⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⠓⠒⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⠒⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
          │⣿⠤⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⡿⠆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
17:44:41 _│⡟⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⠒⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣷⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
18/05/02  │⣭⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
          │⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
20:51:38 _│⣶⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣷⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣶⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
18/05/02  │⣿⣷⣶⣶⣶⣶⣶⠖⠒⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⣶⣶⣶⣶⣶⣶⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⣶⣶⣶⣦⣤⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
          │shortterm           │midterm             │longterm            │
22:51:38 _│⣿⣿⣿⣟⣛⡋⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⣿⣿⣟⣛⠛⠛⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⣿⣿⣿⡟⠛⠛⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
18/05/02  │⣿⡿⠍⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⡿⠟⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│⣿⣿⠟⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀│
```
