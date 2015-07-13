#include <v8.h>
#include <node.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

using namespace v8;
using namespace node;

#if NODE_MINOR_VERSION==10
static Handle<Value> Waitpid(const Arguments& args) {
  HandleScope scope;
  int r, child, status;

  if (args[0]->IsInt32()) {
    child = args[0]->Int32Value();

    do {
      r = waitpid(child, &status, 0);
    } while (r != -1);

    Local<Object> result = Object::New();

    if (WIFEXITED(status)) {
      result->Set(String::New("exitCode"), Integer::New(WEXITSTATUS(status)));
      result->Set(String::New("signalCode"), Null());
      return scope.Close(result);
    }
    else if (WIFSIGNALED(status)) {
      result->Set(String::New("exitCode"), Null());
      result->Set(String::New("signalCode"), Integer::New(WTERMSIG(status)));
      return scope.Close(result);
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
#endif

#if NODE_MINOR_VERSION>10
void Waitpid(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  int r, child, status;

  if (args[0]->IsInt32()) {
    child = args[0]->Int32Value();

    do {
      r = waitpid(child, &status, 0);
    } while (r != -1);

    Local<Object> result = Object::New(isolate);

    if (WIFEXITED(status)) {
      result->Set(String::NewFromUtf8(isolate, "exitCode"), Integer::New(isolate, WEXITSTATUS(status)));
      result->Set(String::NewFromUtf8(isolate, "signalCode"), Null(isolate));
      return;
    }
    else if (WIFSIGNALED(status)) {
      result->Set(String::NewFromUtf8(isolate, "exitCode"), Null(isolate));
      result->Set(String::NewFromUtf8(isolate, "signalCode"), Integer::New(isolate, WTERMSIG(status)));
      return;
    }
  }
  else {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Not an integer.")));
  }
}

extern "C" void init(Handle<Object> exports) {
  NODE_SET_METHOD(exports, "waitpid", Waitpid);
}
#endif

NODE_MODULE(waitpid, init)
