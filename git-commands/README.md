# GIT commands
## git crustimerge
Merge a branch with little care for whitespace. Also known as Uncrustify's best friend.

All it does is calling `git merge -s subtree -X patience -X ignore-space-change -X ignore-all-space -X ignore-space-at-eol [args...]`.

## git pr
Checkout a GitHUB pull request locally, using its URL.

```sh
napoleon.bonaparte ~/repos/myproject [develop =] git pr https://git.mydomain.net/bonaparte/myprojet/pull/42
Switched to branch 'reviews/pr-42'
napoleon.bonaparte ~/repos/myproject [reviews/pr-42 $] 
```

## git nuke-merged
Nuke/prune/obliterate all the branches that got already merged in the master branch or, optionally, the specified branch.

```sh
napoleon.bonaparte ~/repos/myproject [master $=] git nuke-merged release/2012.12.30
Nuking branches already merged into release/2012.12.30
Fetching all from repository

Local branches to be deleted:
bar
foo
reviews/pr-42

Remote branches to be deleted:
origin +refs/heads/bar
origin +refs/heads/foo
Press Ctrl+C to abort, or Enter if it's ok: 
```

Note it doesn't try to delete certain branches:
* master
* develop
* the branch that was passed as first parameter (i.e. the merge branch)
* the currently checked-out branch

Also, it will try to delete branches on any remote (but will fail silently on those you don't have enough privileges).

## git list-notmerged
List the commit from the source branch still not merged into the merge branch.

```sh
napoleon.bonaparte ~/repos/myproject [master =] git list-unmerged origin/master origin/some/feature
```

Note it skips "equivalent" commits (commits already merged, but with a different SHA).
