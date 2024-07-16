# Mika
A Micro Tasking application that helps to break down WORK into easy to execute TASKS. Then allows focusing on the upcoming TASK that pays into advancing the WORK.

# Project state
This project is still in its infantile stage and constantly changing at a fast pace. Therefore, the only branch, main, is almost certainly guaranteed to be rebased continuously until the overhead of feature branches are justified!

# Build
Install dependencies and use the build script `compile.sh` within the build folder.

**>_** `cd build/`<br />
**>_** `./compile.sh`

# Run
Run the `main` binary with or without a compatible `.json` file as argument. E.g. the included `mika_backlog.json` file within the build folder.

**>_** `cd build/`<br />
**>_** `./main mika_backlog.json`

# Backlog
```
└─ Id: 0 - Info: Mika Backlog
   ├─ Id: 1 - Info: Prevent non-int input to be cast into ID.
   ├─ Id: 2 - Info: Add human-readable and diff-able file dump of graph.
   ├─ Id: 3 - Info: Add compatibility to different .json file versions on load.
   ├─ Id: 4 - Info: Implement revising tasks.
   ├─ Id: 5 - Info: Implement splitting tasks.
   ├─ Id: 6 - Info: Implement erasing tasks.
   ├─ Id: 7 - Info: Implement moving tasks.
   └─ Id: 8 - Info: Mika Next Gen
      ├─ Id: 9 - Info: Make previous task ID default for next addition.
      ├─ Id: 10 - Info: Make priority system.
      │  ├─ Id: 11 - Info: Add deadlines.
      │  ├─ Id: 12 - Info: Add user priority score.
      │  └─ Id: 13 - Info: Show most urgent as well as most important tasks.
      └─ Id: 14 - Info: Show metadata of work (stats).

```
