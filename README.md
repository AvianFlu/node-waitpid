# node-waitpid

Ever been running a child process, and wished you could just make the parent
node.js process stop and wait until that child exited?

Well, thanks to the tangential needs of some dubious mad science, now you can,
without all the weeping and gnashing of teeth.

```js
var waitpid = require('waitpid');

var status = waitpid(child.pid);

```

Assuming a child process spawned as `child` above, waitpid will suspend the
parent until the child is finished.

## Why would you ever do such a thing!?

Like I said, mad science.  It will make sense eventually.
