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
└─ ID: 0 - Info: Mika Backlog
   ├─ ID: 1 - Info: ̶P̶r̶e̶v̶e̶n̶t̶ ̶n̶o̶n̶-̶i̶n̶t̶ ̶i̶n̶p̶u̶t̶ ̶t̶o̶ ̶b̶e̶ ̶c̶a̶s̶t̶ ̶i̶n̶t̶o̶ ̶I̶D̶.̶
   ├─ ID: 2 - Info: Add human-readable and diff-able file dump of graph.
   ├─ ID: 3 - Info: Add compatibility to different .json file versions on load.
   ├─ ID: 4 - Info: ̶I̶m̶p̶l̶e̶m̶e̶n̶t̶ ̶r̶e̶v̶i̶s̶i̶n̶g̶ ̶t̶a̶s̶k̶s̶.̶
   ├─ ID: 5 - Info: ̶I̶m̶p̶l̶e̶m̶e̶n̶t̶ ̶s̶p̶l̶i̶t̶t̶i̶n̶g̶ ̶t̶a̶s̶k̶s̶ ̶1̶.̶
   │  └─ ID: 17 - Info: ̶I̶m̶p̶l̶e̶m̶e̶n̶t̶ ̶s̶p̶l̶i̶t̶t̶i̶n̶g̶ ̶t̶a̶s̶k̶s̶ ̶2̶.̶
   ├─ ID: 6 - Info: ̶I̶m̶p̶l̶e̶m̶e̶n̶t̶ ̶e̶r̶a̶s̶i̶n̶g̶ ̶t̶a̶s̶k̶s̶.̶
   ├─ ID: 7 - Info: Implement moving tasks.
   ├─ ID: 8 - Info: Mika Next Gen
   │  ├─ ID: 9 - Info: Make previous task ID default for next addition.
   │  ├─ ID: 10 - Info: Make priority system.
   │  │  ├─ ID: 11 - Info: Add deadlines.
   │  │  ├─ ID: 12 - Info: Add user priority score.
   │  │  └─ ID: 13 - Info: Show most urgent as well as most important tasks.
   │  └─ ID: 14 - Info: Show metadata of work (stats).
   ├─ ID: 15 - Info: ̶I̶m̶p̶l̶e̶m̶e̶n̶t̶ ̶m̶a̶r̶k̶i̶n̶g̶ ̶t̶a̶s̶k̶s̶ ̶a̶s̶ ̶d̶o̶n̶e̶.̶
   └─ ID: 16 - Info: ̶I̶m̶p̶l̶e̶m̶e̶n̶t̶ ̶f̶o̶r̶k̶i̶n̶g̶ ̶t̶a̶s̶k̶s̶.̶
```
