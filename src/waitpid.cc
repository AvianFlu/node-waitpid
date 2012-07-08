#include <v8.h>
#include <node.h>
#include <sys/wait.h>
#include <errno.h>

using namespace v8;
using namespace node;

static Handle<Value> Waitpid(const Arguments& args) {
  HandleScope scope;
  int r, target, *status = NULL;

  if (args[0]->IsInt32()) {
    target = args[0]->Int32Value();

    r = waitpid(target, status, NULL);

    if (r == -1) {
      perror("waitpid");
      return ThrowException(Exception::Error(String::New(strerror(errno))));
    }

    if (WIFEXITED(status)) {
      return scope.Close(Integer::New(WEXITSTATUS(status)));
    }
    else if (WIFSIGNALED(status)) {
      return scope.Close(Integer::New(WTERMSIG(status)));
    }
    return scope.Close(Undefined());
  }
  else {
    return ThrowException(Exception::Error(String::New("Not an integer.")));
  }
}


extern "C" void init(Handle<Object> target) {
  HandleScope scope;

  NODE_SET_METHOD(target, "waitpid", Waitpid);
}
