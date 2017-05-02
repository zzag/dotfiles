# Squash N commits

```sh
$ git rebase -i HEAD~N
```

replace the words "pick" with "squash"

To push changes:

```sh
$ git push origin branchname --force
```


# Undo merge

```sh
$ git reset --hard <sha>
$ git push origin branch-name --force
```

# Sync a repo

```sh
$ git remote add upstream <repo>
$ git fetch upstream
$ git merge upstream/master
```
