all : libtracing myprogram

libtracing:
	g++ -shared -o libtracing.so event_trace.cc event_trace_impl.cc -fPIC 

.PHONY : myprogram
myprogram:
	g++ myprogram.cc -L. -ltracing -o myprogram

clean:
	@rm libtracing.so ||:
	@rm myprogram ||: