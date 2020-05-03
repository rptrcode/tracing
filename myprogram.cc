#include <iostream>
#include "event_trace.h"
#include "event_trace_plat.h"

using namespace std;
FILE* pFile;

void b() {
  TRACE_EVENT0("category", "b");
}

void a() {
  TRACE_EVENT0("category", "a");
  for (int i = 0; i < 3; i++) {
    sleep(1);
    b();
  }
}
void somefunction() {
  TRACE_EVENT0("category", "somefunction");
  cout << "somefunction" << endl;
  a();
}

// You'll need to define event_trace::OutputCallback, to determine how you want
// the JSON data to get handled:
namespace event_trace {

void OutputCallback(const char* pData) {
  fprintf(pFile, "%s\n", pData);
}

void BufferFullCallback() {
  TRACE_EVENT_FLUSH_LOG();
}
};  // namespace event_trace

int main() {
  pFile = fopen("tracefile.json", "w");

  TRACE_EVENT_BEGIN();  // begin tracing (called at start of app)

  somefunction();

  TRACE_EVENT_FINISH();  // end tracing (called at end of app)

  fclose(pFile);
}
